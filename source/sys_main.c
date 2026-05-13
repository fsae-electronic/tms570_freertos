/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */

/* ---------------------------------------------------------------------------------- *
 * Headers                                                                            *
 * ---------------------------------------------------------------------------------- */

/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */

// Standard library includes
#include <stdint.h>
#include <string.h>

/* TMS570 supports MPU, so the app will call MPU_xxx wrappers transparently (because
 * os_mpu_wrappers.h does the remapping). MPU_WRAPPERS_INCLUDED_FROM_API_FILE should not
 * be used here because defining it prevents os_task.h from redefining all the API
 * functions to use the MPU wrappers.
 */

// FreeRTOS includes
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_queue.h"
#include "os_semphr.h"
#include "os_event_groups.h"
#include "os_timer.h"

// Peripheral headers - project-specific (HALCoGen generated)
#include "can.h"
#include "sci.h"
#include "esm.h"
#include "gio.h"

/* ---------------------------------------------------------------------------------- *
 * Macro constants and definitions                                                    *
 * ---------------------------------------------------------------------------------- */

/* Proper alignment for the TMS570 MPU should be ensured by defining aligned stack types
 * or using compiler attributes. It typically requires regions to be aligned to their
 * size, so stacks and local data arrays should be aligned to their total byte size.
 */
#define TASK_STACK_WORDS(sz_bytes) ((sz_bytes) / sizeof(StackType_t))

/* Task priorities */
#define PRIO_RULES (5U)     // Compliance checking for every sensor frame
#define PRIO_MOTOR_FSM (4U) // Real-time control loop, responds to commands
#define PRIO_CMD_RX (3U)    // Command reception affects motor control, must be responsive
#define PRIO_TELEMETRY (2U) // Periodic reporting, lowest urgency

/* Task stack sizes */
#define STACK_RULES TASK_STACK_WORDS(512)
#define STACK_MOTOR_FSM TASK_STACK_WORDS(1024)
#define STACK_CMD_RX TASK_STACK_WORDS(512)
#define STACK_TELEMETRY TASK_STACK_WORDS(1024)

/* Bits for sysEvents */
#define EVT_CAN_RX_AVAILABLE (1 << 0)
#define EVT_CMD_PENDING (1 << 1)
#define EVT_SENSOR_RULES_READY (1 << 2)
#define EVT_SENSOR_TELEMETRY_READY (1 << 3)
#define EVT_MOTOR_STATUS_READY (1 << 4)

/* Queue lengths */
#define CAN_DRIVERS_TX_QUEUE_LENGTH 16U
#define CAN_SENSORS_TX_QUEUE_LENGTH 16U
#define CAN_DRIVERS_RX_QUEUE_LENGTH 32U
#define CAN_SENSORS_RX_QUEUE_LENGTH 32U
#define UART_TX_QUEUE_LENGTH 256U
#define UART_RX_QUEUE_LENGTH 256U

/* Each CAN controller should have its own RX/TX software queues so the ISR can decouple
 * the mailbox interrupt from task processing.
 */

/* CAN1: motor commands and feedback */
#define CAN_DRIVERS_REG canREG1
#define CAN_DRIVERS_TX canMESSAGE_BOX1
#define CAN_DRIVERS_RX canMESSAGE_BOX2

/* CAN2: sensor board */
#define CAN_SENSORS_REG canREG2
#define CAN_SENSORS_TX canMESSAGE_BOX3
#define CAN_SENSORS_RX canMESSAGE_BOX4

/* As mentioned in the ISR, 2 SCI interfaces may be needed, one for command reception
 * and another for telemetry.
 */

/* Commands and telemetry */
#define SCI_REG sciREG
// #define SCI_COMMANDS_REG sciREG2
// #define SCI_TELEMETRY_REG sciREG1

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
 * @brief Initialize peripherals and modules (CAN, SCI, ESM, etc.)
 */
static void vInitModules(void);

/**
 * @brief Create all application tasks and resources
 * @param xHandles Pointer to struct where created task handles will be stored
 * @param pxResources Pointer to unified resource struct (stacks, buffers, regions)
 */
static void vCreateAppTasks(AppContext_t *pxContext);
static void vPublishSensorSnapshot(AppResources_t *pxResources, const CanFrame_t *pxFrame, BaseType_t xOk);

// Tasks ///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Main tasks
 * - Telemetry task: collects vehicle telemetry, packs it and enqueues for SCI
 * - Rules task: checks compliance with rules and sets alarms/events
 * - Command RX task: receives commands from SCI, validates and dispatches to motor FSM
 * - Motor state machine task: main control for motor drivers
 */
static portTASK_FUNCTION_PROTO(vTelemetryTask, pvParameters);
static portTASK_FUNCTION_PROTO(vRulesTask, pvParameters);
static portTASK_FUNCTION_PROTO(vCmdRxTask, pvParameters);
static portTASK_FUNCTION_PROTO(vMotorFSMTask, pvParameters);

/* An extra dispatcher task could be added to assemble frames, parse commands, etc., and
 * then signal other tasks. The CMD task could be integrated if processing is simple.
 */

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
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                   StackType_t **ppxTimerTaskStackBuffer,
                                   uint32_t *pulTimerTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);

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

/* ---------------------------------------------------------------------------------- *
 * Global function definitions                                                        *
 * ---------------------------------------------------------------------------------- */

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    /* Interrupts should be off here, to ensure a tick does not occur before or during
     * the call to vTaskStartScheduler(). The stacks of the created tasks contain a
     * status word with interrupts switched on so interrupts will automatically get
     * re-enabled when the first task starts to run.
     */

    /* Needs more robust initialization with error checking once configASSERT() is
     * disabled (the additional overhead can be removed when the application is stable).
     */

    /* Create Tasks */
    vInitModules();
    vCreateAppTasks(&xAppContext);

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever (should never reach here) */
    __FOREVER__;

    /* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

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
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    CanFrame_t tx, rx = {.id = CAN_DRIVERS_RX, .dlc = 8};

    /* For data shared between tasks (e.g., rules and telemetry), RTOS communication
     * objects are very restricted. A single queue could be used if another task handles
     * the dispatching (ISR -> sensor RX queue -> sensor task -> rules/telemetry). That
     * task updates a shared latest-value struct under a mutex (or a sequence lock),
     * then notifies both tasks with task notifications or an event bit. An ISR must
     * never access a mutex or any RTOS API that can block.
     */

    // TODO: Handle both CAN controllers, update concurrency model.
    if ((node == CAN_DRIVERS_REG) && (xAppHandles.canTxQ != NULL) && (xAppHandles.canRxQ != NULL))
    {
        if (messageBox == CAN_SENSORS_TX)
        {
            /* Receive frame from queue and transmit */
            if (xQueueReceiveFromISR(xAppHandles.canTxQ, &tx, &xHigherPriorityTaskWoken) == pdPASS)
            {
                if (canTransmit(CAN_SENSORS_REG, CAN_SENSORS_TX, (const uint8 *)tx.data) == (uint32_t)0)
                {
                    /* Error transmitting frame. */
                }
            }
            // else: queue empty, nothing to send
        }
        else if (messageBox == CAN_SENSORS_RX)
        {
            /* Read frame and send to queue */
            if (canGetData(CAN_SENSORS_REG, CAN_SENSORS_RX, (uint8 *)rx.data) == (uint32_t)1)
            {
                if (xQueueSendFromISR(xAppHandles.canRxQ, &rx, &xHigherPriorityTaskWoken) == pdFAIL)
                {
                    /* Queue full, frame lost. */
                }
                else
                {
                    vPublishSensorSnapshot(&xAppResources, &rx, pdTRUE);

                    if (xAppHandles.sysEvents != NULL)
                    {
                        (void)xEventGroupSetBitsFromISR(xAppHandles.sysEvents,
                                                        (EVT_SENSOR_RULES_READY | EVT_SENSOR_TELEMETRY_READY),
                                                        &xHigherPriorityTaskWoken);
                    }
                }
            }
            // else: error reading from CAN peripheral
        }
        // ... drivers data handling ...

        /* Signal tasks */
        if (xHigherPriorityTaskWoken != pdFALSE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
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

/* ---------------------------------------------------------------------------------- *
 * Local function definitions                                                         *
 * ---------------------------------------------------------------------------------- */

static void vInitModules(void)
{
    /* Initialize peripherals used by the application. */
    canInit();
    sciInit();
    esmInit();
    gioInit();

    /* Enable peripheral callbacks used by notifications. */
    canEnableErrorNotification(CAN_DRIVERS_REG);
    canEnableStatusChangeNotification(CAN_DRIVERS_REG);
    canEnableErrorNotification(CAN_SENSORS_REG);
    canEnableStatusChangeNotification(CAN_SENSORS_REG);
    // sciEnableNotification(SCI_REG, (uint32)(SCI_TX_INT | SCI_RX_INT /* | SCI_... */));
    sciEnableNotification(SCI_REG, (uint32_t)0); // Interrupts enabled in HALCoGen

    // Add other peripheral initializations as needed.
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

static void vCreateAppTasks(AppContext_t *pxContext)
{
    AppResources_t *pxResources = pxContext->resources;
    AppHandles_t *pxHandles = pxContext->handles;

    /* Create comm objects used by tasks - populate pxResources->comms */
    configASSERT((pxHandles != NULL) && (pxResources != NULL));

    /* CAN1 queues (motor drivers) */
    configASSERT((pxHandles->canTxQ = xQueueCreateStatic(CAN_DRIVERS_TX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                                         pxResources->ucCanDriversTxQueueStorage,
                                                         &pxResources->canDriversTxQueueBuffer)) != NULL);

    configASSERT((pxHandles->canRxQ = xQueueCreateStatic(CAN_DRIVERS_RX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                                         pxResources->ucCanDriversRxQueueStorage,
                                                         &pxResources->canDriversRxQueueBuffer)) != NULL);

    /* CAN2 queues (sensors) */
    configASSERT((pxResources->canSensorsComm.txQ = xQueueCreateStatic(CAN_SENSORS_TX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                                                       pxResources->ucCanSensorsTxQueueStorage,
                                                                       &pxResources->canSensorsTxQueueBuffer)) != NULL);

    configASSERT((pxResources->canSensorsComm.rxQ = xQueueCreateStatic(CAN_SENSORS_RX_QUEUE_LENGTH, sizeof(CanFrame_t),
                                                                       pxResources->ucCanSensorsRxQueueStorage,
                                                                       &pxResources->canSensorsRxQueueBuffer)) != NULL);

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

    /* Shared region array (all tasks use the same regions). This enables all tasks to
     * access the handles and resources. For more isolation, each task would define its
     * own regions with smaller scope.
     */

    /* Region 0: covers entire resources struct (queues, semaphores, etc.). */
    pxResources->sharedRegions[0] = (MemoryRegion_t){.pvBaseAddress = (void *)pxResources,
                                                     .ulLengthInBytes = sizeof(AppResources_t),
                                                     .ulParameters = portMPU_REGION_READ_WRITE};

    /* Region 1: covers task and communication handles */
    pxResources->sharedRegions[1] = (MemoryRegion_t){.pvBaseAddress = (void *)pxHandles,
                                                     .ulLengthInBytes = sizeof(AppHandles_t),
                                                     .ulParameters = portMPU_REGION_READ_WRITE};

    /* Regions 2-6 unused */
    pxResources->sharedRegions[2] = (MemoryRegion_t){NULL, 0, 0};
    pxResources->sharedRegions[3] = (MemoryRegion_t){NULL, 0, 0};
    pxResources->sharedRegions[4] = (MemoryRegion_t){NULL, 0, 0};
    pxResources->sharedRegions[5] = (MemoryRegion_t){NULL, 0, 0};

    /* All tasks are created as unprivileged restricted with shared regions.
     * Note: xTaskCreateRestricted still allocates TCB dynamically, only the stack is
     * static. For fully static allocation, kernel modifications would be needed.
     */

    /* Telemetry task */
    TaskParameters_t xTelemetryParams = {
        .pvTaskCode = vTelemetryTask,
        .pcName = "Telemetry",
        .usStackDepth = STACK_TELEMETRY,
        .pvParameters = (void *)pxResources,
        .uxPriority = PRIO_TELEMETRY,
        .puxStackBuffer = pxResources->telemetryStack,
        .xRegions = {pxResources->sharedRegions[0], pxResources->sharedRegions[1], pxResources->sharedRegions[2],
                     pxResources->sharedRegions[3], pxResources->sharedRegions[4], pxResources->sharedRegions[5]}};
    configASSERT(xTaskCreateRestricted(&xTelemetryParams, &pxHandles->telemetryTask) == pdPASS);

    /* Rules task */
    TaskParameters_t xRulesParams = {
        .pvTaskCode = vRulesTask,
        .pcName = "Rules",
        .usStackDepth = STACK_RULES,
        .pvParameters = (void *)pxResources,
        .uxPriority = PRIO_RULES,
        .puxStackBuffer = pxResources->rulesStack,
        .xRegions = {pxResources->sharedRegions[0], pxResources->sharedRegions[1], pxResources->sharedRegions[2],
                     pxResources->sharedRegions[3], pxResources->sharedRegions[4], pxResources->sharedRegions[5]}};
    configASSERT(xTaskCreateRestricted(&xRulesParams, &pxHandles->rulesTask) == pdPASS);

    /* Command RX task */
    TaskParameters_t xCmdRxParams = {
        .pvTaskCode = vCmdRxTask,
        .pcName = "CmdRx",
        .usStackDepth = STACK_CMD_RX,
        .pvParameters = (void *)pxResources,
        .uxPriority = PRIO_CMD_RX,
        .puxStackBuffer = pxResources->cmdRxStack,
        .xRegions = {pxResources->sharedRegions[0], pxResources->sharedRegions[1], pxResources->sharedRegions[2],
                     pxResources->sharedRegions[3], pxResources->sharedRegions[4], pxResources->sharedRegions[5]}};
    configASSERT(xTaskCreateRestricted(&xCmdRxParams, &pxHandles->cmdRxTask) == pdPASS);

    /* Motor FSM task */
    /* The motor task can remain unprivileged if it only builds CAN frames and pushes
     * them through queues, and the CAN ISR handles the privileged register access. If
     * the FSM needs more direct hardware access, make it privileged or add necessary
     * peripherals to the shared regions.
     */
    TaskParameters_t xMotorParams = {
        .pvTaskCode = vMotorFSMTask,
        .pcName = "MotorFSM",
        .usStackDepth = STACK_MOTOR_FSM,
        .pvParameters = (void *)pxResources,
        // .uxPriority = (PRIO_MOTOR_SM | portPRIVILEGE_BIT), /* Privileged task */
        .uxPriority = (PRIO_MOTOR_FSM),
        .puxStackBuffer = pxResources->motorStack,
        .xRegions = {pxResources->sharedRegions[0], pxResources->sharedRegions[1], pxResources->sharedRegions[2],
                     pxResources->sharedRegions[3], pxResources->sharedRegions[4], pxResources->sharedRegions[5]}};
    configASSERT(xTaskCreateRestricted(&xMotorParams, &pxHandles->motorFSMTask) == pdPASS);
}

// Tasks ///////////////////////////////////////////////////////////////////////////////

static portTASK_FUNCTION(vTelemetryTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppResources_t *pxResources = pxCtx->resources;
    AppHandles_t *pxHandles = pxCtx->handles;
    CanFrame_t txFrame = {.id = 0x123, .dlc = 8, .data = {0x54U, 0x4DU, 0x53U, 0x35U, 0x37U, 0x30U, 0x01U, 0x00U}};
    uint32_t seqBefore, seqAfter;

    // (void)memset(&txFrame, 0, sizeof(txFrame));

    __FOREVER__
    {
        // TODO: Update with the protection mechanism used for shared data.

        /* Gather protected data. */
        if (xSemaphoreTake(pxHandles->dataMutex, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            /* ... read shared config or sensor settings safely ... */
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
    }
}

static portTASK_FUNCTION(vRulesTask, pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppResources_t *pxResources = pxCtx->resources;
    AppHandles_t *pxHandles = pxCtx->handles;
    EventBits_t uxBits;
    CanFrame_t sensorFrame;
    uint32_t seqBefore, seqAfter;

    __FOREVER__
    {
        /* Wait for an event or poll periodically */
        uxBits = xEventGroupWaitBits(pxHandles->sysEvents, EVT_SENSOR_RULES_READY, pdTRUE, pdFALSE, pdMS_TO_TICKS(500));
        if ((uxBits & EVT_SENSOR_RULES_READY) != 0)
        {
            /* Consume the latest published sensor snapshot with sequence-lock retry. */
            do
            {
                seqBefore = pxResources->sensorSnapshot.sequence;
                sensorFrame = pxResources->sensorSnapshot.frame;
                seqAfter = pxResources->sensorSnapshot.sequence;
            } while ((seqBefore != seqAfter) || (seqBefore & 1U));

            /* ... apply rules with consistent snapshot ... */
            (void)sensorFrame;
        }

        /* run compliance checks periodically */
        vTaskDelay(pdMS_TO_TICKS(100));
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
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Static Internal Kernel Memory Allocation ////////////////////////////////////////////

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

/* USER CODE END */
