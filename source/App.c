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
#include "ti_fee.h"

/* Application includes */
#include "data.h"
#include "test/test_App.h"

/* External libraries */
#include "cobs/cobs.h"
#include "msgpack/msgpack.h"
#include "xbee/xbee.h"

/* ---------------------------------------------------------------------------------- *
 * Macro constants and definitions                                                    *
 * ---------------------------------------------------------------------------------- */

#define PRIO_MAX (configMAX_PRIORITIES - 1U)
#define TASK_STACK_WORDS(sz_bytes) ((sz_bytes) / sizeof(StackType_t))
#define __FOREVER__ for (;;)

#ifndef ENABLE_APP_TEST
#define ENABLE_APP_TEST
#endif

/* Task priorities */
#define PRIO_INIT PRIO_MAX    // Must perform setup
#define PRIO_DATA_UPDATE (6U) // 60Hz data update from CAN
#define PRIO_RULES (5U)       // Compliance checking before actuation
#define PRIO_RF_DATA_RX (4U)  // Receive and dispatch RF data
#define PRIO_DRIVERS_FSM (3U) // Drivers state machine
#define PRIO_CMD (3U)         // Command intake affects drivers control
#define PRIO_TELEMETRY (2U)   // 50Hz telemetry

/* Task stack sizes */
#define STACK_INIT TASK_STACK_WORDS(configMINIMAL_STACK_SIZE)
#define STACK_DATA_UPDATE TASK_STACK_WORDS(512)
#define STACK_RULES TASK_STACK_WORDS(512)
#define STACK_DRIVERS_FSM TASK_STACK_WORDS(1024)
#define STACK_RF_DATA_RX TASK_STACK_WORDS(512)
#define STACK_CMD TASK_STACK_WORDS(512)
#define STACK_TELEMETRY TASK_STACK_WORDS(512)

// Event flags (32 bits) ///////////////////////////////////////////////////////////////

/* CAN mailbox updates */
#define EVT_CAN_FADC1 (1 << 0)
#define EVT_CAN_FADC2 (1 << 1)
#define EVT_CAN_RADC1 (1 << 2)
#define EVT_CAN_RADC2 (1 << 3)
#define EVT_CAN_DRV1 (1 << 4)
#define EVT_CAN_DRV2 (1 << 5)
/* RF data reception */
#define EVT_SCI_RX_READY (1 << 10)
#define EVT_SCI_TX_READY (1 << 11)
#define EVT_SCI_CMD (1 << 12)
#define EVT_SCI_TELEMETRY_ACK (1 << 13)
#define EVT_TELEMETRY_NACK (1 << 14)
/* Data ready for processing */
#define EVT_NEW_DATA (EVT_CAN_FADC1 | EVT_CAN_FADC2 | EVT_CAN_RADC1 | EVT_CAN_RADC2 | EVT_CAN_DRV1 | EVT_CAN_DRV2)
#define EVT_DATA_UPDATE (1 << 15)
#define EVT_DATA_APPROVED (1 << 16)
/* Task ready flags */
#define EVT_CMD_READY (1 << 20)
#define EVT_TELEMETRY_READY (1 << 21)
/* Periodic events */
#define EVT_PERIODIC_TELEMETRY (1 << 25)

// CAN mailboxes ///////////////////////////////////////////////////////////////////////

/* ADC2CAN */
#define FADC1_MAILBOX canMESSAGE_BOX1 // Rx
#define FADC2_MAILBOX canMESSAGE_BOX2 // Rx
#define RADC1_MAILBOX canMESSAGE_BOX3 // Rx
#define RADC2_MAILBOX canMESSAGE_BOX4 // Rx
/* Main ECU */
#define DRV_STATUS_MAILBOX canMESSAGE_BOX5 // Tx
#define ERR_MSG_MAILBOX canMESSAGE_BOX6    // Tx
#define CALIB_MAILBOX canMESSAGE_BOX9      // Tx, base for 0x50n messages
/* Drivers */
#define DRV1_MAILBOX canMESSAGE_BOX7      // Rx
#define DRV2_MAILBOX canMESSAGE_BOX8      // Rx
#define DRV_CTRL_MAILBOX canMESSAGE_BOX17 // Tx

////////////////////////////////////////////////////////////////////////////////////////

/* Peripheral registers */
#define CAN_REG canREG1
#define SCI_REG sciREG

/* Clock and update rates */
// #define CLOCK_RTI_HZ 1000000                       // 1MHz
// #define TELEMETRY_UPDATE_TICKS (CLOCK_RTI_HZ / 50) // 50Hz

/* RTI is blocked by default (this FreeRTOS port uses RTI Counter 0 Compare 0 to
 * generate the system tick). In order to use it, rti.c has to be manually added to the
 * project. Modify prvSetupTimerInterrupt() in os_port.c as needed to avoid conflicts
 * with rtiInit(). Periodic events could also be generated using software timers (see
 * vPeriodicTimerCallback).
 */

/* RTI notifications */
#define RTI_TELEMETRY_UPDATE rtiNOTIFICATION_COMPARE1

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
} AppTasks_t;

/* Synchronization and inter-task communication */
typedef struct
{
    /* No queues are needed because data is shared globally from the data and dashboard
     * modules. Tasks can just read the latest data when they receive an event flag.
     */
    EventGroupHandle_t xSysEvents; // Event flags
    /* Tasks need data that is mutex locked for consistency, so to avoid blocking
     * updates and losing data, local copies should be used to maintain protected access
     * as short as possible.
     */
    SemaphoreHandle_t xDataMutex;
    // TimerHandle_t xInitTimer;      // Post scheduler initializations
    // TimerHandle_t xPeriodicTimer;  // Periodic events
    TimerHandle_t xTxTimeoutTimer; // Telemetry transmission timeout
#ifdef ENABLE_APP_TEST
    TimerHandle_t xAppTestTimer; // Timer for test app events
#endif
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

    /* RTOS objects */
    StaticEventGroup_t xSysEventsBuffer;
    StaticSemaphore_t xDataMutexBuffer;
    // StaticTimer_t xInitTimerBuffer;
    // StaticTimer_t xPeriodicTimerBuffer;
    StaticTimer_t xTxTimeoutTimerBuffer;
#ifdef ENABLE_APP_TEST
    StaticTimer_t xAppTestTimerBuffer;
#endif
} AppResources_t;

/* Data struct for local copies */
typedef struct
{
    DataFADC1_t xFADC1;
    DataFADC2_t xFADC2;
    DataRADC1_t xRADC1;
    DataRADC2_t xRADC2;
    DataDrv1_t xDrv1;
    DataDrv2_t xDrv2;
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
 * Function prototypes with global scope                                              *
 * ---------------------------------------------------------------------------------- */

/**
 * @brief Initialize the application
 * @note This function is called once at the beginning of the main program
 */
void vAppInit(void);

/**
 * @brief Run the application
 * @note This function is called constantly in an infinite loop
 */
void vAppRun(void);

/**
 * @brief Provide static memory for the FreeRTOS idle and timer tasks
 * @param ppx_TaskTCBBuffer Pointer to receive the TCB buffer
 * @param ppx_TaskStackBuffer Pointer to receive the stack buffer
 * @param pul_TaskStackSize Pointer to receive the stack size in words
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);

/* ---------------------------------------------------------------------------------- *
 * Prototypes for private functions with file level scope                             *
 * ---------------------------------------------------------------------------------- */

// Initialization //////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize peripherals and modules
 * @note Called before scheduler starts
 */
static void vAppInitModules(void);

/**
 * @brief Initialize modules that require interrupts to be enabled
 * @note Called after scheduler starts
 */
static void vAppInitModulesWithInterrupts(void);

/**
 * @brief Create all application tasks
 * @param pxCtx Pointer to application context containing RTOS resources
 */
static void vAppCreateTasks(AppContext_t *pxCtx);

/**
 * @brief Create all application objects
 * @param pxCtx Pointer to application context containing RTOS resources
 */
static void vAppCreateObjects(AppContext_t *pxCtx);

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
 */
static portTASK_FUNCTION_PROTO(vInitTask, pvParameters);
static portTASK_FUNCTION_PROTO(vDataUpdateTask, pvParameters);
static portTASK_FUNCTION_PROTO(vRulesTask, pvParameters);
static portTASK_FUNCTION_PROTO(vDriversFSMTask, pvParameters);
static portTASK_FUNCTION_PROTO(vRFDataRxTask, pvParameters);
static portTASK_FUNCTION_PROTO(vCmdTask, pvParameters);
static portTASK_FUNCTION_PROTO(vTelemetryTask, pvParameters);

// Callbacks ///////////////////////////////////////////////////////////////////////////

/**
 * @brief Timer callbacks
 * @param xTimer Timer handle
 * - Init Timer: startup initializations that require interrupts
 * - Periodic Timer: periodic events
 * - Tx Timeout: telemetry retransmission
 * - App Test Timer: trigger test app events (if enabled)
 */
// static void vInitTimerCallback(TimerHandle_t xTimer);
// static void vPeriodicTimerCallback(TimerHandle_t xTimer);
static void vTxTimeoutCallback(TimerHandle_t xTimer);
#ifdef ENABLE_APP_TEST
static void vAppTestTimerCallback(TimerHandle_t xTimer);
#endif

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

/* Parsed XBee API frame data */
// static XBeeAPIParsedFrame_t xParsedFrame;

/* TODO: Modify the xbee module to allow configurable addresses */
static const uint64_t dest64 = 0x0013A20040B2B3D4; // Example 64-bit destination address
static const uint16_t dest16 = 0xFFFE;             // Example 16-bit destination address

/* ---------------------------------------------------------------------------------- *
 * Global function definitions                                                        *
 * ---------------------------------------------------------------------------------- */

void vAppInit(void)
{
    /* Initialize modules */
    vAppInitModules();

    /* Create all tasks and objects */
    vAppCreateTasks(&xAppContext);
    vAppCreateObjects(&xAppContext);

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
    // vAppInitModulesWithInterrupts();
    // portDISABLE_INTERRUPTS();

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();
}

void vAppRun(void)
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

    /* Set event bits and data flags */
    if (node == CAN_REG)
    {
        switch (messageBox)
        {
        case FADC1_MAILBOX:
            xEvent = EVT_CAN_FADC1;
            break;
        case FADC2_MAILBOX:
            xEvent = EVT_CAN_FADC2;
            break;
        case RADC1_MAILBOX:
            xEvent = EVT_CAN_RADC1;
            break;
        case RADC2_MAILBOX:
            xEvent = EVT_CAN_RADC2;
            break;
        case DRV1_MAILBOX:
            xEvent = EVT_CAN_DRV1;
            break;
        case DRV2_MAILBOX:
            xEvent = EVT_CAN_DRV2;
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

/* ---------------------------------------------------------------------------------- *
 * Local function definitions                                                         *
 * ---------------------------------------------------------------------------------- */

// Initialization //////////////////////////////////////////////////////////////////////

static void vAppInitModules(void)
{
    /* Initialize peripherals */
    canInit();
    gioInit();
    rtiInit(); // RTI will be started after scheduler starts
    sciInit();

    /* Initialize EEPROM/Flash */
    TI_Fee_Init();
    while (TI_Fee_GetStatus(0) != IDLE)
        TI_Fee_MainFunction();

    /* Enable peripheral callbacks */
    canEnableErrorNotification(CAN_REG);
    canEnableStatusChangeNotification(CAN_REG);
    rtiEnableNotification(RTI_TELEMETRY_UPDATE);
    // sciEnableNotification(SCI_REG, SCI_TX_INT | SCI_RX_INT);

#ifdef ENABLE_APP_TEST
    vTestAppInit();
#endif
}

/**
 * @brief Initialize modules that require interrupts to be enabled
 * @note Called from vInitTask or vInitTimerCallback after scheduler starts
 */
static void vAppInitModulesWithInterrupts(void)
{
    rtiStartCounter(rtiCOUNTER_BLOCK1); // Start RTI to generate periodic events

    /* Add initializations as needed */
}

/**
 * @brief Create all application tasks
 */
static void vAppCreateTasks(AppContext_t *pxCtx)
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
}

static void vAppCreateObjects(AppContext_t *pxCtx)
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

#ifdef ENABLE_APP_TEST
    /* Timer for test app events */
    pxSync->xAppTestTimer = xTimerCreate("AppTestTimer", pdMS_TO_TICKS(100), pdTRUE, NULL, vAppTestTimerCallback);
    configASSERT(pxSync->xAppTestTimer != NULL);
    xTimerStart(pxSync->xAppTestTimer, 0);
#endif
}

// Tasks ///////////////////////////////////////////////////////////////////////////////

static portTASK_FUNCTION(vInitTask, pvParameters)
{
    (void)pvParameters;

    /* Initialize modules that require interrupts */
    vAppInitModulesWithInterrupts();

    /* Delete self */
    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vDataUpdateTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData, xLocalData;
    EventBits_t uxBits;

    __FOREVER__
    {
        /* Wait for data update event */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_DATA_UPDATE,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if ((uxBits & EVT_DATA_UPDATE) != 0U)
        {
            /* Read new frames and update local fields */
            if (uxBits & EVT_CAN_FADC1)
                canGetData(canREG1, FADC1_MAILBOX, xLocalData.xFADC1.raw);
            if (uxBits & EVT_CAN_FADC2)
                canGetData(canREG1, FADC2_MAILBOX, xLocalData.xFADC2.raw);
            if (uxBits & EVT_CAN_RADC1)
                canGetData(canREG1, RADC1_MAILBOX, xLocalData.xRADC1.raw);
            if (uxBits & EVT_CAN_RADC2)
                canGetData(canREG1, RADC2_MAILBOX, xLocalData.xRADC2.raw);
            if (uxBits & EVT_CAN_DRV1)
                canGetData(canREG1, DRV1_MAILBOX, xLocalData.xDrv1.raw);
            if (uxBits & EVT_CAN_DRV2)
                canGetData(canREG1, DRV2_MAILBOX, xLocalData.xDrv2.raw);

            /* Update global data */
            if (xSemaphoreTake(pxSync->xDataMutex, pdMS_TO_TICKS(portMAX_DELAY)) == pdTRUE)
            {
                if (uxBits & EVT_CAN_FADC1)
                    pxData->xFADC1 = xLocalData.xFADC1;
                if (uxBits & EVT_CAN_FADC2)
                    pxData->xFADC2 = xLocalData.xFADC2;
                if (uxBits & EVT_CAN_RADC1)
                    pxData->xRADC1 = xLocalData.xRADC1;
                if (uxBits & EVT_CAN_RADC2)
                    pxData->xRADC2 = xLocalData.xRADC2;
                if (uxBits & EVT_CAN_DRV1)
                    pxData->xDrv1 = xLocalData.xDrv1;
                if (uxBits & EVT_CAN_DRV2)
                    pxData->xDrv2 = xLocalData.xDrv2;

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
        uint16_t xAPPS1;
        uint16_t xAPPS2;
        uint16_t xFBrake;
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
            xLocalData.xAPPS1 = pxData->xFADC1.values.xAPPS1;
            xLocalData.xAPPS2 = pxData->xFADC1.values.xAPPS2;
            xLocalData.xFBrake = pxData->xFADC1.values.xFBrake;
            /* ... */

            xSemaphoreGive(pxSync->xDataMutex);

            /////////////////////////////////////////////
            // TODO: Placeholder for compliance checks //
            /////////////////////////////////////////////

            xEventGroupSetBits(pxSync->xSysEvents, EVT_DATA_APPROVED);

            /* TODO: Send rulebreaks via CAN */
        }
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
        uint8_t xSpeedN1;
        uint8_t xSpeedN2;
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
            xLocalData.xSpeedN1 = pxData->xDrv1.values.xSpeedN1;
            xLocalData.xSpeedN2 = pxData->xDrv1.values.xSpeedN2;
            /* ... */

            xSemaphoreGive(pxSync->xDataMutex);

            ////////////////////////////////////////////
            // TODO: Placeholder for driver FSM logic //
            ////////////////////////////////////////////
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vRFDataRxTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;
    /* TODO: Checksum length should be in xbee.h */
    uint8_t xRxBuff[MAX_PAYLOAD_SIZE + 1], xId, err = 0; // Buffer for RF data
    uint16_t xRxLen;                                     // Length of received data
    XBeeAPIStatus_t xStatus;
    XBeeAPIParsedFrame_t xParsedFrame;
    msgpack_unpacked msg;
    msgpack_unpacker unp;
    msgpack_unpack_return ret;
    msgpack_object obj;
    cobs_decode_result result;

    __FOREVER__
    {
        /* Avoid starting a new transfer before the previous one is handled */
        // uxBits = xEventGroupWaitBits(
        //     pxSync->xSysEvents,
        //     EVT_CMD_READY | EVT_TELEMETRY_READY, // Wait if either is busy
        //     pdTRUE,                              // Clear bits after return
        //     pdTRUE,                              // Wait for all bits
        //     portMAX_DELAY);

        /* Setup RF data reception */
        sciReceive(SCI_REG, 1, xRxBuff);

        /* Wait for the transfer to start */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_SCI_RX_READY,
            pdTRUE,  // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if ((uxBits & EVT_SCI_RX_READY) != 0U)
        {
            do
            {
                xRxLen = XBeeRxLen(sciReceiveByte(SCI_REG));
            } while (xRxLen == 0); // Wait until the length byte is received

            /* Receive the rest of the frame */
            sciReceive(SCI_REG, xRxLen, xRxBuff);

            /* Wait for the transfer to complete */
            uxBits = xEventGroupWaitBits(
                pxSync->xSysEvents,
                EVT_SCI_RX_READY,
                pdTRUE,  // Clear bits after return
                pdFALSE, // Wait for any bit
                portMAX_DELAY);

            /* Parse the received data */
            xStatus = XBeeParseFrame(xRxBuff, xRxLen, &xParsedFrame);

            switch (xStatus)
            {
            case XBEE_API_RX_SUCCESS:
                switch (xParsedFrame.type)
                {
                case XBEE_API_TYPE_RX_PACKET:
                    /* Only commands are received for now */
                    xId = CMD_ID;
                    if (msgpack_unpacker_init(&unp, MAX_PAYLOAD_SIZE))
                    {
                        /* Copy parsed frame data to unpacker */
                        if (msgpack_unpacker_buffer_capacity(&unp) < xParsedFrame.rxPacket.rfDataLen)
                            if (!msgpack_unpacker_reserve_buffer(&unp, xParsedFrame.rxPacket.rfDataLen))
                                err = 1; // Handle buffer reservation error

                        if (!err)
                        {
                            memcpy(msgpack_unpacker_buffer(&unp), xParsedFrame.rxPacket.rfData,
                                   xParsedFrame.rxPacket.rfDataLen);
                            msgpack_unpacker_buffer_consumed(&unp, xParsedFrame.rxPacket.rfDataLen);
                            ret = msgpack_unpacker_next(&unp, &msg);
                            switch (ret)
                            {
                            case MSGPACK_UNPACK_SUCCESS:
                                obj = msg.data;
                                result = cobs_decode(xRxBuff, sizeof(xRxBuff), obj.via.bin.ptr, obj.via.bin.size);
                                if (result.status == COBS_DECODE_OK)
                                {
                                    /* TODO: Send command to CMD task queue */
                                }
                                else
                                {
                                    /* TODO: Handle COBS decoding error */
                                }
                                break;
                            case MSGPACK_UNPACK_CONTINUE:
                                /* Check capacity, reserve buffer space, copy additional
                                 * data to the buffer, notify consumed buffer size, then
                                 * call msgpack_unpacker_next(&unp, &msg) again. */
                                break;
                            case MSGPACK_UNPACK_PARSE_ERROR:
                                /* Discard invalid data */
                                break;
                            }
                        }
                        err = 0;
                        msgpack_unpacker_destroy(&unp);
                    }
                    else
                    {
                        /* TODO: Handle unpacker initialization error */
                    }
                    break;
                case XBEE_API_TYPE_TX_STATUS:
                    xId = TELEMETRY_ACK_ID;
                default:
                    /* Discard unknown packet type */
                    break;
                }

                /* Set data ready flag for the relevant task */
                switch (xId)
                {
                case CMD_ID:              // Command identifier
                    uxBits = EVT_SCI_CMD; // | EVT_TELEMETRY_READY;
                    break;
                case TELEMETRY_ACK_ID:              // Telemetry ack identifier
                    uxBits = EVT_SCI_TELEMETRY_ACK; // | EVT_CMD_READY;
                    break;
                default:
                    break;
                }

                xEventGroupSetBits(pxSync->xSysEvents, uxBits);
                break;
            // case XBEE_API_RX_ERROR_INVALID_CHECKSUM:
            //     /* Handle checksum error */
            //     break;
            // case XBEE_API_RX_ERROR_INVALID_FRAME:
            //     /* Handle frame error */
            //     break;
            default:
                /* Discard frame */
                break;
            }
        }

        vTaskDelete(NULL);
    }
}

// static portTASK_FUNCTION(vRFDataTxTask, pvParameters)
// {
//     AppContext_t *pxCtx = (AppContext_t *)pvParameters;
//     (void)pxCtx;

//     __FOREVER__
//     {
//         /* Unlike Rx, Tx can be done inside tasks as long as proper synchronization
//          * is maintained, such as using a mutex to block sciSend from being called
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
    uint8_t xTxBuff[MAX_PAYLOAD_SIZE], xRxBuff[64], **pxFrame = NULL, *pxFrameID; // Buffers for telemetry data
    size_t xTxLen = 0, xRxLen = 0;                                                // Length of data to send/receive
    struct
    {
        DataFADC1_t xFADC1;
        DataFADC2_t xFADC2;
        /* ... */
    } xLocalData;
    XBeeAPIStatus_t xStatus;
    msgpack_sbuffer sbuf;
    msgpack_packer pk;
    cobs_encode_result result;

    __FOREVER__
    {
        /* Wait for 50Hz periodic event or telemetry ack/nack */
        uxBits = xEventGroupWaitBits(
            pxSync->xSysEvents,
            EVT_PERIODIC_TELEMETRY | EVT_SCI_TELEMETRY_ACK | EVT_TELEMETRY_NACK,
            pdFALSE, // Clear bits after return
            pdFALSE, // Wait for any bit
            portMAX_DELAY);

        if (((uxBits & EVT_PERIODIC_TELEMETRY) != 0U) || ((uxBits & EVT_TELEMETRY_NACK) != 0U))
        {
            if (((uxBits & EVT_PERIODIC_TELEMETRY) != 0U) &&
                xSemaphoreTake(pxSync->xDataMutex, portMAX_DELAY) == pdTRUE)
            {
                /* Local copy of telemetry data */
                xLocalData.xFADC1 = pxData->xFADC1;
                xLocalData.xFADC2 = pxData->xFADC2;
                /* ... */

                xSemaphoreGive(pxSync->xDataMutex);
            }

            /* Initialize msgpack buffer and packer */
            msgpack_sbuffer_init(&sbuf);
            msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

            /* Pack telemetry data into msgpack format */
            // msgpack_pack_bin_with_body(&pk, (const char *)&xLocalData, sizeof(xLocalData));
            msgpack_pack_array(&pk, 7);
            msgpack_pack_uint16(&pk, xLocalData.xFADC1.values.xAPPS1);
            msgpack_pack_uint16(&pk, xLocalData.xFADC1.values.xAPPS2);
            msgpack_pack_uint16(&pk, xLocalData.xFADC1.values.xFBrake);
            msgpack_pack_uint16(&pk, xLocalData.xFADC2.values.xFWheel1);
            msgpack_pack_uint16(&pk, xLocalData.xFADC2.values.xFWheel2);
            msgpack_pack_uint16(&pk, xLocalData.xFADC2.values.xSteering);
            /* ... */

            result = cobs_encode(xTxBuff, sizeof(xTxBuff), sbuf.data, sbuf.size);
            msgpack_sbuffer_destroy(&sbuf);
            if (result.status == COBS_ENCODE_OK)
            {
                /* Build XBee transmission request API frame */
                xStatus = XBeeBuildTxRequest((uint8_t *)&dest64, (uint8_t *)&dest16, xTxBuff, result.out_len,
                                             pxFrame, pxFrameID);
                /* TODO: Modify the XBee module to return len by reference */

                if (xStatus == XBEE_API_TX_SUCCESS)
                {
                    /* Send telemetry data to XBee via UART */
                    sciSend(SCI_REG, xTxLen, xTxBuff);

                    /* Set a timer to handle retransmission */
                    xTimerStart(pxSync->xTxTimeoutTimer, 0);
                }
                else if (xStatus == XBEE_API_TX_ERROR_FRAME_TOO_LARGE)
                {
                    /* TODO: Handle 'frame exceeds maximum payload size' error */
                }
                /* TODO: Handle other errors */
            }
            else
            {
                /* TODO: Handle encoding error */
            }
        }
        else if ((uxBits & EVT_SCI_TELEMETRY_ACK) != 0U)
        {
            /* Telemetry acknowledged, can send next packet */
            xTimerStop(pxSync->xTxTimeoutTimer, 0);
        }
    }

    vTaskDelete(NULL);
}

// Callbacks ///////////////////////////////////////////////////////////////////////////

// static void vInitTimerCallback(TimerHandle_t xTimer)
// {
//     configASSERT(xTimer != NULL);

//     /* Initialize modules that require interrupts */
//     vAppInitModulesWithInterrupts();

//     /* Delete self */
//     xTimerDelete(xTimer, 0);
// }

// static void vPeriodicTimerCallback(TimerHandle_t xTimer)
// {
//     configASSERT(xTimer != NULL);

//     /* This callback can be used to signal periodic events if RTI notifications are not
//      * available. Multiple timers with different periods can be implemented with the
//      * same callback by using their pvTimerID parameter to identify which timer expired.
//      */
// }

static void vTxTimeoutCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    xEventGroupSetBits(xAppContext.xSync->xSysEvents, EVT_TELEMETRY_NACK);
}

#ifdef ENABLE_APP_TEST
static void vAppTestTimerCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    vTestAppPeriodic();
}
#endif
