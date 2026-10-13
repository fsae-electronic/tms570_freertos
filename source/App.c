/**
 * @file App.c
 * @brief Application functions
 * @authors Agustin Fisher, Agustin Solari Raigoso, Valentin U. Vieira
 */

/* ---------------------------------------------------------------------------------- *
 * Headers                                                                            *
 * ---------------------------------------------------------------------------------- */

#if !defined(__TI_COMPILER_VERSION__) && !defined(__GNUC__) && !defined(__clang__)
#include <stdalign.h>
#endif

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
#include "esm.h"
#include "gio.h"
#include "rti.h"
#include "sci.h"
#include "ti_fee.h"

/* Application includes */
#include "data.h"
#include "can_wrappers.h"
#include "test/test_App.h"

/* External libraries */
#include "cobs/cobs.h"
#include "msgpack/cwpack.h"
#include "xbee/xbee.h"

/* ---------------------------------------------------------------------------------- *
 * Macro constants and definitions                                                    *
 * ---------------------------------------------------------------------------------- */

#ifndef ENABLE_APP_TEST
#define ENABLE_APP_TEST
#endif

#if defined(__TI_COMPILER_VERSION__) && !defined(__clang__)
/* Classic TI compiler (armcl) */
#define ALIGN_TO(symbol, bytes) _Pragma(TI_STR(DATA_ALIGN(symbol, bytes)))
#define TI_STR(x) TI_STR_EXPAND(x)
#define TI_STR_EXPAND(x) #x
#define _ALIGN_ATTRIBUTE(bytes)
#define ALIGN_ATTRIBUTE_(bytes)
#elif defined(__GNUC__) || defined(__clang__)
/* GCC and TI armclang (based on LLVM/Clang) */
#define ALIGN_TO(symbol, bytes)
#define _ALIGN_ATTRIBUTE(bytes) __attribute__((aligned(bytes)))
#define ALIGN_ATTRIBUTE_(bytes)
#else
/* Default fallback to C11 / C23 standard if the compiler is modern */
#define ALIGN_TO(symbol, bytes)
#define _ALIGN_ATTRIBUTE(bytes)
#define ALIGN_ATTRIBUTE_(bytes) alignas(bytes)
#endif

#define PRIO_MAX (configMAX_PRIORITIES - 1U)
#define TASK_STACK_WORDS(sz_bytes) ((sz_bytes) / sizeof(StackType_t))
#define __FOREVER__ for (;;)

/* Task priorities */
#define PRIO_CAN_TX (6U)      // Main ECU updates and calibration
#define PRIO_CAN_RX (6U)      // 60Hz data update from CAN
#define PRIO_RULES (5U)       // Compliance checking before actuation
#define PRIO_SCI_RX (4U)      // Receive RF data
#define PRIO_DRIVERS_FSM (3U) // Drivers state machine
#define PRIO_CMD (3U)         // Command intake affects drivers control
#define PRIO_SCI_TX (2U)      // 50Hz telemetry

/* Task stack sizes */
#define STACK_IDLE configMINIMAL_STACK_SIZE
#define STACK_TIMER configTIMER_TASK_STACK_DEPTH
#define STACK_CAN_TX TASK_STACK_WORDS(512)
#define STACK_CAN_RX TASK_STACK_WORDS(512)
#define STACK_RULES TASK_STACK_WORDS(512)
#define STACK_SCI_RX TASK_STACK_WORDS(512)
#define STACK_DRIVERS_FSM TASK_STACK_WORDS(1024)
#define STACK_CMD TASK_STACK_WORDS(512)
#define STACK_SCI_TX TASK_STACK_WORDS(1024)

#define STACK_IDLE_SZ (STACK_IDLE * sizeof(StackType_t))
#define STACK_TIMER_SZ (STACK_TIMER * sizeof(StackType_t))

// SCI event flags /////////////////////////////////////////////////////////////////////

/* RF data reception */
#define EVT_SCI_TX_READY (1 << 1)
#define EVT_SCI_RX_READY (1 << 0)
#define EVT_SCI_CMD (1 << 2)
#define EVT_SCI_TELEMETRY_ACK (1 << 3)
#define EVT_SCI_TELEMETRY_NACK (1 << 4)

/* Task ready flags */
#define EVT_SCI_CMD_READY (1 << 7)
#define EVT_SCI_TELEMETRY_READY (1 << 8)

// System event flags //////////////////////////////////////////////////////////////////

/* Periodic events */
#define EVT_RTI_PERIODIC_TELEMETRY (1 << 9)
// #define EVT_RTI_WATCHDOG_RESET (1 << 10)

/* Rule break events */
#define EVT_APPS_RULE_BREAK (1 << 11)
#define EVT_BSE_RULE_BREAK (1 << 12)
// #define EVT_BP_RULE_BREAK (1 << 13)
// #define STATE_SHUTDOWN (1 << 14) // Drivers state indicating shut down caused by an implausibility check

/* Data ready for processing */
#define EVT_CAN_NEW_DATA (1 << 15)
#define EVT_CAN_DATA_APPROVED (1 << 16)

/* ESM events */
#define EVT_WATCHDOG_NMI (1 << 17)

// CAN event flags /////////////////////////////////////////////////////////////////////

/* Mailbox updates */
#define EVT_CAN_TPS (1 << 0)
#define EVT_CAN_FRONT (1 << 1)
#define EVT_CAN_CURRENT (1 << 2)
#define EVT_CAN_REAR (1 << 3)
#define EVT_CAN_MAIN_ECU (1 << 4)
#define EVT_CAN_CALIB (1 << 5)
#define EVT_CAN_HEARTBEAT (1 << 6)
#define EVT_CAN_DRV_CTRL1 (1 << 7)
#define EVT_CAN_DRV_CTRL2 (1 << 8)
#define EVT_CAN_DRV_STATUS1 (1 << 9)
#define EVT_CAN_DRV_STATUS2 (1 << 10)
#define EVT_CAN_MOTOR1 (1 << 11)
#define EVT_CAN_MOTOR2 (1 << 12)
#define EVT_CAN_DRV1 (1 << 13)
#define EVT_CAN_DRV2 (1 << 14)
#define EVT_CAN_TX_DATA (EVT_CAN_MAIN_ECU | EVT_CAN_CALIB | EVT_CAN_DRV_CTRL1 | EVT_CAN_DRV_CTRL2)
#define EVT_CAN_RX_DATA (EVT_CAN_TPS | EVT_CAN_FRONT | EVT_CAN_CURRENT | EVT_CAN_REAR |  \
                         EVT_CAN_HEARTBEAT | EVT_CAN_DRV_STATUS1 | EVT_CAN_DRV_STATUS2 | \
                         EVT_CAN_MOTOR1 | EVT_CAN_MOTOR2 | EVT_CAN_DRV1 | EVT_CAN_DRV2)

/* Transmission complete */
#define EVT_CAN_MAIN_ECU_TX (1 << 17)
#define EVT_CAN_CALIB_TX (1 << 18)
#define EVT_CAN_DRV_CTRL1_TX (1 << 19)
#define EVT_CAN_DRV_CTRL2_TX (1 << 20)
#define EVT_CAN_TX_READY (EVT_CAN_MAIN_ECU_TX | EVT_CAN_CALIB_TX | EVT_CAN_DRV_CTRL1_TX | EVT_CAN_DRV_CTRL2_TX)

// CAN mailboxes ///////////////////////////////////////////////////////////////////////

/* ADC2CAN */
#define TPS_MAILBOX canMESSAGE_BOX1     // Rx
#define FRONT_MAILBOX canMESSAGE_BOX2   // Rx
#define CURRENT_MAILBOX canMESSAGE_BOX3 // Rx
#define REAR_MAILBOX canMESSAGE_BOX4    // Rx

/* Main ECU */
#define MAIN_ECU_MAILBOX canMESSAGE_BOX5 // Tx
#define CALIB_MAILBOX canMESSAGE_BOX6    // Tx

/* Drivers */
#define HEARTBEAT_MAILBOX canMESSAGE_BOX7    // Rx
#define DRV_CTRL1_MAILBOX canMESSAGE_BOX8    // Tx
#define DRV_CTRL2_MAILBOX canMESSAGE_BOX9    // Tx
#define DRV_STATUS1_MAILBOX canMESSAGE_BOX10 // Rx
#define DRV_STATUS2_MAILBOX canMESSAGE_BOX11 // Rx
#define MOTOR1_MAILBOX canMESSAGE_BOX12      // Rx
#define MOTOR2_MAILBOX canMESSAGE_BOX13      // Rx
#define DRV1_MAILBOX canMESSAGE_BOX14        // Rx
#define DRV2_MAILBOX canMESSAGE_BOX15        // Rx

////////////////////////////////////////////////////////////////////////////////////////

/* Peripheral registers */
#define CAN_REG canREG1
#define SCI_REG scilinREG

/* RTI is blocked by default (this FreeRTOS port uses RTI Counter 0 Compare 0 to
 * generate the system tick). In order to use it, rti.c has to be manually added to the
 * project. Modify prvSetupTimerInterrupt() in os_port.c as needed to avoid conflicts
 * with rtiInit(). Periodic events could also be generated using software timers (see
 * vPeriodicTimerCallback).
 */

/* RTI notifications */
#define RTI_TELEMETRY_UPDATE rtiNOTIFICATION_COMPARE1
// #define RTI_WATCHDOG rtiNOTIFICATION_COMPARE2

/* Command reception */
#define CMD_LENGTH (16U)

/* SCI identifiers */
#define CMD_ID (0x01U)
#define TELEMETRY_ACK_ID (0x7EU)
#define TELEMETRY_NACK_ID (0x7FU)

/* Implausibility checks */
#define RB_APPS_THRESHOLD (10U)         // Max accelerator pedal travel deviation allowed
#define RB_APPS_TIMEOUT_MS (100U)       // Time window to consider APPS implausibility valid
#define RB_BSE_TIMEOUT_MS (100U)        // Time window to consider BSE implausibility valid
#define RB_BP_BSE_THRESHOLD (90U)       // Max brake pedal travel allowed without mechanical actuation
#define RB_BP_APPS_HIGH_THRESHOLD (25U) // Max accelerator pedal travel allowed with mechanical brakes engaged
#define RB_BP_APPS_LOW_THRESHOLD (5U)   // Max accelerator pedal travel allowed to release the shut down

// #define QUEUE_LENGTH (10U)

/* ---------------------------------------------------------------------------------- *
 * Enumerations, structures and typedefs                                              *
 * ---------------------------------------------------------------------------------- */

typedef enum
{
    APP_RB_NONE,
    APP_RB_APPS,
    APP_RB_BSE,
    APP_RB_BP
} AppRuleBreak_t;

typedef enum
{
    APP_STATE_INIT,
    APP_STATE_RUNNING,
    APP_STATE_SOFT_SHUTDOWN,
    APP_STATE_HARD_SHUTDOWN
} AppState_t;

/* Application tasks */
typedef struct
{
    TaskHandle_t xCANTxTask;
    TaskHandle_t xCANRxTask;
    TaskHandle_t xRulesTask;
    TaskHandle_t xSCIRxTask;
    TaskHandle_t xDriversFSMTask;
    TaskHandle_t xCmdTask;
    TaskHandle_t xSCITxTask;
} AppTasks_t;

/* Synchronization and inter-task communication */
typedef struct
{
    /* It is not possible to wait on the bits used by the kernel itself, defined by
     * eventEVENT_BITS_CONTROL_BYTES, so each group can hold up to 23 events.
     */
    EventGroupHandle_t xSysEvents; // Event flags for system events
    EventGroupHandle_t xCANEvents; // Event flags for CAN data updates

    /* Tasks need mutex locked data for consistency, so local copies are used to avoid
     * blocking data updates and maintain protected access as short as possible.
     */
    SemaphoreHandle_t xCANDataMutex; // Mutex for CAN data

    TimerHandle_t xAPPSRuleBreakTimer; // Timer to track APPS rule breaks
    TimerHandle_t xBSERuleBreakTimer;  // Timer to track BSE rule breaks
#ifdef ENABLE_APP_TEST
    TimerHandle_t xAppTestTimer; // Timer for test app events
#endif
} AppSync_t;

/* Application resources (static allocation buffers) */
typedef struct
{
    /* Task TCBs */
    StaticTask_t xCANTxTaskBuffer;
    StaticTask_t xCANRxTaskBuffer;
    StaticTask_t xRulesTaskBuffer;
    StaticTask_t xSCIRxTaskBuffer;
    StaticTask_t xDriversFSMTaskBuffer;
    StaticTask_t xCmdTaskBuffer;
    StaticTask_t xSCITxTaskBuffer;

    /* Proper alignment for the TMS570 MPU should be ensured by defining aligned stack
     * types or using compiler attributes. It typically requires regions to be aligned
     * to their size, so stacks and local data arrays should be aligned to their total
     * byte size.
     */
    StackType_t xCANTxStack[STACK_CAN_TX];
    StackType_t xCANRxStack[STACK_CAN_RX];
    StackType_t xRulesStack[STACK_RULES];
    StackType_t xSCIRxStack[STACK_SCI_RX];
    StackType_t xDriversFSMStack[STACK_DRIVERS_FSM];
    StackType_t xCmdStack[STACK_CMD];
    StackType_t xSCITxStack[STACK_SCI_TX];

    /* RTOS objects */
    StaticEventGroup_t xSysEventsBuffer;
    StaticEventGroup_t xCANEventsBuffer;
    StaticSemaphore_t xCANDataMutexBuffer;
    StaticTimer_t xAPPSRuleBreakTimerBuffer;
    StaticTimer_t xBSERuleBreakTimerBuffer;
#ifdef ENABLE_APP_TEST
    StaticTimer_t xAppTestTimerBuffer;
#endif
} AppResources_t;

/* Data struct for local copies */
typedef struct
{
    DataTPS_t xTPS;
    DataFront_t xFront;
    DataCurrent_t xCurrent;
    DataRear_t xRear;
    DataMainECU_t xMainECU;
    DataCalib_t xCalib;
    DataHeartbeat_t xHeartbeat;
    DataDrvCtrl_t xDrvCtrl1;
    DataDrvCtrl_t xDrvCtrl2;
    DataDrvStatus_t xDrvStatus1;
    DataDrvStatus_t xDrvStatus2;
    DataMotor_t xMotor1;
    DataMotor_t xMotor2;
    DataDrv_t xDrv1;
    DataDrv_t xDrv2;
} AppCANData_t;

/* Unified app context */
typedef struct
{
    AppRuleBreak_t xRuleBreak;
    AppState_t xState;
    AppResources_t *xResources;
    AppTasks_t *xTasks;
    AppSync_t *xSync;
    AppCANData_t *xCANData;
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

/**
 * @brief Called after the scheduler starts and the timer service task executes
 */
void vApplicationDaemonTaskStartupHook(void);

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
 * - CAN Tx: transmits Main ECU data and calibrations over CAN
 * - CAN Rx: receives sensors and drivers data over CAN
 * - Rules: checks compliance with rules and sets alarms/events
 * - SCI Rx: receives and dispatches RF data
 * - Drivers State Machine: main control for drivers
 * - Command: processes and validates received commands
 * - SCI Tx: collects vehicle telemetry, packs it and transmits it over RF
 */
static portTASK_FUNCTION_PROTO(vCANTxTask, pvParameters);
static portTASK_FUNCTION_PROTO(vCANRxTask, pvParameters);
static portTASK_FUNCTION_PROTO(vRulesTask, pvParameters);
static portTASK_FUNCTION_PROTO(vSCIRxTask, pvParameters);
static portTASK_FUNCTION_PROTO(vDriversFSMTask, pvParameters);
static portTASK_FUNCTION_PROTO(vCmdTask, pvParameters);
static portTASK_FUNCTION_PROTO(vSCITxTask, pvParameters);

// Callbacks ///////////////////////////////////////////////////////////////////////////

/**
 * @brief Timer callbacks
 * @param xTimer Timer handle
 * - APPS/BSE Rule Break Timer: track APPS/BSE implausibilities
 * - App Test Timer: trigger test app events (if enabled)
 */
static void vAPPSRuleBreakTimerCallback(TimerHandle_t xTimer);
static void vBSERuleBreakTimerCallback(TimerHandle_t xTimer);
#ifdef ENABLE_APP_TEST
static void vAppTestTimerCallback(TimerHandle_t xTimer);
#endif

/* ---------------------------------------------------------------------------------- *
 * Static variables and constants with file level scope                               *
 * ---------------------------------------------------------------------------------- */

static AppResources_t xAppResources;
static AppTasks_t xAppTasks;
static AppSync_t xAppSync;
static AppCANData_t xAppCANData;
static AppContext_t xAppContext = {APP_RB_NONE,
                                   APP_STATE_INIT,
                                   &xAppResources,
                                   &xAppTasks,
                                   &xAppSync,
                                   &xAppCANData};

static const uint64_t dest64 = 0x0013A20040B2B3D4; // Example 64-bit destination address
static const uint16_t dest16 = 0xFFFE;             // Example 16-bit destination address

ALIGN_TO(uxIdleTaskStack, STACK_IDLE_SZ)
ALIGN_ATTRIBUTE_(STACK_IDLE_SZ)
static StackType_t uxIdleTaskStack[STACK_IDLE] _ALIGN_ATTRIBUTE(STACK_IDLE_SZ);
ALIGN_TO(uxTimerTaskStack, STACK_TIMER_SZ)
ALIGN_ATTRIBUTE_(STACK_TIMER_SZ)
static StackType_t uxTimerTaskStack[STACK_TIMER] _ALIGN_ATTRIBUTE(STACK_TIMER_SZ);

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
     */
    // portENABLE_INTERRUPTS();
    // vAppInitModulesWithInterrupts();
    // portDISABLE_INTERRUPTS();
    /* To initialize peripherals that require interrupts, the daemon task startup hook
     * is used to perform the necessary setup.
     */

    xAppContext.xState = APP_STATE_RUNNING;

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();
}

void vAppRun(void)
{
    /* All work done in tasks */
}

// Notifications ///////////////////////////////////////////////////////////////////////

/* _FromISR APIs (interrupt safe) and the xHigherPriorityTaskWoken parameter should be
 * used to update RTOS objects and request a context switch.
 */

void canErrorNotification(canBASE_t *node, uint32 notification)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;
    // uint32_t uLastError, uTxPending;

    /* Handle CAN error notifications */
    if (node == CAN_REG)
    {
        /* TODO: Implement CAN error handling (currently done in HALCoGen) */

        // /* Check the error level */
        // switch (notification)
        // {
        // case canLEVEL_ACTIVE: // Network state is healthy
        //     break;
        // case canLEVEL_WARNING: // Network state is degraded, continue monitoring
        // case canLEVEL_PASSIVE:
        // case canLEVEL_PARITY_ERR:
        //     /* Error handling and logging */
        //     break;
        // case canLEVEL_BUS_OFF: // Network state is catastrophic, initiate recovery sequence
        //     canHandleBusOffRecovery(node);
        //     break;
        // default: // Unknown state
        //     break;
        // }

        // /* Check the Last Error Code (LEC) */
        // uLastError = canGetLastError(node);
        // switch (uLastError)
        // {
        // case canERROR_ACKNOWLEDGE: // No node acknowledged the TX
        //     // Handle ACK failure
        //     break;
        // case canERROR_BIT1: // Sent a 1, read a 0
        // case canERROR_BIT0: // Sent a 0, read a 1
        //     // Handle physical layer / bus contention errors
        //     break;
        // default:
        //     break;
        // }

        /* Failed Tx does not trigger a notification, so ready flags must be set here */
        if (!canIsTxMessagePending(node, MAIN_ECU_MAILBOX))
        {
            xEvent |= EVT_CAN_MAIN_ECU_TX;
        }
        if (!canIsTxMessagePending(node, CALIB_MAILBOX))
        {
            xEvent |= EVT_CAN_CALIB_TX;
        }
        if (!canIsTxMessagePending(node, DRV_CTRL1_MAILBOX))
        {
            xEvent |= EVT_CAN_DRV_CTRL1_TX;
        }
        if (!canIsTxMessagePending(node, DRV_CTRL2_MAILBOX))
        {
            xEvent |= EVT_CAN_DRV_CTRL2_TX;
        }
    }

    if ((xEvent != 0U) && (xAppSync.xCANEvents != NULL))
    {
        xEventGroupSetBitsFromISR(xAppSync.xCANEvents, xEvent, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void canMessageNotification(canBASE_t *node, uint32 messageBox)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    /* Set event bits and data flags */
    if (node == CAN_REG)
    {
        switch (messageBox)
        {
        case TPS_MAILBOX:
            xEvent = EVT_CAN_TPS;
            break;
        case FRONT_MAILBOX:
            xEvent = EVT_CAN_FRONT;
            break;
        case CURRENT_MAILBOX:
            xEvent = EVT_CAN_CURRENT;
            break;
        case REAR_MAILBOX:
            xEvent = EVT_CAN_REAR;
            break;
        case HEARTBEAT_MAILBOX:
            xEvent = EVT_CAN_HEARTBEAT;
            break;
        case DRV_STATUS1_MAILBOX:
            xEvent = EVT_CAN_DRV_STATUS1;
            break;
        case DRV_STATUS2_MAILBOX:
            xEvent = EVT_CAN_DRV_STATUS2;
            break;
        case MOTOR1_MAILBOX:
            xEvent = EVT_CAN_MOTOR1;
            break;
        case MOTOR2_MAILBOX:
            xEvent = EVT_CAN_MOTOR2;
            break;
        case DRV1_MAILBOX:
            xEvent = EVT_CAN_DRV1;
            break;
        case DRV2_MAILBOX:
            xEvent = EVT_CAN_DRV2;
            break;
        case MAIN_ECU_MAILBOX:
            xEvent = EVT_CAN_MAIN_ECU_TX;
            break;
        case CALIB_MAILBOX:
            xEvent = EVT_CAN_CALIB_TX;
            break;
        default:
            break;
        }
    }

    if ((xEvent != 0U) && (xAppSync.xCANEvents != NULL))
    {
        xEventGroupSetBitsFromISR(xAppSync.xCANEvents, xEvent, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void canStatusChangeNotification(canBASE_t *node, uint32 notification)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    if (node == CAN_REG)
    {
        // /* Handle status change notifications */
        // if ((notification & canLEVEL_RxOK) != 0U)
        // {
        //     xEvent = EVT_CAN_NEW_DATA; // New data received
        // }
        // else if ((notification & canLEVEL_TxOK) != 0U)
        // {
        //     xEvent = EVT_CAN_TX_READY; // Transmission completed
        // }
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != NULL))
    {
        xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void esmGroup2Notification(uint32 channel)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    /* TODO: Handle system reset if watchdog is not serviced */
    switch (channel)
    {
    // case esmCHANNEL24:
    //     xEvent = EVT_WATCHDOG_NMI;
    //     break;
    default:
        break;
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != NULL))
    {
        xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void rtiNotification(uint32 notification)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    /* Signal periodic events */
    switch (notification)
    {
    case RTI_TELEMETRY_UPDATE: // 50Hz
        xEvent = EVT_RTI_PERIODIC_TELEMETRY;
        break;
    // case RTI_WATCHDOG: // 10Hz
    //     xEvent = EVT_RTI_WATCHDOG;
    //     break;
    default:
        break;
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != NULL))
    {
        xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void sciNotification(sciBASE_t *sci, uint32 flags)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = 0U;

    if (sci == SCI_REG)
    {
        if ((flags & SCI_RX_INT) != 0U)
        {
            xEvent = EVT_SCI_RX_READY;
        }
        else if ((flags & SCI_TX_INT) != 0U)
        {
            xEvent = EVT_SCI_TX_READY;
        }
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != NULL))
    {
        xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// Static Internal Kernel Memory Allocation ////////////////////////////////////////////

/* When the static memory allocator is enabled (configSUPPORT_STATIC_ALLOCATION = 1),
 * the idle task and the timer task (configUSE_TIMERS = 1) will use static memory
 * supplied by user functions. These callbacks must always be manually provided on MPU
 * ports so that the stack alignment required by the target architecture is maintained.
 * The kernel will call vApplicationGet*TaskMemory() to allow the application to create
 * and return memory buffers for the TCB and stack. The idle task is run when a core
 * runs out of scheduled work. It performs some housekeeping and can also trigger the
 * user's vTaskIdleHook() if it is enabled.
 */

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationDaemonTaskStartupHook(void)
{
    vAppInitModulesWithInterrupts();
}

/* ---------------------------------------------------------------------------------- *
 * Local function definitions                                                         *
 * ---------------------------------------------------------------------------------- */

// Initialization //////////////////////////////////////////////////////////////////////

static void vAppInitModules(void)
{
    /* Initialize peripherals */
    canInit();
    // dwwdInit(Generate_NMI, 4095, Size_100_Percent);
    gioInit();
    rtiInit(); // RTI will be started after scheduler starts
    sciInit();

    /* Enable peripheral callbacks */
    canEnableErrorNotification(CAN_REG);
    canEnableStatusChangeNotification(CAN_REG);
    rtiEnableNotification(RTI_TELEMETRY_UPDATE);
    // sciEnableNotification(SCI_REG, SCI_TX_INT | SCI_RX_INT);

#ifdef ENABLE_APP_TEST
    vTestAppInit();
#endif
}

static void vAppInitModulesWithInterrupts(void)
{
    // rtiStartCounter(rtiCOUNTER_BLOCK1); // Start RTI to generate periodic events
    // dwdCounterEnable(); // Start DWWD to monitor system health

    /* Add initializations as needed */
}

static void vAppCreateTasks(AppContext_t *pxCtx)
{
    AppResources_t *pxRes = pxCtx->xResources;
    AppTasks_t *pxTasks = pxCtx->xTasks;

    configASSERT((pxRes != NULL) && (pxTasks != NULL));

    pxTasks->xCANTxTask = xTaskCreateStatic(vCANTxTask,
                                            "CANTx",
                                            STACK_CAN_TX,
                                            (void *)pxCtx,
                                            PRIO_CAN_TX | portPRIVILEGE_BIT,
                                            pxRes->xCANTxStack,
                                            &pxRes->xCANTxTaskBuffer);
    configASSERT(pxTasks->xCANTxTask != NULL);

    pxTasks->xCANRxTask = xTaskCreateStatic(vCANRxTask,
                                            "CANRx",
                                            STACK_CAN_RX,
                                            (void *)pxCtx,
                                            PRIO_CAN_RX | portPRIVILEGE_BIT,
                                            pxRes->xCANRxStack,
                                            &pxRes->xCANRxTaskBuffer);
    configASSERT(pxTasks->xCANRxTask != NULL);

    pxTasks->xRulesTask = xTaskCreateStatic(vRulesTask,
                                            "Rules",
                                            STACK_RULES,
                                            (void *)pxCtx,
                                            PRIO_RULES | portPRIVILEGE_BIT,
                                            pxRes->xRulesStack,
                                            &pxRes->xRulesTaskBuffer);
    configASSERT(pxTasks->xRulesTask != NULL);

    pxTasks->xSCIRxTask = xTaskCreateStatic(vSCIRxTask,
                                            "SCIRx",
                                            STACK_SCI_RX,
                                            (void *)pxCtx,
                                            PRIO_SCI_RX | portPRIVILEGE_BIT,
                                            pxRes->xSCIRxStack,
                                            &pxRes->xSCIRxTaskBuffer);
    configASSERT(pxTasks->xSCIRxTask != NULL);

    pxTasks->xDriversFSMTask = xTaskCreateStatic(vDriversFSMTask,
                                                 "DriversFSM",
                                                 STACK_DRIVERS_FSM,
                                                 (void *)pxCtx,
                                                 PRIO_DRIVERS_FSM | portPRIVILEGE_BIT,
                                                 pxRes->xDriversFSMStack,
                                                 &pxRes->xDriversFSMTaskBuffer);
    configASSERT(pxTasks->xDriversFSMTask != NULL);

    pxTasks->xCmdTask = xTaskCreateStatic(vCmdTask,
                                          "Cmd",
                                          STACK_CMD,
                                          (void *)pxCtx,
                                          PRIO_CMD | portPRIVILEGE_BIT,
                                          pxRes->xCmdStack,
                                          &pxRes->xCmdTaskBuffer);
    configASSERT(pxTasks->xCmdTask != NULL);

    pxTasks->xSCITxTask = xTaskCreateStatic(vSCITxTask,
                                            "SCITx",
                                            STACK_SCI_TX,
                                            (void *)pxCtx,
                                            PRIO_SCI_TX | portPRIVILEGE_BIT,
                                            pxRes->xSCITxStack,
                                            &pxRes->xSCITxTaskBuffer);
    configASSERT(pxTasks->xSCITxTask != NULL);
}

static void vAppCreateObjects(AppContext_t *pxCtx)
{
    AppResources_t *pxRes = pxCtx->xResources;
    AppSync_t *pxSync = pxCtx->xSync;

    configASSERT((pxRes != NULL) && (pxSync != NULL));

    /* Create event group for synchronization flags */
    pxSync->xSysEvents = xEventGroupCreateStatic(&pxRes->xSysEventsBuffer);
    configASSERT(pxSync->xSysEvents != NULL);

    /* Create event group for data update flags */
    pxSync->xCANEvents = xEventGroupCreateStatic(&pxRes->xCANEventsBuffer);
    configASSERT(pxSync->xCANEvents != NULL);

    /* Create mutex for CAN data */
    pxSync->xCANDataMutex = xSemaphoreCreateMutexStatic(&pxRes->xCANDataMutexBuffer);
    configASSERT(pxSync->xCANDataMutex != NULL);

    /* Timer to track APPS rule breaks */
    pxSync->xAPPSRuleBreakTimer = xTimerCreate("APPSRuleBreakTimer",
                                               pdMS_TO_TICKS(RB_APPS_TIMEOUT_MS),
                                               pdFALSE,
                                               NULL,
                                               vAPPSRuleBreakTimerCallback);
    configASSERT(pxSync->xAPPSRuleBreakTimer != NULL);

    /* Timer to track BSE rule breaks */
    pxSync->xBSERuleBreakTimer = xTimerCreate("BSERuleBreakTimer",
                                              pdMS_TO_TICKS(RB_BSE_TIMEOUT_MS),
                                              pdFALSE,
                                              NULL,
                                              vBSERuleBreakTimerCallback);
    configASSERT(pxSync->xBSERuleBreakTimer != NULL);

#ifdef ENABLE_APP_TEST
    /* Timer for test app events */
    pxSync->xAppTestTimer = xTimerCreate("AppTestTimer",
                                         pdMS_TO_TICKS(100),
                                         pdTRUE,
                                         NULL,
                                         vAppTestTimerCallback);
    configASSERT(pxSync->xAppTestTimer != NULL);
    xTimerStart(pxSync->xAppTestTimer, 0);
#endif
}

// Tasks ///////////////////////////////////////////////////////////////////////////////

static portTASK_FUNCTION(vCANTxTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppCANData_t *pxCANData = pxCtx->xCANData, xLocalData;
    EventBits_t uxBits;
    uint8_t raw[8]; // Temporary buffer for CAN data

    __FOREVER__
    {
        /* Wait for data update events */
        uxBits = xEventGroupWaitBits(pxSync->xCANEvents,
                                     EVT_CAN_TX_DATA,
                                     pdFALSE, // Do not clear bits after return
                                     pdFALSE, // Wait for any bit
                                     portMAX_DELAY);

        if ((uxBits & EVT_CAN_TX_DATA) != 0U)
        {
            if (xSemaphoreTake(pxSync->xCANDataMutex, pdMS_TO_TICKS(portMAX_DELAY)) == pdTRUE)
            {
                /* Copy the data to be transmitted */
                if (uxBits & EVT_CAN_MAIN_ECU)
                {
                    xLocalData.xMainECU = pxCANData->xMainECU;
                    uxBits |= EVT_CAN_MAIN_ECU_TX;
                }
                if (uxBits & EVT_CAN_CALIB)
                {
                    xLocalData.xCalib = pxCANData->xCalib;
                    uxBits |= EVT_CAN_CALIB_TX;
                }
                if (uxBits & EVT_CAN_DRV_CTRL1)
                {
                    xLocalData.xDrvCtrl1 = pxCANData->xDrvCtrl1;
                    uxBits |= EVT_CAN_DRV_CTRL1_TX;
                }
                if (uxBits & EVT_CAN_DRV_CTRL2)
                {
                    xLocalData.xDrvCtrl2 = pxCANData->xDrvCtrl2;
                    uxBits |= EVT_CAN_DRV_CTRL2_TX;
                }

                /* Clear the local data */
                // memset(&pxCANData->xMainECU, 0, sizeof(pxCANData->xMainECU));

                if (xSemaphoreGive(pxSync->xCANDataMutex) == pdTRUE)
                {
                    xEventGroupClearBits(pxSync->xCANEvents, EVT_CAN_TX_DATA);
                    uxBits &= EVT_CAN_TX_READY;
                }
                else
                {
                    /* We would not expect this call to fail because we must have
                     * obtained the semaphore to get here.
                     */
                }
            }

            uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
                                         uxBits,
                                         pdTRUE, // Clear bits after return
                                         pdTRUE, // Wait for all bits
                                         portMAX_DELAY);

            /* Process and transmit the local data */
            if (uxBits & EVT_CAN_MAIN_ECU_TX)
            {
                dataPack(raw, &xLocalData.xMainECU, DATA_ID_MAIN_ECU);
                canTransmit(canREG1, MAIN_ECU_MAILBOX, raw);
            }
            if (uxBits & EVT_CAN_CALIB_TX)
            {
                dataPack(raw, &xLocalData.xCalib, DATA_ID_CALIB);
                canTransmit(canREG1, CALIB_MAILBOX, raw);
            }
            if (uxBits & EVT_CAN_DRV_CTRL1_TX)
            {
                dataPack(raw, &xLocalData.xDrvCtrl1, DATA_ID_DRV_CTRL1);
                canTransmit(canREG1, DRV_CTRL1_MAILBOX, raw);
            }
            if (uxBits & EVT_CAN_DRV_CTRL2_TX)
            {
                dataPack(raw, &xLocalData.xDrvCtrl2, DATA_ID_DRV_CTRL2);
                // if (canIsTxMessagePending(canREG1, DRV_CTRL2_MAILBOX))
                // {
                //     canAbortTransmission(canREG1, DRV_CTRL2_MAILBOX);
                // }
                canTransmit(canREG1, DRV_CTRL2_MAILBOX, raw);
            }
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vCANRxTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppCANData_t *pxCANData = pxCtx->xCANData, xLocalData;
    EventBits_t uxBits;
    uint8_t raw[8]; // Temporary buffer for CAN data

    __FOREVER__
    {
        /* Wait for data update events */
        uxBits = xEventGroupWaitBits(pxSync->xCANEvents,
                                     EVT_CAN_RX_DATA,
                                     pdFALSE, // Do not clear bits after return
                                     pdFALSE, // Wait for any bit
                                     portMAX_DELAY);

        if ((uxBits & EVT_CAN_RX_DATA) != 0U)
        {
            /* Read new frames and update local fields */
            if (uxBits & EVT_CAN_TPS)
            {
                canGetData(canREG1, TPS_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xTPS, DATA_ID_TPS);
            }
            if (uxBits & EVT_CAN_FRONT)
            {
                canGetData(canREG1, FRONT_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xFront, DATA_ID_FRONT);
            }
            if (uxBits & EVT_CAN_CURRENT)
            {
                canGetData(canREG1, CURRENT_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xCurrent, DATA_ID_CURRENT);
            }
            if (uxBits & EVT_CAN_REAR)
            {
                canGetData(canREG1, REAR_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xRear, DATA_ID_REAR);
            }
            if (uxBits & EVT_CAN_HEARTBEAT)
            {
                canGetData(canREG1, HEARTBEAT_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xHeartbeat, DATA_ID_HEARTBEAT);
            }
            if (uxBits & EVT_CAN_DRV_STATUS1)
            {
                canGetData(canREG1, DRV_STATUS1_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xDrvStatus1, DATA_ID_DRV_STATUS1);
            }
            if (uxBits & EVT_CAN_DRV_STATUS2)
            {
                canGetData(canREG1, DRV_STATUS2_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xDrvStatus2, DATA_ID_DRV_STATUS2);
            }
            if (uxBits & EVT_CAN_MOTOR1)
            {
                canGetData(canREG1, MOTOR1_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xMotor1, DATA_ID_MOTOR1);
            }
            if (uxBits & EVT_CAN_MOTOR2)
            {
                canGetData(canREG1, MOTOR2_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xMotor2, DATA_ID_MOTOR2);
            }
            if (uxBits & EVT_CAN_DRV1)
            {
                canGetData(canREG1, DRV1_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xDrv1, DATA_ID_DRV1);
            }
            if (uxBits & EVT_CAN_DRV2)
            {
                canGetData(canREG1, DRV2_MAILBOX, raw);
                dataUnpack(raw, &xLocalData.xDrv2, DATA_ID_DRV2);
            }
            /* TODO: Handle Rx errors and lost messages */

            /* Update global data */
            if (xSemaphoreTake(pxSync->xCANDataMutex, pdMS_TO_TICKS(portMAX_DELAY)) == pdTRUE)
            {
                (uxBits & EVT_CAN_TPS) ? pxCANData->xTPS = xLocalData.xTPS : (void)0;
                (uxBits & EVT_CAN_FRONT) ? pxCANData->xFront = xLocalData.xFront : (void)0;
                (uxBits & EVT_CAN_CURRENT) ? pxCANData->xCurrent = xLocalData.xCurrent : (void)0;
                (uxBits & EVT_CAN_REAR) ? pxCANData->xRear = xLocalData.xRear : (void)0;
                (uxBits & EVT_CAN_HEARTBEAT) ? pxCANData->xHeartbeat = xLocalData.xHeartbeat : (void)0;
                (uxBits & EVT_CAN_DRV_STATUS1) ? pxCANData->xDrvStatus1 = xLocalData.xDrvStatus1 : (void)0;
                (uxBits & EVT_CAN_DRV_STATUS2) ? pxCANData->xDrvStatus2 = xLocalData.xDrvStatus2 : (void)0;
                (uxBits & EVT_CAN_MOTOR1) ? pxCANData->xMotor1 = xLocalData.xMotor1 : (void)0;
                (uxBits & EVT_CAN_MOTOR2) ? pxCANData->xMotor2 = xLocalData.xMotor2 : (void)0;
                (uxBits & EVT_CAN_DRV1) ? pxCANData->xDrv1 = xLocalData.xDrv1 : (void)0;
                (uxBits & EVT_CAN_DRV2) ? pxCANData->xDrv2 = xLocalData.xDrv2 : (void)0;

                if (xSemaphoreGive(pxSync->xCANDataMutex) == pdTRUE)
                {
                    xEventGroupClearBits(pxSync->xCANEvents, EVT_CAN_RX_DATA);
                    xEventGroupSetBits(pxSync->xSysEvents, EVT_CAN_NEW_DATA);
                }
                else
                {
                    /* We would not expect this call to fail because we must have
                     * obtained the semaphore to get here.
                     */
                }
            }
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vRulesTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppRuleBreak_t *pxRuleBreak = &pxCtx->xRuleBreak;
    AppState_t *pxState = &pxCtx->xState;
    AppSync_t *pxSync = pxCtx->xSync;
    AppCANData_t *pxCANData = pxCtx->xCANData;
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
        uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
                                     EVT_CAN_NEW_DATA | EVT_APPS_RULE_BREAK | EVT_BSE_RULE_BREAK,
                                     pdTRUE,  // Clear bits after return
                                     pdFALSE, // Wait for any bit
                                     portMAX_DELAY);

        /* TODO: Check implausibilities */
        if ((uxBits & EVT_APPS_RULE_BREAK) != 0U)
        {
            *pxRuleBreak = APP_RB_APPS;
            *pxState = APP_STATE_HARD_SHUTDOWN;
        }
        else if ((uxBits & EVT_BSE_RULE_BREAK) != 0U)
        {
            *pxRuleBreak = APP_RB_BSE;
            *pxState = APP_STATE_HARD_SHUTDOWN;
        }
        else if (((uxBits & EVT_CAN_NEW_DATA) != 0U) && (*pxState != APP_STATE_HARD_SHUTDOWN))
        {
            /* Local copy of the fields that need rule checking */
            if (xSemaphoreTake(pxSync->xCANDataMutex, portMAX_DELAY) == pdTRUE)
            {
                xLocalData.xAPPS1 = pxCANData->xTPS.xTPS1;
                xLocalData.xAPPS2 = pxCANData->xTPS.xTPS2;
                xLocalData.xFBrake = pxCANData->xFront.xFBrake;
                /* ... */

                xSemaphoreGive(pxSync->xCANDataMutex);

                /* Check APPS implausibility */
                if ((xLocalData.xAPPS1 - xLocalData.xAPPS2) > RB_APPS_THRESHOLD)
                {
                    /* Start APPS rule break timer */
                    xTimerStart(pxSync->xAPPSRuleBreakTimer, portMAX_DELAY);
                }
                else
                {
                    /* Stop APPS rule break timer */
                    xTimerStop(pxSync->xAPPSRuleBreakTimer, portMAX_DELAY);
                }

                // /* TODO: Check BSE implausibility */
                // if (/* ... */)
                // {
                //     /* Start BSE rule break timer */
                //     xTimerStart(pxSync->xBSERuleBreakTimer, portMAX_DELAY);
                // }
                // else
                // {
                //     /* Stop BSE rule break timer if running */
                //     xTimerStop(pxSync->xBSERuleBreakTimer, portMAX_DELAY);
                // }

                /* Check BP implausibility */
                if ((*pxState != APP_STATE_SOFT_SHUTDOWN) &&
                    (xLocalData.xFBrake > RB_BP_BSE_THRESHOLD) &&
                    ((xLocalData.xAPPS1 > RB_BP_APPS_HIGH_THRESHOLD) ||
                     (xLocalData.xAPPS2 > RB_BP_APPS_HIGH_THRESHOLD)))
                {
                    // xEventGroupSetBits(pxSync->xSysEvents, EVT_BP_RULE_BREAK);
                    *pxRuleBreak = APP_RB_BP;
                    *pxState = APP_STATE_SOFT_SHUTDOWN;
                }
                else if ((*pxState == APP_STATE_SOFT_SHUTDOWN) &&
                         (xLocalData.xAPPS1 < RB_BP_APPS_LOW_THRESHOLD) &&
                         (xLocalData.xAPPS2 < RB_BP_APPS_LOW_THRESHOLD))
                {
                    /* Clear rule break and return to running state */
                    *pxRuleBreak = APP_RB_NONE;
                    *pxState = APP_STATE_RUNNING;
                }

                if (*pxRuleBreak == APP_RB_NONE)
                {
                    xEventGroupSetBits(pxSync->xSysEvents, EVT_CAN_DATA_APPROVED);
                }
            }

            pxCANData->xMainECU.xTPS = xLocalData.xAPPS1;
            pxCANData->xMainECU.xMode = 0;
            pxCANData->xMainECU.xError = *pxRuleBreak;
            xEventGroupSetBits(pxSync->xCANEvents, EVT_CAN_MAIN_ECU);
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vDriversFSMTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppCANData_t *pxCANData = pxCtx->xCANData;
    EventBits_t uxBits;
    struct
    {
        uint16_t xMotorVelocity1;
        uint16_t xMotorVelocity2;
        uint8_t xMotorTemp1;
        uint8_t xMotorTemp2;
        /* ... */
    } xLocalData;

    __FOREVER__
    {
        /* Wait for new approved data */
        uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
                                     EVT_CAN_DATA_APPROVED,
                                     pdFALSE, // Do not clear bits after return
                                     pdFALSE, // Wait for any bit
                                     portMAX_DELAY);

        if ((uxBits & EVT_CAN_DATA_APPROVED) != 0U)
        {
            /* Local copy of control-related state */
            if (xSemaphoreTake(pxSync->xCANDataMutex, portMAX_DELAY) == pdTRUE)
            {
                xLocalData.xMotorVelocity1 = pxCANData->xMotor1.xMotorVel;
                xLocalData.xMotorVelocity2 = pxCANData->xMotor2.xMotorVel;
                xLocalData.xMotorTemp1 = pxCANData->xDrv1.xDrvTemp;
                xLocalData.xMotorTemp2 = pxCANData->xDrv2.xDrvTemp;
                /* ... */

                xSemaphoreGive(pxSync->xCANDataMutex);
                xEventGroupClearBits(pxSync->xSysEvents, EVT_CAN_DATA_APPROVED);

                ////////////////////////////////////////////
                // TODO: Placeholder for driver FSM logic //
                ////////////////////////////////////////////
            }
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vSCIRxTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;
    uint8_t xRxBuff[XBEE_API_MAX_FRAME_SZ]; // Buffers for RF data
    uint8_t xId;
    // uint16_t xRxLen; // Length of received data
    // XBeeAPIStatus_t xStatus = XBEE_API_RX_ERROR_NO_DATA;
    XBeeAPIStatus_t xStatus;
    XBeeAPIFrame_t xRxFrame;
    cw_unpack_context uc;
    cobs_decode_result result;
    // bool xSetupRx = true;

    /* Setup RF data reception */
    sciReceive(SCI_REG, 1, xRxFrame.buffer);

    __FOREVER__
    {
        /* Wait for the transfer to start */
        uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
                                     EVT_SCI_RX_READY,
                                     pdTRUE,  // Clear bits after return
                                     pdFALSE, // Wait for any bit
                                     portMAX_DELAY);

        if ((uxBits & EVT_SCI_RX_READY) != 0U)
        {
            /* Parse incoming data until the length is known */
            xStatus = XBeeAPIParseFrame(&xRxFrame);

            if ((xStatus == XBEE_API_RX_SUCCESS) && (xRxFrame.length != 0))
            {
                /* Receive the rest of the data */
                sciReceive(SCI_REG, xRxFrame.length, xRxFrame.buffer);
                // xSetupRx = true; // Setup reception for the next frame

                /* Wait for the transfer to complete */
                uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
                                             EVT_SCI_RX_READY,
                                             pdTRUE,  // Clear bits after return
                                             pdFALSE, // Wait for any bit
                                             portMAX_DELAY);

                /* Parse the complete frame */
                xStatus = XBeeAPIParseFrame(&xRxFrame);

                switch (xStatus)
                {
                case XBEE_API_RX_SUCCESS:
                    switch (xRxFrame.type)
                    {
                    case XBEE_API_TYPE_RX_PACKET:
                        /* Only commands are received for now */
                        xId = CMD_ID;

                        /* Decode the received data using COBS, excluding the 0x00 delimiter */
                        result = cobs_decode(xRxBuff, sizeof(xRxBuff),
                                             xRxFrame.data.rxPacket.rxData, xRxFrame.data.rxPacket.rxDataLen - 1);

                        if (result.status == COBS_DECODE_OK)
                        {
                            if (cw_unpack_context_init(&uc, xRxBuff, result.out_len, NULL) == CWP_RC_OK)
                            {
                                while (uc.return_code != CWP_RC_END_OF_INPUT)
                                {
                                    /* Unpack parsed frame data */
                                    cw_unpack_next(&uc);
                                    switch (uc.return_code)
                                    {
                                    case CWP_RC_OK:
                                        switch (uc.item.type)
                                        {
                                        case CWP_ITEM_BIN:
                                            /* Process binary data */
                                            /* TODO: Send command to CMD task queue */
                                            break;
                                        default:
                                            /* Discard unknown parsed type */
                                            break;
                                        }
                                        break;
                                    default: // Discard invalid data
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                /* TODO: Handle unpacker initialization error */
                            }
                        }
                        else
                        {
                            /* TODO: Handle COBS decoding error */
                        }
                        break;
                    case XBEE_API_TYPE_TX_STATUS:
                        if (xRxFrame.data.txStatus.deliveryStatus == XBEE_API_TX_STATUS_SUCCESS)
                        {
                            xId = TELEMETRY_ACK_ID;
                        }
                        else
                        {
                            xId = TELEMETRY_NACK_ID;
                        }
                    default: // Discard unknown packet type
                        break;
                    }

                    /* Set data ready flag for the relevant task */
                    switch (xId)
                    {
                    case CMD_ID:              // Command identifier
                        uxBits = EVT_SCI_CMD; // | EVT_SCI_TELEMETRY_READY;
                        break;
                    case TELEMETRY_ACK_ID:              // Telemetry ack identifier
                        uxBits = EVT_SCI_TELEMETRY_ACK; // | EVT_SCI_CMD_READY;
                        break;
                    case TELEMETRY_NACK_ID:              // Telemetry nack identifier
                        uxBits = EVT_SCI_TELEMETRY_NACK; // | EVT_SCI_CMD_READY
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
                default: // Discard frame
                    break;
                }

                // xStatus = XBEE_API_RX_ERROR_NO_DATA; // Reset status
                /* Setup reception for the next frame */
                sciReceive(SCI_REG, 1, xRxFrame.buffer);
            }
        }
    }

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vCmdTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;
    uint8_t xRxBuff[CMD_LENGTH]; // Buffer for Cmd data

    __FOREVER__
    {
        /* Wait for the transfer to complete */
        uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
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

static portTASK_FUNCTION(vSCITxTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppCANData_t *pxCANData = pxCtx->xCANData;
    EventBits_t uxBits;
    // XBeeAPIStatus_t xStatus = XBEE_API_RX_ERROR_NO_DATA;
    XBeeAPIStatus_t xStatus;
    XBeeAPIFrame_t xTxFrame;
    uint8_t uTxBuff[XBEE_API_MAX_FRAME_SZ]; // Buffers for telemetry data
    // size_t xTxLen = 0; // Length of data to send
    double xLocalData[7];
    // uint8_t *uBuffPtr = &xFrame.buffer[XBEE_API_FRAME_HEAD_SZ + XBEE_API_DATA_HEAD_SZ_TX_REQUEST];          // Pointer to the payload position
    // uint8_t uBuffLen = sizeof(xFrame.buffer) - (XBEE_API_FRAME_HEAD_SZ + XBEE_API_DATA_HEAD_SZ_TX_REQUEST); // Length of the payload buffer
    uint8_t i = 0, uPayloadPos = XBEE_API_FRAME_HEAD_SZ + XBEE_API_DATA_HEAD_SZ_TX_REQUEST; // Position of the payload in the frame buffer
    cw_pack_context pc;
    cobs_encode_result result;
    const char keyBuff[][4] = {"s_00", "s_01", "s_02", "s_03", "s_04", "s_05", "s_06"}; // Buffer for msgpack keys

    __FOREVER__
    {
        /* Wait for 50Hz periodic event or telemetry ack/nack */
        uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
                                     EVT_RTI_PERIODIC_TELEMETRY | EVT_SCI_TELEMETRY_ACK | EVT_SCI_TELEMETRY_NACK,
                                     pdFALSE, // Clear bits after return
                                     pdFALSE, // Wait for any bit
                                     portMAX_DELAY);

        if ((uxBits & EVT_RTI_PERIODIC_TELEMETRY) != 0U)
        {
            if (xSemaphoreTake(pxSync->xCANDataMutex, portMAX_DELAY) == pdTRUE)
            {
                /* Local copy of telemetry data */
                xLocalData[0] = (double)pxCANData->xTPS.xTPS1;
                xLocalData[1] = (double)pxCANData->xTPS.xTPS2;
                xLocalData[2] = (double)pxCANData->xFront.xFLSpeed;
                xLocalData[3] = (double)pxCANData->xFront.xFRSpeed;
                xLocalData[4] = (double)pxCANData->xFront.xDirAngle;
                xLocalData[5] = (double)pxCANData->xFront.xFBrake;
                xLocalData[6] = (double)pxCANData->xRear.xRBrake;
                /* ... */

                xSemaphoreGive(pxSync->xCANDataMutex);
            }

            /* Initialize msgpack buffer and packer */
            cw_pack_context_init(&pc, uTxBuff, sizeof(uTxBuff), NULL);

            /* Pack telemetry data into msgpack format */
            cw_pack_map_size(&pc, 7);
            for (; i < 7; ++i)
            {
                cw_pack_str(&pc, keyBuff[i], 4);
                cw_pack_double(&pc, xLocalData[i]);
            }

            if (pc.return_code == CWP_RC_OK)
            {
                /* Encode the msgpack data using COBS */
                result = cobs_encode(&xTxFrame.buffer[uPayloadPos], sizeof(xTxFrame.buffer) - (uPayloadPos + 1), // Leave space for delimiter
                                     uTxBuff, pc.end - pc.current);

                if (result.status == COBS_ENCODE_OK)
                {
                    /* Append delimiter byte */
                    xTxFrame.buffer[result.out_len] = 0x00;

                    /* Build XBee transmission request API frame */
                    xTxFrame.type = XBEE_API_TYPE_TX_REQUEST;
                    xTxFrame.data.txRequest.frameID = 0x00; // No response required
                    xTxFrame.data.txRequest.dest64 = dest64;
                    xTxFrame.data.txRequest.dest16 = dest16;
                    xTxFrame.data.txRequest.broadcastRadius = 0x00; // Maximum hops
                    xTxFrame.data.txRequest.txOptions = 0x00;       // Disable ACK and retries
                    xTxFrame.data.txRequest.payloadData = &xTxFrame.buffer[uPayloadPos];
                    xTxFrame.data.txRequest.payloadDataLen = result.out_len + 1;
                    xStatus = XBeeAPIBuildFrame(&xTxFrame);

                    if (xStatus == XBEE_API_TX_SUCCESS)
                    {
                        /* Send telemetry data */
                        xEventGroupSetBits(pxSync->xSysEvents, EVT_SCI_TELEMETRY_READY);
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
            else
            {
                /* TODO: Handle msgpack packing error */
            }
        }
        else if ((uxBits & EVT_SCI_TELEMETRY_ACK) != 0U)
        {
            /* TODO: Clear pending Tx using frameID */
        }
        else if ((uxBits & EVT_SCI_TELEMETRY_NACK) != 0U)
        {
            /* If previous transmission failed, retry with the same data */
            xEventGroupSetBits(pxSync->xSysEvents, EVT_SCI_TELEMETRY_READY);

            /* TODO: Retransmit with frameID, and remove if max retries exceeded */
        }

        uxBits = xEventGroupGetBits(pxSync->xSysEvents);

        if ((uxBits & EVT_SCI_TELEMETRY_READY) != 0U)
        {
            xEventGroupClearBits(pxSync->xSysEvents, EVT_SCI_TELEMETRY_READY);

            uxBits = xEventGroupWaitBits(pxSync->xSysEvents,
                                         EVT_SCI_TX_READY,
                                         pdTRUE,  // Clear bits after return
                                         pdFALSE, // Wait for any bit
                                         portMAX_DELAY);

            if ((uxBits & EVT_SCI_TX_READY) != 0U)
            {
                sciSend(SCI_REG, xTxFrame.length, xTxFrame.buffer);
            }
        }
    }

    vTaskDelete(NULL);
}

// Callbacks ///////////////////////////////////////////////////////////////////////////

static void vAPPSRuleBreakTimerCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    xEventGroupSetBits(xAppContext.xSync->xSysEvents, EVT_APPS_RULE_BREAK);
}

static void vBSERuleBreakTimerCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    xEventGroupSetBits(xAppContext.xSync->xSysEvents, EVT_BSE_RULE_BREAK);
}

#ifdef ENABLE_APP_TEST
static void vAppTestTimerCallback(TimerHandle_t xTimer)
{
    configASSERT(xTimer != NULL);

    vTestAppPeriodic();
}
#endif
