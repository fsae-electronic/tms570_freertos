/**
 * @file App.cpp
 * @brief Application functions
 */

/* ---------------------------------------------------------------------------------- *
 * Headers                                                                            *
 * ---------------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"
{
#endif

// Standard library includes
// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>

/* TMS570 supports MPU, so the app will call MPU_xxx wrappers transparently (because
 * os_mpu_wrappers.h does the remapping). MPU_WRAPPERS_INCLUDED_FROM_API_FILE should not
 * be used here because defining it prevents os_task.h from redefining all the API
 * functions to use the MPU wrappers.
 */

// FreeRTOS includes
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_timer.h"
#include "os_queue.h"
#include "os_semphr.h"
#include "os_event_groups.h"

// Peripheral includes
#include "can.h"
#include "gio.h"
#include "rti.h"
#include "sci.h"
#include "spi.h"
#include "ti_fee.h"
#ifdef __cplusplus
}
#endif

// Application includes
#include "Bridgetek_EVE2.hpp"
#include "dashboard.hpp"
#include "data.hpp"
#include "ui_touch.hpp"
#include "test_main_ecu.hpp"

/* RTI is blocked by default (this FreeRTOS port uses RTI Counter 0 Compare 0 to
 * generate the system tick). In order to use it, rti.c has to be created and modified
 * directly so that there is no conflict with prvSetupTimerInterrupt().
 */

/* ---------------------------------------------------------------------------------- *
 * Macro constants and definitions                                                    *
 * ---------------------------------------------------------------------------------- */

/* Proper alignment for the TMS570 MPU should be ensured by defining aligned stack types
 * or using compiler attributes. It typically requires regions to be aligned to their
 * size, so stacks and local data arrays should be aligned to their total byte size.
 */
#define TASK_STACK_WORDS(sz_bytes) ((sz_bytes) / sizeof(StackType_t))

/* Task priorities */
#define PRIO_INIT (configMAX_PRIORITIES - 1U) // Initialization task, runs once at startup
#define PRIO_RULES (5U)                       // Compliance checking for every sensor frame
#define PRIO_MOTOR_FSM (4U)                   // Real-time control loop, responds to commands
#define PRIO_CMD_RX (3U)                      // Command reception affects motor control, must be responsive
#define PRIO_TELEMETRY (2U)                   // Periodic reporting, lowest urgency

/* Task stack sizes */
#define STACK_RULES TASK_STACK_WORDS(512)
#define STACK_MOTOR_FSM TASK_STACK_WORDS(1024)
#define STACK_CMD_RX TASK_STACK_WORDS(512)
#define STACK_TELEMETRY TASK_STACK_WORDS(1024)

/* Bits for sysEvents */
#define EVT_CAN_SENSORS (1 << 0)
#define EVT_CAN_SPEED (1 << 1)
#define EVT_CAN_MOTOR1 (1 << 2)
#define EVT_CAN_MOTOR2 (1 << 3)
#define EVT_CAN_DRIVERS_STATUS (1 << 4)
#define EVT_CAN_DYNAMIC (1 << 5)
// #define EVT_CMD_PENDING (1 << 1)
// #define EVT_SENSOR_RULES_READY (1 << 2)
// #define EVT_SENSOR_TELEMETRY_READY (1 << 3)
// #define EVT_MOTOR_STATUS_READY (1 << 4)

/* Queue lengths */
#define CAN_TX_QUEUE_LENGTH 16U
#define CAN_RX_QUEUE_LENGTH 32U
#define UART_TX_QUEUE_LENGTH 256U
#define UART_RX_QUEUE_LENGTH 256U

/* Each CAN controller should have its own RX/TX software queues so the ISR can decouple
 * the mailbox interrupt from task processing.
 */

/* CAN */
#define CAN_REG canREG1
#define SENSORS_MAILBOX canMESSAGE_BOX1        // Rx
#define SPEED_MAILBOX canMESSAGE_BOX2          // Rx
#define MOTOR1_MAILBOX canMESSAGE_BOX3         // Tx
#define MOTOR2_MAILBOX canMESSAGE_BOX4         // Tx
#define DRIVERS_STATUS_MAILBOX canMESSAGE_BOX5 // Rx
#define DYNAMIC_MAILBOX canMESSAGE_BOX6        // Rx

/* As mentioned in the ISR, 2 SCI interfaces may be needed, one for command reception
 * and another for telemetry.
 */

/* Commands and telemetry */
#define SCI_REG sciREG
// #define SCI_COMMANDS_REG sciREG2
// #define SCI_TELEMETRY_REG sciREG1

#define SPI_REG spiREG4

#define CLOCK_RTI_HZ 10000000                   // 10Mhz
#define SCREEN_UPDATE_TICKS (CLOCK_RTI_HZ / 30) // 30Hz update rate for the screen
#define DATA_UPDATE_TICKS (CLOCK_RTI_HZ / 60)   // 60Hz update rate for data updates

#define RTI_SCREEN_UPDATE rtiNOTIFICATION_COMPARE1
#define RTI_DATA_UPDATE rtiNOTIFICATION_COMPARE2
#define RTI_TELEMETRY_UPDATE rtiNOTIFICATION_COMPARE3

#define __FOREVER__ for (;;)

/* ---------------------------------------------------------------------------------- *
 * Enumerations, structures and typedefs                                              *
 * ---------------------------------------------------------------------------------- */

// typedef enum
// {
//     TASK1_ID = 0,
//     TASK2_ID,
//     TASK3_ID,
//     TASK4_ID
// } TaskId_t;

// TODO: CAN drivers/wrappers only need data.
typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} CanFrame_t; /* CAN frame payload */

// TODO: Simplify concurrency by using multiple queues, atomic reads/writes, etc.
/* Shared snapshot of the latest received CAN frame (multiple task reception). The ISR
 * updates the sequence counter before/after copying the frame. Tasks detect a stable
 * copy using a retry loop: read sequence before, frame, sequence after. Retry if odd
 * (write in progress) or if sequence changed (write completed during read).
 */
typedef struct
{
    volatile uint32_t sequence;
    CanFrame_t frame;
} CanFrameSnapshot_t;

/* Controller communication struct for organizing queue/snapshot/events. */
typedef struct
{
    QueueHandle_t txQ;           /* outbound frames */
    QueueHandle_t rxQ;           /* inbound frames (for queuing before snapshot) */
    CanFrameSnapshot_t snapshot; /* latest RX frame with sequence lock */
    uint32_t okCount;            /* successful RX count */
    uint32_t failCount;          /* failed RX count */
} CanControllerComm_t;

/* Application handles for tasks and resources */
typedef struct
{
    /* Tasks */
    TaskHandle_t telemetryTask;
    TaskHandle_t rulesTask;
    TaskHandle_t cmdRxTask;
    TaskHandle_t motorFSMTask;

    // TODO: Update and unify legacy handle management.
    /* Inter-task communications */
    QueueHandle_t canTxQ;         /* outbound CAN frames */
    QueueHandle_t canRxQ;         /* inbound CAN frames */
    QueueHandle_t uartTxQ;        /* outbound bytes/frames from SCI */
    QueueHandle_t uartRxQ;        /* inbound bytes/frames from SCI */
    SemaphoreHandle_t cfgMutex;   /* protect shared config data */
    SemaphoreHandle_t dataMutex;  /* protect shared data */
    EventGroupHandle_t sysEvents; /* system-level event bits */

    QueueHandle_t telemetryQueue; /* telemetry data for SCI transmission */

    TimerHandle_t telemetryTimer; /* periodic timer for telemetry */
} AppHandles_t;

// TODO: Remove legacy snapshot.
/* Extended snapshot with counters. */
typedef struct
{
    volatile uint32_t sequence;
    CanFrame_t frame;
    uint32_t okCount;
    uint32_t failCount;
} SensorSnapshot_t;

/* Application resources (stacks, buffers, shared regions) */
typedef struct
{
    // Task TCBs and stacks
    StaticTask_t telemetryTaskBuffer;
    StaticTask_t rulesTaskBuffer;
    StaticTask_t cmdRxTaskBuffer;
    StaticTask_t motorTaskBuffer;

    // TODO: Define aligned arrays as static variables outside the struct, and store
    // pointers here (alignment syntax inside a struct/typedef is not supported by the
    // TI compiler of the TMS570).
    /* For memory alignment, #pragma directives must be placed in a separate line and
     * cannot be embedded within other statements.
     *
     * GCC, Clang and TI Arm Clang support the aligned attribute:
     * typedef struct {
     *     StackType_t telemetryStack[STACK_TELEMETRY]; __attribute__((aligned(STACK_TELEMETRY * sizeof(StackType_t))));
     * } AlignedStack_t;
     *
     * Standard C11/C23 use the alignas keyword:
     * #include <stdalign.h>
     * typedef struct {
     *    alignas(STACK_TELEMETRY * sizeof(StackType_t)) StackType_t telemetryStack[STACK_TELEMETRY];
     * } AlignedStack_t;
     *
     * Another option is to use struct alignment (TI supported), but it does not
     * automatically apply to arrays of that struct or to the struct if it’s nested:
     * #pragma STRUCT_ALIGN(AlignedStack_t, STACK_TELEMETRY * sizeof(StackType_t))
     * typedef struct {
     *   StackType_t telemetryStack[STACK_TELEMETRY];
     * } AlignedStack_t;
     */
    // #pragma DATA_ALIGN(telemetryStack, STACK_TELEMETRY * sizeof(StackType_t))
    StackType_t telemetryStack[STACK_TELEMETRY];
    StackType_t rulesStack[STACK_RULES];
    StackType_t cmdRxStack[STACK_CMD_RX];
    StackType_t motorStack[STACK_MOTOR_FSM];

    // Message queues and storage buffers
    StaticQueue_t canDriversTxQueueBuffer;
    StaticQueue_t canDriversRxQueueBuffer;
    StaticQueue_t canSensorsTxQueueBuffer;
    StaticQueue_t canSensorsRxQueueBuffer;
    StaticQueue_t uartTxQueueBuffer;
    StaticQueue_t uartRxQueueBuffer;
    uint8_t ucCanDriversTxQueueStorage[CAN_DRIVERS_TX_QUEUE_LENGTH * sizeof(CanFrame_t)];
    uint8_t ucCanDriversRxQueueStorage[CAN_DRIVERS_RX_QUEUE_LENGTH * sizeof(CanFrame_t)];
    uint8_t ucCanSensorsTxQueueStorage[CAN_SENSORS_TX_QUEUE_LENGTH * sizeof(CanFrame_t)];
    uint8_t ucCanSensorsRxQueueStorage[CAN_SENSORS_RX_QUEUE_LENGTH * sizeof(CanFrame_t)];
    uint8_t ucUartTxQueueStorage[UART_TX_QUEUE_LENGTH * sizeof(uint8_t)];
    uint8_t ucUartRxQueueStorage[UART_RX_QUEUE_LENGTH * sizeof(uint8_t)];

    StaticQueue_t telemetryQueueBuffer;
    uint8_t ucTelemetryQueueStorage[UART_TX_QUEUE_LENGTH * sizeof(CanFrame_t)];

    // Semaphores for signaling
    StaticSemaphore_t cfgMutexBuffer;
    StaticSemaphore_t dataMutexBuffer;
    // Event group for system-level events (e.g. new CAN frame)
    StaticEventGroup_t sysEventsBuffer;

    /* Per-controller communication: snapshots, queues, counters */
    CanControllerComm_t canDriversComm; /* CAN1: sensor board (telemetry) */
    CanControllerComm_t canSensorsComm; /* CAN2: motor driver (commands/feedback) */

    SensorSnapshot_t sensorSnapshot;

    // Shared MPU regions for all tasks
    MemoryRegion_t sharedRegions[portNUM_CONFIGURABLE_REGIONS];
} AppResources_t;

// TODO: Better isolation and per-task regions. Define separate resource structs per
// task, each with only the queues/buffers/stacks that it needs.
/* Unified resources with shared MPU regions (static allocation). All tasks that are
 * unprivileged restricted enable MPU protection of the entire task subsystem by
 * pointing their regions to this struct.
 */
typedef struct
{
    AppResources_t *resources;
    AppHandles_t *handles;
} AppContext_t;

/* ---------------------------------------------------------------------------------- *
 * Prototypes for private functions with file level scope                             *
 * ---------------------------------------------------------------------------------- */

/**
 * @brief Initialize peripherals and modules (CAN, SCI, etc.)
 */
static void vModulesInit(void);

/**
 * @brief Initialize modules that require interrupts to be enabled
 */
static void vInitModulesWithInterrupts(void);

/**
 * @brief Create all application tasks and resources
 */
static void vTasksInit(void);
static void vPublishSensorSnapshot(AppResources_t *pxResources, const CanFrame_t *pxFrame, BaseType_t xOk);

// Tasks ///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Main tasks
 * - Initialization task: performs setup that requires interrupts, then deletes itself
 * - Telemetry task: collects vehicle telemetry, packs it and enqueues for SCI
 * - Rules task: checks compliance with rules and sets alarms/events
 * - Command RX task: receives commands from SCI, validates and dispatches to motor FSM
 * - Motor state machine task: main control for motor drivers
 */
static portTASK_FUNCTION_PROTO(vInitTask, pvParameters);
static portTASK_FUNCTION_PROTO(vMotorFSMTask, pvParameters);
static portTASK_FUNCTION_PROTO(vCmdRxTask, pvParameters);
static portTASK_FUNCTION_PROTO(vRulesTask, pvParameters);
static portTASK_FUNCTION_PROTO(vTelemetryTask, pvParameters);

/* An extra dispatcher task could be added to assemble frames, parse commands, etc., and
 * then signal other tasks. The CMD task could be integrated if processing is simple.
 */

// Callbacks ///////////////////////////////////////////////////////////////////////////

static void vInitTimerCallback(TimerHandle_t xTimer);

/* ---------------------------------------------------------------------------------- *
 * Static variables and constants with file level scope                               *
 * ---------------------------------------------------------------------------------- */

static AppHandles_t xAppHandles;
static AppResources_t xAppResources;

/* Unified application resources and task handles. All static allocation consolidated
 * here. This struct is populated by vCreateAppTasks.
 */
static AppContext_t xAppContext = {
    .resources = &xAppResources,
    .handles = &xAppHandles,
};

Bridgetek_EVE2 display_data;

/* ---------------------------------------------------------------------------------- *
 * Global function definitions                                                        *
 * ---------------------------------------------------------------------------------- */

void vAppInit(void)
{
    /* configASSERT() may be disabled once the application is stable to remove the extra
     * overhead. More robust initialization with error checking would be needed then.
     */

    /* Initialize modules and create Tasks */
    vModulesInit();
    vTasksInit();

    /* Interrupts should be off here, to ensure a tick does not occur before or during
     * the call to vTaskStartScheduler(). The stacks of the created tasks contain a
     * status word with interrupts switched on so they will automatically get re-enabled
     * when the first task starts to run. To initialize peripherals that require
     * interrupts, a dedicated init task can be used that performs the necessary setup
     * and then deletes itself (see vInitTask), or alternatively use a timer callback
     * that runs once after scheduler start (see vInitTimerCallback).
     */
    // portENABLE_INTERRUPTS();
    // vInitModulesWithInterrupts();
    // portDISABLE_INTERRUPTS();

    /* Start Scheduler */
    vTaskStartScheduler();
}

void vAppRun(void)
{
}

// ISRs ////////////////////////////////////////////////////////////////////////////////

/* Notifications can be defined in notification.c, or be redefined externally thanks to
 * the WEAK pragma. If a link set contains both a weak definition (notification.c) and a
 * non-weak definition (e.g., sys_main.c), the non-weak definition is always used. In
 * the FreeRTOS context, use _FROM_ISR APIs and set xHigherPriorityTaskWoken.
 *
 * Error handling needs improvement, consider logging or setting an event bit. Queue
 * management depends on transmission trigger - periodic or event driven.
 */

/**
 * @brief ISR handler for CAN TX/RX interrupt
 */
void canMessageNotification(canBASE_t *node, uint32 messageBox)
{
    /* For data shared between tasks (e.g., rules and telemetry), RTOS communication
     * objects are very restricted. A single queue could be used if another task handles
     * the dispatching (ISR -> sensor RX queue -> sensor task -> rules/telemetry). That
     * task updates a shared latest-value struct under a mutex (or a sequence lock),
     * then notifies both tasks with task notifications or an event bit. An ISR must
     * never access a mutex or any RTOS API that can block.
     */
    /* Mark which sensor mailbox changed and wake the rules task to perform the multiple
     * reads, process the data and send it to the telemetry task.
     */

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t xEvent = (EventBits_t)0U;

    if (node == CAN_REG)
    {
        switch (messageBox)
        {
        case SENSORS_MAILBOX:
            sensors_data.new_data = true;
            xEvent |= EVT_CAN_SENSORS;
            break;
        case SPEED_MAILBOX:
            speed_data.new_data = true;
            xEvent |= EVT_CAN_SPEED;
            break;
        case MOTOR1_MAILBOX:
            motor1_data.new_data = true;
            xEvent |= EVT_CAN_MOTOR1;
            break;
        case MOTOR2_MAILBOX:
            motor2_data.new_data = true;
            xEvent |= EVT_CAN_MOTOR2;
            break;
        case DRIVERS_STATUS_MAILBOX:
            drivers_status_data.new_data = true;
            xEvent |= EVT_CAN_DRIVERS_STATUS;
            break;
        case DYNAMIC_MAILBOX:
            dynamic_data.new_data = true;
            xEvent |= EVT_CAN_DYNAMIC;
            break;
        default:
            // Handle other message boxes if needed
            break;
        }

        if ((xEvent != (EventBits_t)0U) && (xAppHandles.sysEvents != NULL))
        {
            (void)xEventGroupSetBitsFromISR(xAppHandles.sysEvents, xEvent, &xHigherPriorityTaskWoken);
        }
    }

    if (xHigherPriorityTaskWoken != pdFALSE)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void rtiNotification(uint32 notification)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    CanFrame_t tx, rx = {.id = CAN_DRIVERS_RX, .dlc = 8};

    if (notification == rtiNOTIFICATION_COMPARE1) // Update display at 30Hz
    {
        update_dashboard_draw(display_data, &dashboard_data);
    }
    else if (notification == rtiNOTIFICATION_COMPARE0) // Update data at 60Hz
    {
        update_dashboard_data(&dashboard_data);

        test_main_ecu_periodic();
        update_data();

        ui_handle_touch(display_data, &dashboard_data);
    }
    else if (notification == rtiNOTIFICATION_COMPARE3) // Update telemetry at 50Hz
    {
    }
}

/**
 * @brief ISR handler for SCI TX/RX interrupt
 * @param sci SCI register frame pointer used to access the SCI module registers.
 * @param flags Status flags indicating the cause of the interrupt (e.g., RX ready)
 */
void sciNotification(sciBASE_t *sci, uint32 flags)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t c = 0;

    /* Telemetry and Command RX tasks currently both use the same queue. A dispatcher or
     * separate queues should be added if processing differs significantly. If the
     * length of the received frames is variable, sciReceiveByte would be preferred.
     * sciReceive is suitable as long as the "Length" field (third frame byte) is used.
     */

    // TODO: Dispatch SCI msgs separately.
    if ((sci == SCI_REG) && (xAppHandles.uartTxQ != NULL) && (xAppHandles.uartRxQ != NULL))
    {
        if ((flags & SCI_TX_INT) != 0U)
        {
            if (xQueueReceiveFromISR(xAppHandles.uartTxQ, &c, &xHigherPriorityTaskWoken) == pdTRUE)
            {
                sciSend(sci, sizeof(uint8_t), &c); // TODO: Update to send frames or use a dedicated command queue.
            }
        }
        else if ((flags & SCI_RX_INT) != 0U)
        {
            sciReceive(sci, sizeof(uint8_t), &c); // TODO: Update to receive frames or use a dedicated command queue.
            if (xQueueSendFromISR(xAppHandles.uartRxQ, &c, &xHigherPriorityTaskWoken) == pdFAIL)
            {
                /* Queue full, byte lost. Set an event bit or counter. */
            }
        }

        if (xHigherPriorityTaskWoken != pdFALSE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

// Static Internal Kernel Memory Allocation ////////////////////////////////////////////

/* When the static memory allocator is enabled (configSUPPORT_STATIC_ALLOCATION = 1),
 * the idle task and the timer task (configUSE_TIMERS = 1) will use static memory
 * supplied by user functions. The kernel will call vApplicationGetTimerTaskMemory() to
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
        *pulTimerTaskStackSize = sizeof(uxTimerTaskStack) / sizeof(*uxTimerTaskStack);
    }
#ifdef __cplusplus
}
#endif

/* ---------------------------------------------------------------------------------- *
 * Local function definitions                                                         *
 * ---------------------------------------------------------------------------------- */

static void vInitModules(void)
{
    /* Initialize peripherals. */
    canInit();
    gioInit();
    // rtiInit();
    sciInit();
    spiInit();
    TI_Fee_Init();
    while (TI_Fee_GetStatus(0) != IDLE)
        TI_Fee_MainFunction();

    display_data.setup(WQVGA);
    display_data.Init();
    /* Try restoring previous touch + debug calibration from EEPROM. */
    if (!loadCalibration(display_data))
    {
        /* If not found, perform fresh touch calibration. */
        eve_calibrate(display_data);
        saveCalibration(display_data);
    }
    init_data();
    test_main_ecu_init();
    init_dashboard(&dashboard_data);

    /* Enable peripheral callbacks. */
    canEnableErrorNotification(CAN_REG);
    canEnableStatusChangeNotification(canREG1);
    rtiEnableNotification(rtiNOTIFICATION_COMPARE1);
    rtiEnableNotification(rtiNOTIFICATION_COMPARE2);
    // sciEnableNotification(SCI_REG, (uint32)(SCI_TX_INT | SCI_RX_INT /* | SCI_... */));
    sciEnableNotification(SCI_REG, (uint32_t)0); // Interrupts enabled in HALCoGen
    spiEnableNotification(SPI_REG, (uint32_t)0); // Interrupts enabled in HALCoGen

    /* Add other initializations as needed. */
}

static void vInitModulesWithInterrupts(void)
{
    rtiStartCounter(rtiCOUNTER_BLOCK0); /* Start RTI counter block 0 */
}

static void vPublishSensorSnapshot(AppResources_t *pxResources, const CanFrame_t *pxFrame, BaseType_t xOk)
{
    configASSERT((pxResources != NULL) && (pxFrame != NULL));

    /* Sequence-lock style publish: ISR is the only writer. Increment sequence to mark
     * write start (odd), then copy frame, then increment again (even).
     */

    // TODO: Update legacy snapshot or remove entirely, and use a single atomic write if
    // the platform supports it, to avoid the need for a retry loop on the reader side.
    pxResources->sensorSnapshot.sequence++;
    pxResources->sensorSnapshot.frame = *pxFrame;
    if (xOk == pdTRUE)
    {
        pxResources->sensorSnapshot.okCount++;
    }
    else
    {
        pxResources->sensorSnapshot.failCount++;
    }
    pxResources->sensorSnapshot.sequence++;
}

static void vCreateAppTasks(void)
{
    AppResources_t *pxResources = xAppContext.resources;
    AppHandles_t *pxHandles = xAppContext.handles;

    /* Create comm objects used by tasks - populate pxResources->comms */
    configASSERT((pxHandles != NULL) && (pxResources != NULL));

    /* CAN1 queues (motor drivers) */
    pxHandles->canTxQ = xQueueCreateStatic(CAN_DRIVERS_TX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                           pxResources->ucCanDriversTxQueueStorage,
                                           &pxResources->canDriversTxQueueBuffer);

    pxHandles->canRxQ = xQueueCreateStatic(CAN_DRIVERS_RX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                           pxResources->ucCanDriversRxQueueStorage,
                                           &pxResources->canDriversRxQueueBuffer);

    /* CAN2 queues (sensors) */
    pxResources->canSensorsComm.txQ = xQueueCreateStatic(CAN_SENSORS_TX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                                         pxResources->ucCanSensorsTxQueueStorage,
                                                         &pxResources->canSensorsTxQueueBuffer);

    pxResources->canSensorsComm.rxQ = xQueueCreateStatic(CAN_SENSORS_RX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                                         pxResources->ucCanSensorsRxQueueStorage,
                                                         &pxResources->canSensorsRxQueueBuffer);

    configASSERT((pxResources->canDriversComm.txQ = pxHandles->canTxQ) != NULL);
    configASSERT((pxResources->canDriversComm.rxQ = pxHandles->canRxQ) != NULL);
    configASSERT((pxResources->canSensorsComm.txQ = pxResources->canSensorsComm.txQ) != NULL);
    configASSERT((pxResources->canSensorsComm.rxQ = pxResources->canSensorsComm.rxQ) != NULL);

    /* Link CAN1 comms to legacy handles */
    // pxResources->canDriversComm.txQ = pxHandles->canTxQ;
    // pxResources->canDriversComm.rxQ = pxHandles->canRxQ;

    configASSERT((pxHandles->uartTxQ = xQueueCreateStatic(UART_TX_QUEUE_LENGTH, sizeof(uint8_t),
                                                          pxResources->ucUartTxQueueStorage,
                                                          &pxResources->uartTxQueueBuffer)) != NULL);
    configASSERT((pxHandles->uartRxQ = xQueueCreateStatic(UART_RX_QUEUE_LENGTH, sizeof(uint8_t),
                                                          pxResources->ucUartRxQueueStorage,
                                                          &pxResources->uartRxQueueBuffer)) != NULL);
    configASSERT((pxHandles->cfgMutex = xSemaphoreCreateMutexStatic(&pxResources->cfgMutexBuffer)) != NULL);
    configASSERT((pxHandles->dataMutex = xSemaphoreCreateMutexStatic(&pxResources->dataMutexBuffer)) != NULL);
    configASSERT((pxHandles->sysEvents = xEventGroupCreateStatic(&pxResources->sysEventsBuffer)) != NULL);

    xTaskCreateStatic(vInitTask, "Init", STACK_INIT, (void *)pxResources, PRIO_INIT, pxResources->initStack, &pxResources->initTaskBuffer);
    /* Alternatively, use a timer that starts after vTaskStartScheduler. */
    // TimerHandle_t xInitTimer = xTimerCreate("InitPost", 1, pdFALSE, NULL, vInitTimerCallback);
    // xTimerStart(xInitTimer, 0);

    xTaskCreateStatic(vMotorFSMTask, "MotorFSM", STACK_MOTOR_FSM, (void *)pxResources, PRIO_MOTOR_FSM, pxResources->motorStack, &pxResources->motorTaskBuffer);
    xTaskCreateStatic(vRulesTask, "Rules", STACK_RULES, (void *)pxResources, PRIO_RULES, pxResources->rulesStack, &pxResources->rulesTaskBuffer);
    xTaskCreateStatic(vCmdRxTask, "CmdRx", STACK_CMD_RX, (void *)pxResources, PRIO_CMD_RX, pxResources->cmdRxStack, &pxResources->cmdRxTaskBuffer);
    xTaskCreateStatic(vTelemetryTask, "Telemetry", STACK_TELEMETRY, (void *)pxResources, PRIO_TELEMETRY, pxResources->telemetryStack, &pxResources->telemetryTaskBuffer);
}

// Tasks ///////////////////////////////////////////////////////////////////////////////

static portTASK_FUNCTION(vInitTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;

    /* Interrupts are enabled here - initialize modules that depend on them */
    vInitModulesWithInterrupts();

    vTaskDelete(NULL);
}

static portTASK_FUNCTION(vTelemetryTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppHandles_t *pxHandles = pxCtx->handles;
    // char telemetryLine[192];
    // int lineLength;
    // struct
    // {
    //     float rpm;
    //     float motor1_voltage;
    //     float motor1_current;
    //     float motor1_temp;
    //     float motor2_voltage;
    //     float motor2_current;
    //     float motor2_temp;
    //     float tps;
    //     float brake_front;
    //     float brake_rear;
    //     float steering_angle;
    //     float battery_voltage;
    //     float battery_current;
    //     float wheel_speed_fl;
    //     float wheel_speed_fr;
    //     float wheel_speed_rl;
    //     float wheel_speed_rr;
    //     uint8_t motor1_fault;
    //     uint8_t motor2_fault;
    //     uint8_t drive_state;
    // } snapshot;

    __FOREVER__
    {
        // /* Read frame and send to queue */
        // if (canGetData(CAN_SENSORS_REG, CAN_SENSORS_RX, (uint8 *)rx.data) == (uint32_t)1)
        // {
        //     if (xQueueSendFromISR(xAppHandles.canRxQ, &rx, &xHigherPriorityTaskWoken) == pdFAIL)
        //     {
        //         /* Queue full, frame lost. */
        //     }
        //     else
        //     {
        //         vPublishSensorSnapshot(&xAppResources, &rx, pdTRUE);

        //         if (xAppHandles.sysEvents != NULL)
        //         {
        //             (void)xEventGroupSetBitsFromISR(xAppHandles.sysEvents,
        //                                             (EVT_SENSOR_RULES_READY | EVT_SENSOR_TELEMETRY_READY),
        //                                             &xHigherPriorityTaskWoken);
        //         }
        //     }
        // }
        // // else: error reading from CAN peripheral

        // TODO: Update with the protection mechanism used for shared data.

        (void)xEventGroupWaitBits(pxHandles->sysEvents,
                                  EVT_SENSOR_TELEMETRY_READY,
                                  pdTRUE,
                                  pdFALSE,
                                  portMAX_DELAY);

        /* Gather protected data. */
        if (xSemaphoreTake(pxHandles->dataMutex, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            /* ... read shared config or sensor settings safely ... */
            // snapshot.rpm = dashboard_data.rpm;
            // snapshot.motor1_voltage = dashboard_data.motor1_voltage;
            // snapshot.motor1_current = dashboard_data.motor1_current;
            // snapshot.motor1_temp = dashboard_data.motor1_temp;
            // snapshot.motor2_voltage = dashboard_data.motor2_voltage;
            // snapshot.motor2_current = dashboard_data.motor2_current;
            // snapshot.motor2_temp = dashboard_data.motor2_temp;
            // snapshot.tps = dashboard_data.tps;
            // snapshot.brake_front = dashboard_data.brake_front;
            // snapshot.brake_rear = dashboard_data.brake_rear;
            // snapshot.steering_angle = dashboard_data.steering_angle;
            // snapshot.battery_voltage = dashboard_data.battery_voltage;
            // snapshot.battery_current = dashboard_data.battery_current;
            // snapshot.wheel_speed_fl = dashboard_data.wheel_speed_fl;
            // snapshot.wheel_speed_fr = dashboard_data.wheel_speed_fr;
            // snapshot.wheel_speed_rl = dashboard_data.wheel_speed_rl;
            // snapshot.wheel_speed_rr = dashboard_data.wheel_speed_rr;
            // snapshot.motor1_fault = dashboard_data.motor1_fault;
            // snapshot.motor2_fault = dashboard_data.motor2_fault;
            // snapshot.drive_state = dashboard_data.drive_state;
            xSemaphoreGive(pxHandles->dataMutex);
        }

        /* Alternatively use a published snapshot with sequence-lock retry. */
        do
        {
            seqBefore = pxResources->sensorSnapshot.sequence;
            txFrame = pxResources->sensorSnapshot.frame;
            seqAfter = pxResources->sensorSnapshot.sequence;
        } while ((seqBefore != seqAfter) || (seqBefore & 1U));

        /* ... process and prepare the telemetry data for transmission ... */

        if (xQueueSend(pxHandles->canTxQ, &txFrame, pdMS_TO_TICKS(10)) == pdPASS)
        {
            /* TX is handled by the CAN ISR; telemetry only enqueues the command. */
            xEventGroupSetBits(pxHandles->sysEvents, EVT_SENSOR_TELEMETRY_READY);
        }

        // TODO: Use a timer or event notification instead of fixed delay, to achieve
        // more precise periodicity and/or trigger on new data.
        vTaskDelay(pdMS_TO_TICKS(200)); /* periodic telemetry */

        // lineLength = snprintf(telemetryLine,
        //                       sizeof(telemetryLine),
        //                       "RPM=%ld,M1=%ld,%ld,%ld,M2=%ld,%ld,%ld,TPS=%ld,BRK=%ld,%ld,STEER=%ld,BV=%ld,BC=%ld,WS=%ld,%ld,%ld,%ld,FLT=%u,%u,STATE=%u\n",
        //                       (long)snapshot.rpm,
        //                       (long)snapshot.motor1_voltage,
        //                       (long)snapshot.motor1_current,
        //                       (long)snapshot.motor1_temp,
        //                       (long)snapshot.motor2_voltage,
        //                       (long)snapshot.motor2_current,
        //                       (long)snapshot.motor2_temp,
        //                       (long)snapshot.tps,
        //                       (long)snapshot.brake_front,
        //                       (long)snapshot.brake_rear,
        //                       (long)snapshot.steering_angle,
        //                       (long)snapshot.battery_voltage,
        //                       (long)snapshot.battery_current,
        //                       (long)snapshot.wheel_speed_fl,
        //                       (long)snapshot.wheel_speed_fr,
        //                       (long)snapshot.wheel_speed_rl,
        //                       (long)snapshot.wheel_speed_rr,
        //                       (unsigned)snapshot.motor1_fault,
        //                       (unsigned)snapshot.motor2_fault,
        //                       (unsigned)snapshot.drive_state);

        // if (lineLength > 0)
        // {
        //     (void)sciSend(sciREG, (uint32_t)lineLength, (uint8 *)telemetryLine);
        // }
    }
}

static portTASK_FUNCTION(vRulesTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppHandles_t *pxHandles = pxCtx->handles;
    EventBits_t uxBits;

    __FOREVER__
    {
        /* Wait for an event or poll periodically */
        /* Wake when any sensor mailbox has been updated by the CAN ISR. */
        uxBits = xEventGroupWaitBits(pxHandles->sysEvents, EVT_SENSOR_RULES_READY, pdTRUE, pdFALSE, pdMS_TO_TICKS(500));
        if ((uxBits & EVT_SENSOR_RULES_READY) != 0)
        {
            //         /* Identify what sensor data is available - alternatively indicate it in the flag */

            //         /* Read frame and send to queue */
            //         if (canGetData(CAN_REG, x_MAILBOX, (uint8 *)sensorFrame.data) == (uint32_t)1)
            //         {
            //                        // if (xQueueSend(xAppHandles.canRxQ, &sensorFrame, (TickType_t)0) == pdFAIL)
            //             if (xQueueSend(pxHandles->telemetryQueue, &sensorFrame, (TickType_t)0) == pdFAIL)
            //             // if (xTaskNotify(pxHandles->telemetryTask, ) != pdPASS)
            //             // if (xMessageBufferSend(xMessageBuffer, &sensorFrame, sizeof(sensorFrame), (TickType_t)0) == pdFAIL)
            //             /* Stream buffers not available in this version. */
            //             {
            //                 /* Queue full, frame lost. */
            //             }
            //             else
            //             {
            //                 vPublishSensorSnapshot(&xAppResources, &sensorFrame, pdTRUE);

            //                 if (xAppHandles.sysEvents != NULL)
            //                 {
            //                     (void)xEventGroupSetBitsFromISR(xAppHandles.sysEvents,
            //                                                     (EVT_SENSOR_RULES_READY | EVT_SENSOR_TELEMETRY_READY),
            //                                                     &xHigherPriorityTaskWoken);
            //                 }
            //             }
            //         }
            //         // else: error reading from CAN peripheral

            //         /* Consume the latest published sensor snapshot with sequence-lock retry. */
            //         do
            //         {
            //             seqBefore = pxResources->sensorSnapshot.sequence;
            //             sensorFrame = pxResources->sensorSnapshot.frame;
            //             seqAfter = pxResources->sensorSnapshot.sequence;
            //         } while ((seqBefore != seqAfter) || (seqBefore & 1U));

            //         /* ... apply rules with consistent snapshot ... */
            //         (void)sensorFrame;
            //     }

            //     /* run compliance checks periodically */
            //     vTaskDelay(pdMS_TO_TICKS(100));
            // }
            if (xSemaphoreTake(pxHandles->dataMutex, pdMS_TO_TICKS(50)) == pdTRUE)
            {
                update_data();
                xSemaphoreGive(pxHandles->dataMutex);

                if (pxHandles->sysEvents != NULL)
                {
                    (void)xEventGroupSetBits(pxHandles->sysEvents, EVT_SENSOR_TELEMETRY_READY);
                }
            }

            /* Rules analysis can run here using the now-consistent dashboard_data. */
        }
    }
}

static portTASK_FUNCTION(vCmdRxTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    // AppResources_t *pxResources = pxCtx->resources;
    AppHandles_t *pxHandles = pxCtx->handles;
    uint8_t b;

    __FOREVER__
    {
        if (xQueueReceive(pxHandles->uartRxQ, &b, portMAX_DELAY) == pdPASS)
        {
            xEventGroupSetBits(pxHandles->sysEvents, EVT_CMD_PENDING);

            /* ... process the received command before sending to motor FSM ... */

            /* Local echo of received byte. */
            // sciSendByte(sciREG, (uint8)b);
        }
    }
}

static portTASK_FUNCTION(vMotorFSMTask, pvParameters)
{
    // AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    // AppResources_t *pxResources = pxCtx->resources;
    // AppHandles_t *pxHandles = pxCtx->handles;

    __FOREVER__
    {
        /* Manage FSM and process commands. */
        // ...

        // /* Use mutex to update shared config/state */
        // if (xSemaphoreTake(pxHandles->cfgMutex, pdMS_TO_TICKS(10)) == pdTRUE)
        // {
        //     /* update state machine variables safely */
        //     xSemaphoreGive(pxHandles->cfgMutex);
        // }

        /* Run control step */

        /* This has to be done in the MotorFSM Task. */
        /* Receive frame from queue and transmit */
        // if (xQueueReceiveFromISR(xAppHandles.canTxQ, &tx, &xHigherPriorityTaskWoken) == pdPASS)
        // {
        //     if (canTransmit(CAN_REG, MOTOR1_MAILBOX, (const uint8 *)tx.data) == (uint32_t)0)
        //     {
        //         /* Error transmitting frame. */
        //     }
        // }
        /* else: queue empty, nothing to send */

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Callbacks ///////////////////////////////////////////////////////////////////////////

static void vInitTimerCallback(TimerHandle_t xTimer)
{
    vInitModulesWithInterrupts();
    xTimerDelete(xTimer, 0);
}
