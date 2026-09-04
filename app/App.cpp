/**
 * @file App.cpp
 * @brief Application functions
 */

/* ---------------------------------------------------------------------------------- *
 * Headers                                                                            *
 * ---------------------------------------------------------------------------------- */

/* TMS570 supports MPU, so the app will call MPU_ wrappers transparently (because
 * os_mpu_wrappers.h does the remapping). MPU_WRAPPERS_INCLUDED_FROM_API_FILE should not
 * be used here because defining it prevents os_task.h from redefining all the API
 * functions to use the MPU wrappers.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_timer.h"
#include "os_semphr.h"
#include "os_event_groups.h"

/* Peripheral includes */
#include "can.h"
#include "gio.h"
#include "rti.h"
#include "sci.h"
#include "spi.h"
#include "ti_fee.h"
#include <string.h>

#ifdef __cplusplus
}
#endif

/* RTI is blocked by default (this FreeRTOS port uses RTI Counter 0 Compare 0 to
 * generate the system tick). In order to use it, rti.c has to be created and modified
 * directly so that there is no conflict with prvSetupTimerInterrupt(). Periodic events
 * could also be generated using software timers (see vPeriodicTimerCallback).
 */

/* Application includes */
#include "Bridgetek_EVE2.hpp"
#include "dashboard.hpp"
#include "data.hpp"
#include "test_main_ecu.hpp"
#include "ui_buttons.hpp"
#include "ui_rpm_bar.hpp"
#include "ui_touch.hpp"

/* ---------------------------------------------------------------------------------- *
 * Macro constants and definitions                                                    *
 * ---------------------------------------------------------------------------------- */

#define PRIO_MAX (configMAX_PRIORITIES - 1U)
#define TASK_STACK_WORDS(sz_bytes) ((sz_bytes) / sizeof(StackType_t))
#define __FOREVER__ for (;;)

/* Task priorities */
#define PRIO_INIT PRIO_MAX    // Must perform setup
#define PRIO_DATA_UPDATE (6U) // 60Hz data update from CAN
#define PRIO_RULES (5U)       // Compliance checking before actuation
#define PRIO_RF_DATA_RX (4U)  // Receive and dispatch RF data
#define PRIO_DRIVERS_FSM (3U) // Drivers state machine
#define PRIO_CMD (3U)         // Command intake affects drivers control
#define PRIO_TELEMETRY (2U)   // 50Hz telemetry
#define PRIO_UI_INPUT (1U)    // Touch input processing
#define PRIO_DISPLAY (1U)     // 30Hz display refresh

/* Task stack sizes */
#define STACK_INIT TASK_STACK_WORDS(configMINIMAL_STACK_SIZE)
#define STACK_DATA_UPDATE TASK_STACK_WORDS(512)
#define STACK_RULES TASK_STACK_WORDS(512)
#define STACK_DRIVERS_FSM TASK_STACK_WORDS(1024)
#define STACK_RF_DATA_RX TASK_STACK_WORDS(512)
#define STACK_CMD TASK_STACK_WORDS(512)
#define STACK_TELEMETRY TASK_STACK_WORDS(512)
#define STACK_UI_INPUT TASK_STACK_WORDS(256)
#define STACK_DISPLAY TASK_STACK_WORDS(768)

// Event flags (32 bits) ///////////////////////////////////////////////////////////////

/* CAN mailbox updates */
#define EVT_CAN_SENSORS (1 << 0)
#define EVT_CAN_SPEED (1 << 1)
#define EVT_CAN_MOTOR1 (1 << 2)
#define EVT_CAN_MOTOR2 (1 << 3)
#define EVT_CAN_DRIVERS_STATUS (1 << 4)
#define EVT_CAN_DYNAMIC (1 << 5)
/* RF data reception */
#define EVT_SCI_RX_READY (1 << 10)
#define EVT_SCI_TX_READY (1 << 11)
#define EVT_SCI_CMD (1 << 12)
#define EVT_SCI_TELEMETRY_ACK (1 << 13)
#define EVT_TELEMETRY_NACK (1 << 14)
/* Data ready for processing */
#define EVT_NEW_DATA (1 << 15)
#define EVT_DATA_APPROVED (1 << 16)
/* Task ready flags */
#define CMD_READY (1 << 20)
#define TELEMETRY_READY (1 << 21)
/* Periodic events */
#define EVT_PERIODIC_DATA_UPDATE (1 << 25)
#define EVT_PERIODIC_TELEMETRY (1 << 26)
#define EVT_PERIODIC_UI_INPUT (1 << 27)
#define EVT_PERIODIC_DISPLAY_UPDATE (1 << 28)

////////////////////////////////////////////////////////////////////////////////////////

/* CAN mailboxes */
#define SENSORS_MAILBOX canMESSAGE_BOX1        // Rx
#define SPEED_MAILBOX canMESSAGE_BOX2          // Rx
#define MOTOR1_MAILBOX canMESSAGE_BOX3         // Tx
#define MOTOR2_MAILBOX canMESSAGE_BOX4         // Tx
#define DRIVERS_STATUS_MAILBOX canMESSAGE_BOX5 // Rx
#define DYNAMIC_MAILBOX canMESSAGE_BOX6        // Rx
#define BUTTONS_MAILBOX canMESSAGE_BOX9        // Tx
#define STATUS_MAILBOX canMESSAGE_BOX10        // Tx

/* Peripheral registers */
#define CAN_REG canREG1
#define SPI_REG spiREG4
#define SCI_REG sciREG

/* Clock and update rates */
#define CLOCK_RTI_HZ 10000000                      // 10MHz
#define SCREEN_UPDATE_TICKS (CLOCK_RTI_HZ / 30)    // 30Hz
#define DATA_UPDATE_TICKS (CLOCK_RTI_HZ / 60)      // 60Hz
#define TELEMETRY_UPDATE_TICKS (CLOCK_RTI_HZ / 50) // 50Hz

/* RTI notifications */
#define RTI_SCREEN_UPDATE rtiNOTIFICATION_COMPARE1
#define RTI_DATA_UPDATE rtiNOTIFICATION_COMPARE2
#define RTI_TELEMETRY_UPDATE rtiNOTIFICATION_COMPARE3

/* Command reception */
#define CMD_LENGTH 16U

/* SCI identifiers */
#define CMD_ID 0x01
#define TELEMETRY_ACK_ID 0x7E

/* ---------------------------------------------------------------------------------- *
 * Enumerations, structures and typedefs                                              *
 * ---------------------------------------------------------------------------------- */

/* Application tasks */
typedef struct
{
    TaskHandle_t xInitTask;
    TaskHandle_t xDataUpdateTask;
    TaskHandle_t xRulesTask;
    TaskHandle_t xDriversFSMTask;
    TaskHandle_t xRFDataRxTask;
    TaskHandle_t xCmdTask;
    TaskHandle_t xTelemetryTask;
    TaskHandle_t xUIInputTask;
    TaskHandle_t xDisplayTask;
} AppTasks_t;

/* Synchronization and inter-task communication */
typedef struct
{
    /* No queues are needed because data is shared globally from the data and dashboard
     * modules. Tasks can just read the latest data when they receive an event flag.
     */
    EventGroupHandle_t xSysEvents; // Event flags
    SemaphoreHandle_t xDataMutex;  // Protect data if needed
    TimerHandle_t xInitTimer;      // Post scheduler initializations
    TimerHandle_t xPeriodicTimer;  // Periodic events
    TimerHandle_t xTxTimeoutTimer; // Telemetry transmission timeout
} AppSync_t;

/* Application resources (static allocation buffers) */
typedef struct
{
    /* Task TCBs and stacks */
    StaticTask_t xInitTaskBuffer;
    StaticTask_t xDataUpdateTaskBuffer;
    StaticTask_t xRulesTaskBuffer;
    StaticTask_t xDriversFSMTaskBuffer;
    StaticTask_t xRFDataRxTaskBuffer;
    StaticTask_t xCmdTaskBuffer;
    StaticTask_t xTelemetryTaskBuffer;
    StaticTask_t xUIInputTaskBuffer;
    StaticTask_t xDisplayTaskBuffer;
    /* Proper alignment for the TMS570 MPU should be ensured by defining aligned stack
     * types or using compiler attributes. It typically requires regions to be aligned
     * to their size, so stacks and local data arrays should be aligned to their total
     * byte size.
     */
    StackType_t xInitStack[STACK_INIT];
    StackType_t xDataUpdateStack[STACK_DATA_UPDATE];
    StackType_t xRulesStack[STACK_RULES];
    StackType_t xDriversFSMStack[STACK_DRIVERS_FSM];
    StackType_t xRFDataRxStack[STACK_RF_DATA_RX];
    StackType_t xCmdStack[STACK_CMD];
    StackType_t xTelemetryStack[STACK_TELEMETRY];
    StackType_t xUIInputStack[STACK_UI_INPUT];
    StackType_t xDisplayStack[STACK_DISPLAY];

    /* RTOS objects */
    StaticEventGroup_t xSysEventsBuffer;
    StaticSemaphore_t xDataMutexBuffer;
    StaticTimer_t xInitTimerBuffer;
    StaticTimer_t xPeriodicTimerBuffer;
    StaticTimer_t xTxTimeoutTimerBuffer;
} AppResources_t;

/* Data struct for local copies */
typedef struct
{
    sensors_data_t sensors_data;
    speed_data_t speed_data;
    dynamic_data_t dynamic_data;
    motor_data_t motor1_data;
    motor_data_t motor2_data;
    drivers_status_data_t drivers_status_data;
    buttons_data_t buttons_data;
    on_board_status_data_t on_board_status_data;
} AppData_t;

/* Unified app context */
typedef struct
{
    AppResources_t *xResources;
    AppTasks_t *xTasks;
    AppSync_t *xSync;
    AppData_t *xData;
} AppContext_t;

/* ---------------------------------------------------------------------------------- *
 * Prototypes for private functions with file level scope                             *
 * ---------------------------------------------------------------------------------- */

// Initialization //////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize peripherals and modules
 * @note Called before scheduler starts
 */
static void vModulesInit(void);

/**
 * @brief Initialize modules that require interrupts to be enabled
 * @note Called after scheduler starts
 */
static void vInitModulesWithInterrupts(void);

/**
 * @brief Create all application tasks
 * @param pxCtx Pointer to application context containing RTOS resources
 */
static void vCreateTasks(AppContext_t *pxCtx);

/**
 * @brief Create all application objects
 * @param pxCtx Pointer to application context containing RTOS resources
 */
static void vCreateObjects(AppContext_t *pxCtx);

// Tasks ///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Main tasks
 * @param pvParameters Pointer to task parameters used to pass the app context
 * - Initialization task: performs setup that requires interrupts, then deletes itself
 * - Data Update task: processes new CAN data and updates dashboard
 * - Rules task: checks compliance with rules and sets alarms/events
 * - Drivers State Machine task: main control for drivers
 * - RF Data Receive task: receives and dispatches RF data
 * - Command task: processes and validates received commands
 * - Telemetry task: collects vehicle telemetry, packs it and enqueues for transmission
 * - UI Input task: processes touch input and updates dashboard state
 * - Display task: renders dashboard data to the screen
 */
static portTASK_FUNCTION_PROTO(vInitTask, pvParameters);
static portTASK_FUNCTION_PROTO(vDataUpdateTask, pvParameters);
static portTASK_FUNCTION_PROTO(vRulesTask, pvParameters);
static portTASK_FUNCTION_PROTO(vDriversFSMTask, pvParameters);
static portTASK_FUNCTION_PROTO(vRFDataRxTask, pvParameters);
static portTASK_FUNCTION_PROTO(vCmdTask, pvParameters);
static portTASK_FUNCTION_PROTO(vTelemetryTask, pvParameters);
static portTASK_FUNCTION_PROTO(vUIInputTask, pvParameters);
static portTASK_FUNCTION_PROTO(vDisplayTask, pvParameters);

// Callbacks ///////////////////////////////////////////////////////////////////////////

/**
 * @brief Timer callbacks
 * @param xTimer Timer handle
 * - Init Timer: startup initializations that require interrupts
 * - Periodic Timer: periodic events
 * - Tx Timeout: telemetry retransmission
 */
static void vInitTimerCallback(TimerHandle_t xTimer);
static void vPeriodicTimerCallback(TimerHandle_t xTimer);
static void vTxTimeoutCallback(TimerHandle_t xTimer);

/* ---------------------------------------------------------------------------------- *
 * Static variables and constants with file level scope                               *
 * ---------------------------------------------------------------------------------- */

static AppResources_t xAppResources;
static AppTasks_t xAppTasks;
static AppSync_t xAppSync;
static AppData_t xAppData;
static AppContext_t xAppContext = {
    &xAppResources,
    &xAppTasks,
    &xAppSync,
    &xAppData};

/* Global display object */
Bridgetek_EVE2 display_data;

/* ---------------------------------------------------------------------------------- *
 * Global function definitions                                                        *
 * ---------------------------------------------------------------------------------- */

/**
 * @brief Initialize the application
 * @note This function is called once at the beginning of the main program
 */
/* Called from halcogen/sys_main.c, which is C - needs unmangled linkage. */
extern "C" void vAppInit(void)
{
    /* Initialize modules */
    vModulesInit();

    /* Create all tasks and objects */
    vCreateTasks(&xAppContext);
    vCreateObjects(&xAppContext);

    /* Interrupts should be off here, to ensure a tick does not occur before or during
     * the call to vTaskStartScheduler(). The stacks of the created tasks contain a
     * status word with interrupts switched on so they will automatically get re-enabled
     * when the first task starts to run.
     *
     * To initialize peripherals that require interrupts, a dedicated init task can be
     * used that performs the necessary setup and then deletes itself (see vInitTask).
     * Alternatively, use a timer callback that runs once after scheduler start
     * (see vInitTimerCallback).
     */
    // portENABLE_INTERRUPTS();
    // vInitModulesWithInterrupts();
    // portDISABLE_INTERRUPTS();

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();
}

/**
 * @brief Run the application
 * @note This function is called constantly in an infinite loop
 */
/* Called from halcogen/sys_main.c, which is C - needs unmangled linkage. */
extern "C" void vAppRun(void)
{
    /* All work done in tasks */
}

// Notifications ///////////////////////////////////////////////////////////////////////

/* Notifications can be defined in notification.c, or be redefined externally thanks to
 * the WEAK pragma. If a link set contains both a weak definition (notification.c) and a
 * non-weak definition (e.g., App.cpp), the non-weak definition is always used. In
 * FreeRTOS, always use _FromISR APIs (interrupt safe) and the xHigherPriorityTaskWoken
 * parameter to request a context switch.
 */

/**
 * @brief Callback for CAN interrupts
 * @param node CAN controller base address
 * @param messageBox Mailbox number that triggered the interrupt
 */
void canMessageNotification(canBASE_t *node, uint32 messageBox)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    /* TODO: The DataUpdate task is periodic, so EVT_CAN_x events are not necessary */

    /* Set event bits and data flags */
    if (node == CAN_REG)
    {
        switch (messageBox)
        {
        case SENSORS_MAILBOX:
            sensors_data.new_data = true;
            xEvent = EVT_CAN_SENSORS;
            break;
        case SPEED_MAILBOX:
            speed_data.new_data = true;
            xEvent = EVT_CAN_SPEED;
            break;
        case MOTOR1_MAILBOX:
            motor1_data.new_data = true;
            xEvent = EVT_CAN_MOTOR1;
            break;
        case MOTOR2_MAILBOX:
            motor2_data.new_data = true;
            xEvent = EVT_CAN_MOTOR2;
            break;
        case DRIVERS_STATUS_MAILBOX:
            drivers_status_data.new_data = true;
            xEvent = EVT_CAN_DRIVERS_STATUS;
            break;
        case DYNAMIC_MAILBOX:
            dynamic_data.new_data = true;
            xEvent = EVT_CAN_DYNAMIC;
            break;
        default:
            break;
        }
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != NULL))
    {
        if (xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken) == pdFAIL)
        {
            /* Handle error */
        }

        if (xHigherPriorityTaskWoken != pdFALSE)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/**
 * @brief Callback for RTI interrupts
 * @param notification RTI notification type
 */
void rtiNotification(uint32 notification)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    /* Signal periodic events */
    switch (notification)
    {
    case RTI_DATA_UPDATE: // 60Hz
        xEvent = EVT_PERIODIC_DATA_UPDATE;
        break;
    case RTI_SCREEN_UPDATE: // 30Hz
        xEvent = EVT_PERIODIC_DISPLAY_UPDATE;
        break;
    case RTI_TELEMETRY_UPDATE: // 50Hz
        xEvent = EVT_PERIODIC_TELEMETRY;
        break;
    default:
        break;
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != NULL))
    {
        if (xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken) == pdFAIL)
        {
            /* Handle error */
        }

        if (xHigherPriorityTaskWoken != pdFALSE)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/**
 * @brief Callback for SCI interrupts
 * @param sci SCI module base address
 * @param flags Status flags indicating the cause of the interrupt
 */
void sciNotification(sciBASE_t *sci, uint32 flags)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    if (sci == SCI_REG)
    {
        if ((flags & SCI_RX_INT) != 0U)
            xEvent = EVT_SCI_RX_READY;
        else if ((flags & SCI_TX_INT) != 0U)
            xEvent = EVT_SCI_TX_READY;
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != NULL))
    {
        if (xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken) == pdFAIL)
        {
            /* Handle error */
        }

        if (xHigherPriorityTaskWoken != pdFALSE)
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// Static Internal Kernel Memory Allocation ////////////////////////////////////////////

/* When the static memory allocator is enabled (configSUPPORT_STATIC_ALLOCATION = 1),
 * the idle task and the timer task (configUSE_TIMERS = 1) will use static memory
 * supplied by user functions. The kernel will call vApplicationGet_TaskMemory() to
 * allow the application to create and return memory buffers for the TCB and stack. The
 * idle task is run when a core runs out of scheduled work. It performs some
 * housekeeping and can also trigger the user's vTaskIdleHook() if it is enabled.
 */

/**
 * @brief Provide static memory for the FreeRTOS idle and timer tasks
 * @param ppx_TaskTCBBuffer Pointer to receive the TCB buffer
 * @param ppx_TaskStackBuffer Pointer to receive the stack buffer
 * @param pul_TaskStackSize Pointer to receive the stack size in words
 */
#ifdef __cplusplus
extern "C"
{
#endif

    void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                       StackType_t **ppxIdleTaskStackBuffer,
                                       uint32_t *pulIdleTaskStackSize)
    {
        static StaticTask_t xIdleTaskTCB;
        static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

        *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
        *ppxIdleTaskStackBuffer = uxIdleTaskStack;
        *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    }

    void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                        StackType_t **ppxTimerTaskStackBuffer,
                                        uint32_t *pulTimerTaskStackSize)
    {
        static StaticTask_t xTimerTaskTCB;
        static StackType_t uxTimerTaskStack[configMINIMAL_STACK_SIZE];

        *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
        *ppxTimerTaskStackBuffer = uxTimerTaskStack;
        *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
    }

#ifdef __cplusplus
}
#endif

/* ---------------------------------------------------------------------------------- *
 * Local function definitions                                                         *
 * ---------------------------------------------------------------------------------- */

// Initialization //////////////////////////////////////////////////////////////////////

static void vModulesInit(void)
{
    /* Initialize peripherals */
    canInit();
    gioInit();
    rtiInitPeriodicCompares(); // Program compares 1-3 reload values (30/60/50Hz).
                               // Does not touch compare 0 or any register the
                               // FreeRTOS port's timer setup owns -- safe pre-scheduler.
    sciInit();
    spiInit();

    /* Initialize EEPROM/Flash */
    TI_Fee_Init();
    while (TI_Fee_GetStatus(0) != IDLE)
        TI_Fee_MainFunction();

    /* Initialize display */
    display_data.setup(WQVGA);
    display_data.Init();

    /* Try restoring touch + debug calibration from EEPROM */
    if (!loadCalibration(display_data))
    {
        /* If not found, perform fresh touch calibration */
        eve_calibrate(display_data);
        saveCalibration(display_data);
    }

    /* Initialize data and dashboard structures */
    init_data();
    test_main_ecu_init();
    init_dashboard(&dashboard_data);

    /* Enable peripheral callbacks */
    canEnableErrorNotification(CAN_REG);
    canEnableStatusChangeNotification(CAN_REG);
    // NOTE: RTI compare 1-3 notifications are NOT enabled here. FreeRTOS's
    // prvSetupTimerInterrupt() (called from vTaskStartScheduler(), which runs
    // after this function) clears ALL RTI interrupt-enable bits and
    // re-enables only compare 0 -- enabling these here would get silently
    // wiped. They are enabled instead from vInitModulesWithInterrupts(),
    // which runs post-scheduler-start from vInitTask.
    // sciEnableNotification(SCI_REG, SCI_TX_INT | SCI_RX_INT);
    sciEnableNotification(SCI_REG, 0); // Enabled in HALCoGen
    // spiEnableNotification(SPI_REG, 0);
}

/**
 * @brief Initialize modules that require interrupts to be enabled
 * @note Called from vInitTask or vInitTimerCallback after scheduler starts
 */
static void vInitModulesWithInterrupts(void)
{
    /* Start RTI counter for periodic interrupts */
    rtiStartCounter(rtiCOUNTER_BLOCK0);

    /* Enable compare 1-3 interrupts here (after the scheduler's own timer
     * setup has already run and cleared/re-set the RTI interrupt-enable
     * bits) so display/data-update/telemetry ticks actually reach the app. */
    rtiEnableNotification(RTI_SCREEN_UPDATE);
    rtiEnableNotification(RTI_DATA_UPDATE);
    rtiEnableNotification(RTI_TELEMETRY_UPDATE);
}

/**
 * @brief Create all application tasks
 */
static void vCreateTasks(AppContext_t *pxCtx)
{
    AppResources_t *pxRes = pxCtx->xResources;
    AppTasks_t *pxTasks = pxCtx->xTasks;

    configASSERT((pxRes != NULL) && (pxTasks != NULL));

    pxTasks->xInitTask = xTaskCreateStatic(
        vInitTask,
        "Init",
        STACK_INIT,
        (void *)pxCtx,
        PRIO_INIT,
        pxRes->xInitStack,
        &pxRes->xInitTaskBuffer);
    configASSERT(pxTasks->xInitTask != NULL);

    pxTasks->xDataUpdateTask = xTaskCreateStatic(
        vDataUpdateTask,
        "DataUpdate",
        STACK_DATA_UPDATE,
        (void *)pxCtx,
        PRIO_DATA_UPDATE,
        pxRes->xDataUpdateStack,
        &pxRes->xDataUpdateTaskBuffer);
    configASSERT(pxTasks->xDataUpdateTask != NULL);

    pxTasks->xRulesTask = xTaskCreateStatic(
        vRulesTask,
        "Rules",
        STACK_RULES,
        (void *)pxCtx,
        PRIO_RULES,
        pxRes->xRulesStack,
        &pxRes->xRulesTaskBuffer);
    configASSERT(pxTasks->xRulesTask != NULL);

    pxTasks->xDriversFSMTask = xTaskCreateStatic(
        vDriversFSMTask,
        "DriversFSM",
        STACK_DRIVERS_FSM,
        (void *)pxCtx,
        PRIO_DRIVERS_FSM,
        pxRes->xDriversFSMStack,
        &pxRes->xDriversFSMTaskBuffer);
    configASSERT(pxTasks->xDriversFSMTask != NULL);

    pxTasks->xRFDataRxTask = xTaskCreateStatic(
        vRFDataRxTask,
        "RFDataRx",
        STACK_RF_DATA_RX,
        (void *)pxCtx,
        PRIO_RF_DATA_RX,
        pxRes->xRFDataRxStack,
        &pxRes->xRFDataRxTaskBuffer);
    configASSERT(pxTasks->xRFDataRxTask != NULL);

    pxTasks->xCmdTask = xTaskCreateStatic(
        vCmdTask,
        "Cmd",
        STACK_CMD,
        (void *)pxCtx,
        PRIO_CMD,
        pxRes->xCmdStack,
        &pxRes->xCmdTaskBuffer);
    configASSERT(pxTasks->xCmdTask != NULL);

    pxTasks->xTelemetryTask = xTaskCreateStatic(
        vTelemetryTask,
        "Telemetry",
        STACK_TELEMETRY,
        (void *)pxCtx,
        PRIO_TELEMETRY,
        pxRes->xTelemetryStack,
        &pxRes->xTelemetryTaskBuffer);
    configASSERT(pxTasks->xTelemetryTask != NULL);

    // pxTasks->xUIInputTask = xTaskCreateStatic(
    //     vUIInputTask,
    //     "UIInput",
    //     STACK_UI_INPUT,
    //     (void *)pxCtx,
    //     PRIO_UI_INPUT,
    //     pxRes->xUIInputStack,
    //     &pxRes->xUIInputTaskBuffer);
    // configASSERT(pxTasks->xUIInputTask != NULL);

    pxTasks->xDisplayTask = xTaskCreateStatic(
        vDisplayTask,
        "Display",
        STACK_DISPLAY,
        (void *)pxCtx,
        PRIO_DISPLAY,
        pxRes->xDisplayStack,
        &pxRes->xDisplayTaskBuffer);
    configASSERT(pxTasks->xDisplayTask != NULL);
}

static void vCreateObjects(AppContext_t *pxCtx)
{
    AppResources_t *pxRes = pxCtx->xResources;
    AppSync_t *pxSync = pxCtx->xSync;

    configASSERT((pxRes != NULL) && (pxSync != NULL));

    /* Create event group for synchronization flags */
    pxSync->xSysEvents = xEventGroupCreateStatic(&pxRes->xSysEventsBuffer);
    configASSERT(pxSync->xSysEvents != NULL);

    /* Create mutex for data */
    pxSync->xDataMutex = xSemaphoreCreateMutexStatic(&pxRes->xDataMutexBuffer);
    configASSERT(pxSync->xDataMutex != NULL);

    /* Timer that starts after vTaskStartScheduler (interrupts enabled) */
    // pxSync->xInitTimer = xTimerCreate("InitTimer", 1, pdFALSE, NULL, vInitTimerCallback);
    // configASSERT(pxSync->xInitTimer != NULL);
    // xTimerStart(pxSync->xInitTimer, 0);

    /* Timer for periodic events if RTI notifications are not used */
    // pxSync->xPeriodicTimer = xTimerCreate("PeriodicTimer", pdMS_TO_TICKS(10), pdTRUE, NULL, vPeriodicTimerCallback);
    // configASSERT(pxSync->xPeriodicTimer != NULL);
    // xTimerStart(pxSync->xPeriodicTimer, 0);

    /* Timer for telemetry transmission timeout */
    pxSync->xTxTimeoutTimer = xTimerCreate("TxTimeoutTimer", pdMS_TO_TICKS(5), pdFALSE, NULL, vTxTimeoutCallback);
    configASSERT(pxSync->xTxTimeoutTimer != NULL);
}

// Tasks ///////////////////////////////////////////////////////////////////////////////

static portTASK_FUNCTION(vInitTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;

    /* Initialize modules that require interrupts */
    vInitModulesWithInterrupts();

    /* Delete self */
    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vDataUpdateTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData;
    EventBits_t uxBits;

    __FOREVER__
    {
        /* Wait for 60Hz periodic event */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_PERIODIC_DATA_UPDATE,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if ((uxBits & EVT_PERIODIC_DATA_UPDATE) != 0U)
        {
            /* Read new frames and update all data structures */
            update_data();

            /* Update dashboard buffers */
            update_dashboard_data(&dashboard_data);

            /* Handle touch input */
            ui_handle_touch(display_data, &dashboard_data);

            /* Update local copy */
            if (xSemaphoreTake(pxSync->xDataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
            {
                /* Tasks need data that is mutex locked for consistency, so to avoid
                 * blocking update_data() and losing frames, local copies are used.
                 */
                pxData->sensors_data = sensors_data;
                pxData->speed_data = speed_data;
                pxData->dynamic_data = dynamic_data;
                pxData->motor1_data = motor1_data;
                pxData->motor2_data = motor2_data;
                pxData->drivers_status_data = drivers_status_data;
                pxData->buttons_data = buttons_data;
                pxData->on_board_status_data = on_board_status_data;

                xSemaphoreGive(pxSync->xDataMutex);
                xEventGroupSetBits(pxSync->xSysEvents, EVT_NEW_DATA);
            }
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vRulesTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData;
    EventBits_t uxBits;
    struct
    {
        uint16_t tps;
        uint16_t brake_front;
        /* brake_rear: no rear sensor board on the bus yet */
        /* ... */
    } xLocalData;

    __FOREVER__
    {
        /* Wait for new data event */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_NEW_DATA,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        /* Local copy of the fields that need rule checking */
        if (xSemaphoreTake(pxSync->xDataMutex, portMAX_DELAY) == pdTRUE)
        {
            xLocalData.tps = pxData->sensors_data.values.tps1;
            xLocalData.brake_front = pxData->speed_data.values.front_brake;
            /* ... */

            xSemaphoreGive(pxSync->xDataMutex);

            /* This is not needed if rule checking is periodic */
            xEventGroupSetBits(pxSync->xSysEvents, EVT_DATA_APPROVED);
        }

        /////////////////////////////////////////////
        // TODO: Placeholder for compliance checks //
        /////////////////////////////////////////////
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vDriversFSMTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData;
    EventBits_t uxBits;
    struct
    {
        uint8_t drive_enabled;
        float motor1_voltage;
        float motor1_current;
        float motor2_voltage;
        float motor2_current;
        uint8_t drive_state;
        /* ... */
    } xLocalData;

    __FOREVER__
    {
        /* Wait for new approved data */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_DATA_APPROVED,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        /* Local copy of control-related state */
        if (xSemaphoreTake(pxSync->xDataMutex, portMAX_DELAY) == pdTRUE)
        {
            xLocalData.drive_enabled = pxData->buttons_data.values.drive_enabled;
            xLocalData.motor1_voltage = pxData->motor1_data.values.motor_voltage;
            xLocalData.motor1_current = pxData->motor1_data.values.motor_current;
            xLocalData.motor2_voltage = pxData->motor2_data.values.motor_voltage;
            xLocalData.motor2_current = pxData->motor2_data.values.motor_current;
            xLocalData.drive_state = pxData->drivers_status_data.values.drive_state;
            /* ... */

            xSemaphoreGive(pxSync->xDataMutex);
        }

        ////////////////////////////////////////////
        // TODO: Placeholder for driver FSM logic //
        ////////////////////////////////////////////
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vRFDataRxTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;
    uint8_t xRxBuff[64], xId; // Buffer for RF data

    __FOREVER__
    {
        /* Avoid starting a new transfer before the previous one is handled */
        // uxBits = xEventGroupWaitBits(
        //     pxSync->xSysEvents,
        //     CMD_READY | TELEMETRY_READY, // Wait if either is busy
        //     pdTRUE,                      // Clear bits after return
        //     pdTRUE,                      // Wait for all bits
        //     portMAX_DELAY);

        /* Setup RF data reception */
        sciReceive(SCI_REG, 1, xRxBuff);

        /* Wait for the transfer to complete */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_SCI_RX_READY,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if ((uxBits & EVT_SCI_RX_READY) != 0U)
        {
            /* Read the ID byte that was just received into xRxBuff[0] */
            xId = xRxBuff[0];

            //////////////////////////////////////////////
            // TODO: Placeholder for RF data processing //
            //////////////////////////////////////////////

            /* Set data ready flag for the relevant task */
            EventBits_t uxSetBits = 0U;
            switch (xId)
            {
            case CMD_ID:                 // Command identifier
                uxSetBits = EVT_SCI_CMD; // | TELEMETRY_READY;
                break;
            case TELEMETRY_ACK_ID:                 // Telemetry ack identifier
                uxSetBits = EVT_SCI_TELEMETRY_ACK; // | CMD_READY;
                break;
            default:
                break;
            }

            if (uxSetBits != 0U)
            {
                xEventGroupSetBits(pxSync->xSysEvents, uxSetBits);
            }
        }
    }

    vTaskDelete(NULL);
}

// static portTASK_FUNCTION(vRFDataTxTask, pvParameters)
// {
//     AppContext_t *pxCtx = (AppContext_t *)pvParameters;
//     (void)pxCtx;

//     __FOREVER__
//     {
//         /* Unlike Rx, Tx can be done inside tasks as long as proper synchronization is
//          * maintained, such as using a mutex to block sciSend from being called
//          * concurrently.
//          */
//     }
// }

static portTASK_FUNCTION(vCmdTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;
    uint8_t xRxBuff[CMD_LENGTH]; // Buffer for Cmd data

    __FOREVER__
    {
        /* Wait for the transfer to complete */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_SCI_CMD,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if ((uxBits & EVT_SCI_CMD) != 0U)
        {
            //////////////////////////////////////////
            // TODO: Placeholder for cmd processing //
            //////////////////////////////////////////
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vTelemetryTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData;
    EventBits_t uxBits = 0U;
    uint8_t xTxBuff[64] = {0}, xRxBuff[64] = {0}; // Buffers for telemetry data
    // size_t xTxLen = 0, xRxLen = 0;             // Length of data to send/receive
    struct
    {
        float rpm;
        float battery_voltage;
        float battery_current;
        float wheel_speed_fl;
        float wheel_speed_fr;
        uint8_t motor1_fault;
        uint8_t motor2_fault;
        /* ... */
    } xLocalData;

    __FOREVER__
    {
        /* Wait for 50Hz periodic event or telemetry ack/nack */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_PERIODIC_TELEMETRY | EVT_SCI_TELEMETRY_ACK | EVT_TELEMETRY_NACK,
            pdTRUE,  // Clear bits after return -- must clear, or a non-clearing
                     // wait returns immediately forever once any bit is set once
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if (((uxBits & EVT_PERIODIC_TELEMETRY) != 0U) || ((uxBits & EVT_TELEMETRY_NACK) != 0U))
        {
            if (((uxBits & EVT_PERIODIC_TELEMETRY) != 0U) &&
                xSemaphoreTake(pxSync->xDataMutex, portMAX_DELAY) == pdTRUE)
            {
                /* Local copy of telemetry data */
                xLocalData.rpm = pxData->dynamic_data.values.rpm;
                xLocalData.battery_voltage = pxData->dynamic_data.values.battey_voltage;
                xLocalData.battery_current = pxData->dynamic_data.values.battery_current;
                xLocalData.wheel_speed_fl = pxData->speed_data.values.wheel_speed_fl;
                xLocalData.wheel_speed_fr = pxData->speed_data.values.wheel_speed_fr;
                xLocalData.motor1_fault = pxData->drivers_status_data.values.motor1_fault;
                xLocalData.motor2_fault = pxData->drivers_status_data.values.motor2_fault;
                /* ... */

                xSemaphoreGive(pxSync->xDataMutex);

                /* Raw fixed-layout pack (no COBS/MessagePack framing yet --
                 * both are vendored but unwired; this is a stopgap so real
                 * data goes out now). Layout, all little-endian:
                 *   [0..3]   float  rpm
                 *   [4..7]   float  battery_voltage
                 *   [8..11]  float  battery_current
                 *   [12..15] float  wheel_speed_fl
                 *   [16..19] float  wheel_speed_fr
                 *   [20]     uint8  motor1_fault
                 *   [21]     uint8  motor2_fault
                 *   [22..63] zero padding
                 * A ground-station script must parse this exact layout. */
                memset(xTxBuff, 0, sizeof(xTxBuff));
                memcpy(&xTxBuff[0],  &xLocalData.rpm,             sizeof(xLocalData.rpm));
                memcpy(&xTxBuff[4],  &xLocalData.battery_voltage, sizeof(xLocalData.battery_voltage));
                memcpy(&xTxBuff[8],  &xLocalData.battery_current, sizeof(xLocalData.battery_current));
                memcpy(&xTxBuff[12], &xLocalData.wheel_speed_fl,  sizeof(xLocalData.wheel_speed_fl));
                memcpy(&xTxBuff[16], &xLocalData.wheel_speed_fr,  sizeof(xLocalData.wheel_speed_fr));
                xTxBuff[20] = xLocalData.motor1_fault;
                xTxBuff[21] = xLocalData.motor2_fault;
            }

            /* Send telemetry data to XBee via UART */
            // sciSend(SCI_REG, len, buffer); // For variable length messages
            sciSend(SCI_REG, sizeof(xTxBuff), xTxBuff);

            /* Set a timer to handle retransmission */
            xTimerStart(pxSync->xTxTimeoutTimer, 0);
        }
        else if ((uxBits & EVT_SCI_TELEMETRY_ACK) != 0U)
        {
            /* Telemetry acknowledged, can send next packet */
            xTimerStop(pxSync->xTxTimeoutTimer, 0);
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vUIInputTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    (void)pxCtx;

    __FOREVER__
    {
        /* This is currently done in the DataUpdate task */

        ////////////////////////////////////////////
        // TODO: Placeholder for input processing //
        ////////////////////////////////////////////
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vDisplayTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;

    __FOREVER__
    {
        /* Wait for 30Hz periodic event */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_PERIODIC_DISPLAY_UPDATE,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if ((uxBits & EVT_PERIODIC_DISPLAY_UPDATE) != 0U)
        {
            /* Update display rendering */
            update_dashboard_draw(display_data, &dashboard_data);
        }
    }

    vTaskDelete(NULL);
}

// Callbacks ///////////////////////////////////////////////////////////////////////////

static void vInitTimerCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    /* Initialize modules that require interrupts */
    vInitModulesWithInterrupts();

    /* Delete self */
    xTimerDelete(xTimer, 0);
}

static void vPeriodicTimerCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    /* This callback can be used to signal periodic events if RTI notifications are not
     * available. Multiple timers with different periods can be implemented with the
     * same callback by using their pvTimerID parameter to identify which timer expired.
     */
}

static void vTxTimeoutCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    xEventGroupSetBits(xAppContext.xSync->xSysEvents, EVT_TELEMETRY_NACK);
}
