# 0 "app/App.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "app/App.cpp"
# 17 "app/App.cpp"
extern "C"
{



# 1 "include/FreeRTOS.h" 1
# 76 "include/FreeRTOS.h"
# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 1 3 4
# 145 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 3 4

# 145 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 214 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 3 4
typedef unsigned int size_t;
# 424 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 435 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 3 4
} max_align_t;






  typedef decltype(nullptr) nullptr_t;
# 77 "include/FreeRTOS.h" 2
# 91 "include/FreeRTOS.h"
# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stdint.h" 1 3 4
# 9 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stdint.h" 3 4
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 1 3 4
# 12 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 3 4
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 1 3 4







# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/features.h" 1 3 4
# 25 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/features.h" 3 4
extern "C" {


# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/_newlib_version.h" 1 3 4
# 29 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/features.h" 2 3 4
# 536 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/features.h" 3 4
}
# 9 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 2 3 4
# 37 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
extern "C" {



typedef signed char __int8_t;

typedef unsigned char __uint8_t;
# 55 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef short int __int16_t;

typedef short unsigned int __uint16_t;
# 77 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef long int __int32_t;

typedef long unsigned int __uint32_t;
# 103 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef long long int __int64_t;

typedef long long unsigned int __uint64_t;
# 134 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef signed char __int_least8_t;

typedef unsigned char __uint_least8_t;
# 160 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef short int __int_least16_t;

typedef short unsigned int __uint_least16_t;
# 182 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef long int __int_least32_t;

typedef long unsigned int __uint_least32_t;
# 200 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef long long int __int_least64_t;

typedef long long unsigned int __uint_least64_t;
# 214 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
typedef long long int __intmax_t;







typedef long long unsigned int __uintmax_t;







typedef int __intptr_t;

typedef unsigned int __uintptr_t;
# 247 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_default_types.h" 3 4
}
# 13 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 2 3 4
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_intsup.h" 1 3 4
# 35 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_intsup.h" 3 4
       
       
       
       
       
       
       
       
# 190 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_intsup.h" 3 4
       
       
       
       
       
       
       
       
# 14 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 2 3 4
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_stdint.h" 1 3 4
# 15 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_stdint.h" 3 4
extern "C" {




typedef __int8_t int8_t ;



typedef __uint8_t uint8_t ;







typedef __int16_t int16_t ;



typedef __uint16_t uint16_t ;







typedef __int32_t int32_t ;



typedef __uint32_t uint32_t ;







typedef __int64_t int64_t ;



typedef __uint64_t uint64_t ;






typedef __intmax_t intmax_t;




typedef __uintmax_t uintmax_t;




typedef __intptr_t intptr_t;




typedef __uintptr_t uintptr_t;




}
# 15 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 2 3 4


extern "C" {



typedef __int_least8_t int_least8_t;
typedef __uint_least8_t uint_least8_t;




typedef __int_least16_t int_least16_t;
typedef __uint_least16_t uint_least16_t;




typedef __int_least32_t int_least32_t;
typedef __uint_least32_t uint_least32_t;




typedef __int_least64_t int_least64_t;
typedef __uint_least64_t uint_least64_t;
# 51 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 3 4
  typedef int int_fast8_t;
  typedef unsigned int uint_fast8_t;
# 61 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 3 4
  typedef int int_fast16_t;
  typedef unsigned int uint_fast16_t;
# 71 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 3 4
  typedef int int_fast32_t;
  typedef unsigned int uint_fast32_t;
# 81 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 3 4
  typedef long long int int_fast64_t;
  typedef long long unsigned int uint_fast64_t;
# 463 "/opt/arm-none-eabi-be/arm-none-eabi/include/stdint.h" 3 4
}
# 10 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stdint.h" 2 3 4
# 92 "include/FreeRTOS.h" 2



# 94 "include/FreeRTOS.h"
extern "C" {



# 1 "include/FreeRTOSConfig.h" 1
# 99 "include/FreeRTOS.h" 2


# 1 "include/os_projdefs.h" 1
# 77 "include/os_projdefs.h"
typedef void (*TaskFunction_t)( void * );
# 102 "include/FreeRTOS.h" 2


# 1 "include/os_portable.h" 1
# 87 "include/os_portable.h"
# 1 "include/os_portmacro.h" 1
# 92 "include/os_portmacro.h"
typedef uint32_t StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;





    typedef uint32_t TickType_t;
# 116 "include/os_portmacro.h"
#pragma SWI_ALIAS(2)



extern void vPortEnterCritical( void );


#pragma SWI_ALIAS(3)



extern void vPortExitCritical( void );


#pragma SWI_ALIAS(5)



extern void vPortDisableInterrupts( void );



#pragma SWI_ALIAS(6)



extern void vPortEnableInterrupts( void );
# 151 "include/os_portmacro.h"
#pragma SWI_ALIAS(0)



extern void vPortYield( void );
# 170 "include/os_portmacro.h"
#pragma SWI_ALIAS(4)



extern void vPortTaskUsesFPU(void);





 unsigned long ulPortCountLeadingZeros( unsigned long ulBitmap );
# 304 "include/os_portmacro.h"
typedef struct MPU_REGION_REGISTERS
{
 unsigned ulRegionBaseAddress;
 unsigned ulRegionSize;
 unsigned ulRegionAttribute;
} xMPU_REGION_REGISTERS;



typedef struct MPU_SETTINGS
{
 xMPU_REGION_REGISTERS xRegion[ ( ( ( ( ( 12UL ) - 2 ) - ( 6UL - 1UL ) ) + 1 ) + 1 ) ];
} xMPU_SETTINGS;
# 88 "include/os_portable.h" 2
# 130 "include/os_portable.h"
extern "C" {


# 1 "include/os_mpu_wrappers.h" 1
# 134 "include/os_portable.h" 2
# 142 "include/os_portable.h"
 StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters, BaseType_t xRunPrivileged ) ;





typedef struct HeapRegion
{
 uint8_t *pucStartAddress;
 size_t xSizeInBytes;
} HeapRegion_t;
# 165 "include/os_portable.h"
void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions ) ;





void *pvPortMalloc( size_t xSize ) ;
void vPortFree( void *pv ) ;
void vPortInitialiseBlocks( void ) ;
size_t xPortGetFreeHeapSize( void ) ;
size_t xPortGetMinimumEverFreeHeapSize( void ) ;





BaseType_t xPortStartScheduler( void ) ;






void vPortEndScheduler( void ) ;
# 198 "include/os_portable.h"
 struct xMEMORY_REGION;
 void vPortStoreTaskMPUSettings( xMPU_SETTINGS *xMPUSettings, const struct xMEMORY_REGION * const xRegions, StackType_t *pxBottomOfStack, uint32_t ulStackDepth ) ;



}
# 105 "include/FreeRTOS.h" 2
# 874 "include/FreeRTOS.h"
struct xSTATIC_LIST_ITEM
{
 TickType_t xDummy1;
 void *pvDummy2[ 4 ];
};
typedef struct xSTATIC_LIST_ITEM StaticListItem_t;


struct xSTATIC_MINI_LIST_ITEM
{
 TickType_t xDummy1;
 void *pvDummy2[ 2 ];
};
typedef struct xSTATIC_MINI_LIST_ITEM StaticMiniListItem_t;


typedef struct xSTATIC_LIST
{
 UBaseType_t uxDummy1;
 void *pvDummy2;
 StaticMiniListItem_t xDummy3;
} StaticList_t;
# 910 "include/FreeRTOS.h"
typedef struct xSTATIC_TCB
{
 void *pxDummy1;

  xMPU_SETTINGS xDummy2;

 StaticListItem_t xDummy3[ 2 ];
 UBaseType_t uxDummy5;
 void *pxDummy6;
 uint8_t ucDummy7[ ( 16 ) ];







  UBaseType_t uxDummy10[ 2 ];


  UBaseType_t uxDummy12[ 2 ];





  void *pvDummy15[ 1 ];
# 945 "include/FreeRTOS.h"
  uint32_t ulDummy18;
  uint8_t ucDummy19;


  uint8_t uxDummy20;


} StaticTask_t;
# 968 "include/FreeRTOS.h"
typedef struct xSTATIC_QUEUE
{
 void *pvDummy1[ 3 ];

 union
 {
  void *pvDummy2;
  UBaseType_t uxDummy2;
 } u;

 StaticList_t xDummy3[ 2 ];
 UBaseType_t uxDummy4[ 3 ];
 uint8_t ucDummy5[ 2 ];


  uint8_t ucDummy6;







  UBaseType_t uxDummy8;
  uint8_t ucDummy9;


} StaticQueue_t;
typedef StaticQueue_t StaticSemaphore_t;
# 1012 "include/FreeRTOS.h"
typedef struct xSTATIC_EVENT_GROUP
{
 TickType_t xDummy1;
 StaticList_t xDummy2;


  UBaseType_t uxDummy3;



   uint8_t ucDummy4;


} StaticEventGroup_t;
# 1041 "include/FreeRTOS.h"
typedef struct xSTATIC_TIMER
{
 void *pvDummy1;
 StaticListItem_t xDummy2;
 TickType_t xDummy3;
 UBaseType_t uxDummy4;
 void *pvDummy5[ 2 ];

  UBaseType_t uxDummy6;



  uint8_t ucDummy7;


} StaticTimer_t;


}
# 23 "app/App.cpp" 2
# 1 "include/os_task.h" 1
# 78 "include/os_task.h"
# 1 "include/os_list.h" 1
# 138 "include/os_list.h"
extern "C" {
# 181 "include/os_list.h"
struct xLIST_ITEM
{

 TickType_t xItemValue;
 struct xLIST_ITEM * pxNext;
 struct xLIST_ITEM * pxPrevious;
 void * pvOwner;
 void * pvContainer;

};
typedef struct xLIST_ITEM ListItem_t;

struct xMINI_LIST_ITEM
{

 TickType_t xItemValue;
 struct xLIST_ITEM * pxNext;
 struct xLIST_ITEM * pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;




typedef struct xLIST
{

 UBaseType_t uxNumberOfItems;
 ListItem_t * pxIndex;
 MiniListItem_t xListEnd;

} List_t;
# 386 "include/os_list.h"
void vListInitialise( List_t * const pxList ) ;
# 397 "include/os_list.h"
void vListInitialiseItem( ListItem_t * const pxItem ) ;
# 410 "include/os_list.h"
void vListInsert( List_t * const pxList, ListItem_t * const pxNewListItem ) ;
# 431 "include/os_list.h"
void vListInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem ) ;
# 446 "include/os_list.h"
UBaseType_t uxListRemove( ListItem_t * const pxItemToRemove ) ;


}
# 79 "include/os_task.h" 2


extern "C" {
# 103 "include/os_task.h"
typedef void * TaskHandle_t;





typedef BaseType_t (*TaskHookFunction_t)( void * );


typedef enum
{
 eRunning = 0,
 eReady,
 eBlocked,
 eSuspended,
 eDeleted,
 eInvalid
} eTaskState;


typedef enum
{
 eNoAction = 0,
 eSetBits,
 eIncrement,
 eSetValueWithOverwrite,
 eSetValueWithoutOverwrite
} eNotifyAction;




typedef struct xTIME_OUT
{
 BaseType_t xOverflowCount;
 TickType_t xTimeOnEntering;
} TimeOut_t;




typedef struct xMEMORY_REGION
{
 void *pvBaseAddress;
 uint32_t ulLengthInBytes;
 uint32_t ulParameters;
} MemoryRegion_t;




typedef struct xTASK_PARAMETERS
{
 TaskFunction_t pvTaskCode;
 const char * const pcName;
 uint16_t usStackDepth;
 void *pvParameters;
 UBaseType_t uxPriority;
 StackType_t *puxStackBuffer;
 MemoryRegion_t xRegions[ ( ( ( ( 12UL ) - 2 ) - ( 6UL - 1UL ) ) + 1 ) ];
} TaskParameters_t;



typedef struct xTASK_STATUS
{
 TaskHandle_t xHandle;
 const char *pcTaskName;
 UBaseType_t xTaskNumber;
 eTaskState eCurrentState;
 UBaseType_t uxCurrentPriority;
 UBaseType_t uxBasePriority;
 uint32_t ulRunTimeCounter;
 StackType_t *pxStackBase;
 uint16_t usStackHighWaterMark;
} TaskStatus_t;


typedef enum
{
 eAbortSleep = 0,
 eStandardSleep,
 eNoTasksWaitingTimeout
} eSleepModeStatus;
# 360 "include/os_task.h"
 BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode,
       const char * const pcName,
       const uint16_t usStackDepth,
       void * const pvParameters,
       UBaseType_t uxPriority,
       TaskHandle_t * const pxCreatedTask ) ;
# 476 "include/os_task.h"
 TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
         const char * const pcName,
         const uint32_t ulStackDepth,
         void * const pvParameters,
         UBaseType_t uxPriority,
         StackType_t * const puxStackBuffer,
         StaticTask_t * const pxTaskBuffer ) ;
# 553 "include/os_task.h"
 BaseType_t MPU_xTaskCreateRestricted( const TaskParameters_t * const pxTaskDefinition, TaskHandle_t *pxCreatedTask ) ;
# 602 "include/os_task.h"
void MPU_vTaskAllocateMPURegions( TaskHandle_t xTask, const MemoryRegion_t * const pxRegions ) ;
# 643 "include/os_task.h"
void MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) ;
# 695 "include/os_task.h"
void MPU_vTaskDelay( const TickType_t xTicksToDelay ) ;
# 754 "include/os_task.h"
void MPU_vTaskDelayUntil( TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement ) ;
# 779 "include/os_task.h"
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) ;
# 826 "include/os_task.h"
UBaseType_t MPU_uxTaskPriorityGet( TaskHandle_t xTask ) ;







UBaseType_t uxTaskPriorityGetFromISR( TaskHandle_t xTask ) ;
# 852 "include/os_task.h"
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) ;
# 908 "include/os_task.h"
void MPU_vTaskGetInfo( TaskHandle_t xTask, TaskStatus_t *pxTaskStatus, BaseType_t xGetFreeStackSpace, eTaskState eState ) ;
# 950 "include/os_task.h"
void MPU_vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority ) ;
# 1001 "include/os_task.h"
void MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) ;
# 1050 "include/os_task.h"
void MPU_vTaskResume( TaskHandle_t xTaskToResume ) ;
# 1079 "include/os_task.h"
BaseType_t xTaskResumeFromISR( TaskHandle_t xTaskToResume ) ;
# 1112 "include/os_task.h"
void vTaskStartScheduler( void ) ;
# 1168 "include/os_task.h"
void vTaskEndScheduler( void ) ;
# 1219 "include/os_task.h"
void MPU_vTaskSuspendAll( void ) ;
# 1273 "include/os_task.h"
BaseType_t MPU_xTaskResumeAll( void ) ;
# 1288 "include/os_task.h"
TickType_t MPU_xTaskGetTickCount( void ) ;
# 1304 "include/os_task.h"
TickType_t xTaskGetTickCountFromISR( void ) ;
# 1318 "include/os_task.h"
UBaseType_t MPU_uxTaskGetNumberOfTasks( void ) ;
# 1331 "include/os_task.h"
char *MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) ;
# 1347 "include/os_task.h"
TaskHandle_t MPU_xTaskGetHandle( const char *pcNameToQuery ) ;
# 1368 "include/os_task.h"
UBaseType_t MPU_uxTaskGetStackHighWaterMark( TaskHandle_t xTask ) ;
# 1405 "include/os_task.h"
 void MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet, BaseType_t xIndex, void *pvValue ) ;
 void *MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery, BaseType_t xIndex ) ;
# 1421 "include/os_task.h"
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask, void *pvParameter ) ;
# 1430 "include/os_task.h"
TaskHandle_t MPU_xTaskGetIdleTaskHandle( void ) ;
# 1529 "include/os_task.h"
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime ) ;
# 1576 "include/os_task.h"
void MPU_vTaskList( char * pcWriteBuffer ) ;
# 1630 "include/os_task.h"
void MPU_vTaskGetRunTimeStats( char *pcWriteBuffer ) ;
# 1711 "include/os_task.h"
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction, uint32_t *pulPreviousNotificationValue ) ;
# 1802 "include/os_task.h"
BaseType_t xTaskGenericNotifyFromISR( TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction, uint32_t *pulPreviousNotificationValue, BaseType_t *pxHigherPriorityTaskWoken ) ;
# 1879 "include/os_task.h"
BaseType_t MPU_xTaskNotifyWait( uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, TickType_t xTicksToWait ) ;
# 1980 "include/os_task.h"
void vTaskNotifyGiveFromISR( TaskHandle_t xTaskToNotify, BaseType_t *pxHigherPriorityTaskWoken ) ;
# 2049 "include/os_task.h"
uint32_t MPU_ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait ) ;
# 2065 "include/os_task.h"
BaseType_t MPU_xTaskNotifyStateClear( TaskHandle_t xTask );
# 2086 "include/os_task.h"
BaseType_t xTaskIncrementTick( void ) ;
# 2119 "include/os_task.h"
void vTaskPlaceOnEventList( List_t * const pxEventList, const TickType_t xTicksToWait ) ;
void vTaskPlaceOnUnorderedEventList( List_t * pxEventList, const TickType_t xItemValue, const TickType_t xTicksToWait ) ;
# 2133 "include/os_task.h"
void vTaskPlaceOnEventListRestricted( List_t * const pxEventList, TickType_t xTicksToWait, const BaseType_t xWaitIndefinitely ) ;
# 2159 "include/os_task.h"
BaseType_t xTaskRemoveFromEventList( const List_t * const pxEventList ) ;
BaseType_t xTaskRemoveFromUnorderedEventList( ListItem_t * pxEventListItem, const TickType_t xItemValue ) ;
# 2170 "include/os_task.h"
void vTaskSwitchContext( void ) ;





TickType_t uxTaskResetEventItemValue( void ) ;




TaskHandle_t MPU_xTaskGetCurrentTaskHandle( void ) ;




void MPU_vTaskSetTimeOutState( TimeOut_t * const pxTimeOut ) ;





BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * const pxTimeOut, TickType_t * const pxTicksToWait ) ;





void vTaskMissedYield( void ) ;





BaseType_t MPU_xTaskGetSchedulerState( void ) ;





void vTaskPriorityInherit( TaskHandle_t const pxMutexHolder ) ;





BaseType_t xTaskPriorityDisinherit( TaskHandle_t const pxMutexHolder ) ;




UBaseType_t uxTaskGetTaskNumber( TaskHandle_t xTask ) ;





void vTaskSetTaskNumber( TaskHandle_t xTask, const UBaseType_t uxHandle ) ;
# 2237 "include/os_task.h"
void vTaskStepTick( const TickType_t xTicksToJump ) ;
# 2253 "include/os_task.h"
eSleepModeStatus eTaskConfirmSleepModeStatus( void ) ;





void *pvTaskIncrementMutexHeldCount( void ) ;


}
# 24 "app/App.cpp" 2
# 1 "include/os_timer.h" 1
# 80 "include/os_timer.h"
# 1 "include/os_list.h" 1
# 81 "include/os_timer.h" 2



extern "C" {
# 118 "include/os_timer.h"
typedef void * TimerHandle_t;




typedef void (*TimerCallbackFunction_t)( TimerHandle_t xTimer );





typedef void (*PendedFunction_t)( void *, uint32_t );
# 269 "include/os_timer.h"
 TimerHandle_t MPU_xTimerCreate( const char * const pcTimerName,
        const TickType_t xTimerPeriodInTicks,
        const UBaseType_t uxAutoReload,
        void * const pvTimerID,
        TimerCallbackFunction_t pxCallbackFunction ) ;
# 399 "include/os_timer.h"
 TimerHandle_t MPU_xTimerCreateStatic( const char * const pcTimerName,
          const TickType_t xTimerPeriodInTicks,
          const UBaseType_t uxAutoReload,
          void * const pvTimerID,
          TimerCallbackFunction_t pxCallbackFunction,
          StaticTimer_t *pxTimerBuffer ) ;
# 427 "include/os_timer.h"
void *MPU_pvTimerGetTimerID( const TimerHandle_t xTimer ) ;
# 448 "include/os_timer.h"
void MPU_vTimerSetTimerID( TimerHandle_t xTimer, void *pvNewID ) ;
# 485 "include/os_timer.h"
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) ;







TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( void ) ;
# 1228 "include/os_timer.h"
BaseType_t xTimerPendFunctionCallFromISR( PendedFunction_t xFunctionToPend, void *pvParameter1, uint32_t ulParameter2, BaseType_t *pxHigherPriorityTaskWoken ) ;
# 1262 "include/os_timer.h"
BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend, void *pvParameter1, uint32_t ulParameter2, TickType_t xTicksToWait ) ;
# 1273 "include/os_timer.h"
const char * MPU_pcTimerGetName( TimerHandle_t xTimer ) ;
# 1284 "include/os_timer.h"
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) ;
# 1299 "include/os_timer.h"
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) ;





BaseType_t xTimerCreateTimerTask( void ) ;
BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer, const BaseType_t xCommandID, const TickType_t xOptionalValue, BaseType_t * const pxHigherPriorityTaskWoken, const TickType_t xTicksToWait ) ;


}
# 25 "app/App.cpp" 2
# 1 "include/os_semphr.h" 1
# 77 "include/os_semphr.h"
# 1 "include/os_queue.h" 1
# 79 "include/os_queue.h"
extern "C" {
# 88 "include/os_queue.h"
typedef void * QueueHandle_t;






typedef void * QueueSetHandle_t;






typedef void * QueueSetMemberHandle_t;
# 692 "include/os_queue.h"
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition ) ;
# 821 "include/os_queue.h"
BaseType_t xQueuePeekFromISR( QueueHandle_t xQueue, void * const pvBuffer ) ;
# 1013 "include/os_queue.h"
BaseType_t MPU_xQueueGenericReceive( QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait, const BaseType_t xJustPeek ) ;
# 1028 "include/os_queue.h"
UBaseType_t MPU_uxQueueMessagesWaiting( const QueueHandle_t xQueue ) ;
# 1045 "include/os_queue.h"
UBaseType_t MPU_uxQueueSpacesAvailable( const QueueHandle_t xQueue ) ;
# 1059 "include/os_queue.h"
void MPU_vQueueDelete( QueueHandle_t xQueue ) ;
# 1440 "include/os_queue.h"
BaseType_t xQueueGenericSendFromISR( QueueHandle_t xQueue, const void * const pvItemToQueue, BaseType_t * const pxHigherPriorityTaskWoken, const BaseType_t xCopyPosition ) ;
BaseType_t xQueueGiveFromISR( QueueHandle_t xQueue, BaseType_t * const pxHigherPriorityTaskWoken ) ;
# 1530 "include/os_queue.h"
BaseType_t xQueueReceiveFromISR( QueueHandle_t xQueue, void * const pvBuffer, BaseType_t * const pxHigherPriorityTaskWoken ) ;





BaseType_t xQueueIsQueueEmptyFromISR( const QueueHandle_t xQueue ) ;
BaseType_t xQueueIsQueueFullFromISR( const QueueHandle_t xQueue ) ;
UBaseType_t uxQueueMessagesWaitingFromISR( const QueueHandle_t xQueue ) ;
# 1549 "include/os_queue.h"
BaseType_t xQueueCRSendFromISR( QueueHandle_t xQueue, const void *pvItemToQueue, BaseType_t xCoRoutinePreviouslyWoken );
BaseType_t xQueueCRReceiveFromISR( QueueHandle_t xQueue, void *pvBuffer, BaseType_t *pxTaskWoken );
BaseType_t xQueueCRSend( QueueHandle_t xQueue, const void *pvItemToQueue, TickType_t xTicksToWait );
BaseType_t xQueueCRReceive( QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait );






QueueHandle_t MPU_xQueueCreateMutex( const uint8_t ucQueueType ) ;
QueueHandle_t MPU_xQueueCreateMutexStatic( const uint8_t ucQueueType, StaticQueue_t *pxStaticQueue ) ;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( const UBaseType_t uxMaxCount, const UBaseType_t uxInitialCount ) ;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( const UBaseType_t uxMaxCount, const UBaseType_t uxInitialCount, StaticQueue_t *pxStaticQueue ) ;
void* MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) ;





BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex, TickType_t xTicksToWait ) ;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) ;
# 1639 "include/os_queue.h"
 QueueHandle_t MPU_xQueueGenericCreate( const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, const uint8_t ucQueueType ) ;
# 1648 "include/os_queue.h"
 QueueHandle_t MPU_xQueueGenericCreateStatic( const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, uint8_t *pucQueueStorage, StaticQueue_t *pxStaticQueue, const uint8_t ucQueueType ) ;
# 1699 "include/os_queue.h"
QueueSetHandle_t MPU_xQueueCreateSet( const UBaseType_t uxEventQueueLength ) ;
# 1723 "include/os_queue.h"
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet ) ;
# 1742 "include/os_queue.h"
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet ) ;
# 1778 "include/os_queue.h"
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet, const TickType_t xTicksToWait ) ;




QueueSetMemberHandle_t xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet ) ;


void vQueueWaitForMessageRestricted( QueueHandle_t xQueue, TickType_t xTicksToWait, const BaseType_t xWaitIndefinitely ) ;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue, BaseType_t xNewQueue ) ;
void vQueueSetQueueNumber( QueueHandle_t xQueue, UBaseType_t uxQueueNumber ) ;
UBaseType_t uxQueueGetQueueNumber( QueueHandle_t xQueue ) ;
uint8_t ucQueueGetQueueType( QueueHandle_t xQueue ) ;



}
# 78 "include/os_semphr.h" 2

typedef QueueHandle_t SemaphoreHandle_t;
# 26 "app/App.cpp" 2
# 1 "include/os_event_groups.h" 1
# 78 "include/os_event_groups.h"
# 1 "include/os_timer.h" 1
# 79 "include/os_event_groups.h" 2


extern "C" {
# 123 "include/os_event_groups.h"
typedef void * EventGroupHandle_t;
# 133 "include/os_event_groups.h"
typedef TickType_t EventBits_t;
# 188 "include/os_event_groups.h"
 EventGroupHandle_t MPU_xEventGroupCreate( void ) ;
# 241 "include/os_event_groups.h"
 EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t *pxEventGroupBuffer ) ;
# 336 "include/os_event_groups.h"
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait ) ;
# 393 "include/os_event_groups.h"
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear ) ;
# 449 "include/os_event_groups.h"
 BaseType_t xEventGroupClearBitsFromISR( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet ) ;
# 526 "include/os_event_groups.h"
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet ) ;
# 601 "include/os_event_groups.h"
 BaseType_t xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, BaseType_t *pxHigherPriorityTaskWoken ) ;
# 730 "include/os_event_groups.h"
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, const EventBits_t uxBitsToWaitFor, TickType_t xTicksToWait ) ;
# 766 "include/os_event_groups.h"
EventBits_t xEventGroupGetBitsFromISR( EventGroupHandle_t xEventGroup ) ;
# 780 "include/os_event_groups.h"
void MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) ;


void vEventGroupSetBitsCallback( void *pvEventGroup, const uint32_t ulBitsToSet ) ;
void vEventGroupClearBitsCallback( void *pvEventGroup, const uint32_t ulBitsToClear ) ;



 UBaseType_t uxEventGroupGetNumber( void* xEventGroup ) ;



}
# 27 "app/App.cpp" 2


# 1 "include/can.h" 1
# 52 "include/can.h"
# 1 "include/reg_can.h" 1
# 52 "include/reg_can.h"
# 1 "include/sys_common.h" 1
# 50 "include/sys_common.h"
# 1 "include/hal_stdtypes.h" 1
# 49 "include/hal_stdtypes.h"
# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stdbool.h" 1 3 4
# 50 "include/hal_stdtypes.h" 2







typedef uint64_t uint64;




typedef uint32_t uint32;




typedef uint16_t uint16;




typedef uint8_t uint8;





typedef bool boolean;







typedef int64_t sint64;




typedef int32_t sint32;




typedef int16_t sint16;




typedef int8_t sint8;




typedef float float32;




typedef double float64;




typedef uint8 Std_ReturnType;

typedef struct
{
    uint16 vendorID;
    uint16 moduleID;
    uint8 instanceID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
} Std_VersionInfoType;
# 135 "include/hal_stdtypes.h"
  typedef unsigned char StatusType;
# 51 "include/sys_common.h" 2


extern "C" {
# 63 "include/sys_common.h"
typedef boolean tBoolean;
# 75 "include/sys_common.h"
typedef enum loopBackType
{
    Digital_Lbk = 0U,
    Analog_Lbk = 1U
}loopBackType_t;
# 89 "include/sys_common.h"
typedef enum config_value_type
{
 InitialValue,
 CurrentValue
}config_value_type_t;
# 126 "include/sys_common.h"
}
# 53 "include/reg_can.h" 2
# 70 "include/reg_can.h"
typedef volatile struct canBase
{
    uint32 CTL;
    uint32 ES;
    uint32 EERC;
    uint32 BTR;
    uint32 INT;
    uint32 TEST;
    uint32 rsvd1;
    uint32 PERR;
    uint32 rsvd2[24];
    uint32 ABOTR;
    uint32 TXRQX;
    uint32 TXRQx[4U];
    uint32 NWDATX;
    uint32 NWDATx[4U];
    uint32 INTPNDX;
    uint32 INTPNDx[4U];
    uint32 MSGVALX;
    uint32 MSGVALx[4U];
    uint32 rsvd3;
    uint32 INTMUXx[4U];
    uint32 rsvd4[6];






    uint8 rsvd9;
    uint8 IF1CMD;
    uint8 IF1STAT;
    uint8 IF1NO;

    uint32 IF1MSK;
    uint32 IF1ARB;
    uint32 IF1MCTL;
    uint8 IF1DATx[8U];
    uint32 rsvd5[2];






    uint8 rsvd10;
    uint8 IF2CMD;
    uint8 IF2STAT;
    uint8 IF2NO;

    uint32 IF2MSK;
    uint32 IF2ARB;
    uint32 IF2MCTL;
    uint8 IF2DATx[8U];
    uint32 rsvd6[2];
    uint32 IF3OBS;
    uint32 IF3MSK;
    uint32 IF3ARB;
    uint32 IF3MCTL;
    uint8 IF3DATx[8U];
    uint32 rsvd7[2];
    uint32 IF3UEy[4U];
    uint32 rsvd8[28];
    uint32 TIOC;
    uint32 RIOC;
} canBASE_t;
# 53 "include/can.h" 2


extern "C" {
# 605 "include/can.h"
typedef enum canloopBackType
{
    Internal_Lbk = 0x00000010U,
    External_Lbk = 0x00000100U,
 Internal_Silent_Lbk = 0x00000018U
}canloopBackType_t;





typedef struct can_config_reg
{
    uint32 CONFIG_CTL;
    uint32 CONFIG_ES;
    uint32 CONFIG_BTR;
    uint32 CONFIG_TEST;
    uint32 CONFIG_ABOTR;
    uint32 CONFIG_INTMUX0;
    uint32 CONFIG_INTMUX1;
    uint32 CONFIG_INTMUX2;
    uint32 CONFIG_INTMUX3;
    uint32 CONFIG_TIOC;
    uint32 CONFIG_RIOC;
} can_config_reg_t;
# 743 "include/can.h"
void canInit(void);
uint32 canTransmit(canBASE_t *node, uint32 messageBox, const uint8 * data);
uint32 canGetData(canBASE_t *node, uint32 messageBox, uint8 * const data);
uint32 canSendRemoteFrame(canBASE_t *node, uint32 messageBox);
uint32 canFillMessageObjectData(canBASE_t *node, uint32 messageBox, const uint8 * data);
uint32 canIsTxMessagePending(canBASE_t *node, uint32 messageBox);
uint32 canIsRxMessageArrived(canBASE_t *node, uint32 messageBox);
uint32 canIsMessageBoxValid(canBASE_t *node, uint32 messageBox);
uint32 canGetLastError(canBASE_t *node);
uint32 canGetErrorLevel(canBASE_t *node);
void canEnableErrorNotification(canBASE_t *node);
void canDisableErrorNotification(canBASE_t *node);
void canEnableStatusChangeNotification(canBASE_t *node);
void canDisableStatusChangeNotification(canBASE_t *node);
void canEnableloopback(canBASE_t *node, canloopBackType_t Loopbacktype);
void canDisableloopback(canBASE_t *node);
void canIoSetDirection(canBASE_t *node,uint32 TxDir,uint32 RxDir);
void canIoSetPort(canBASE_t *node, uint32 TxValue, uint32 RxValue);
uint32 canIoTxGetBit(canBASE_t *node);
uint32 canIoRxGetBit(canBASE_t *node);
uint32 canGetID(canBASE_t *node, uint32 messageBox);
void canUpdateID(canBASE_t *node, uint32 messageBox, uint32 msgBoxArbitVal);
void can1GetConfigValue(can_config_reg_t *config_reg, config_value_type_t type);
# 781 "include/can.h"
void canErrorNotification(canBASE_t *node, uint32 notification);
# 797 "include/can.h"
void canStatusChangeNotification(canBASE_t *node, uint32 notification);
# 812 "include/can.h"
void canMessageNotification(canBASE_t *node, uint32 messageBox);






}
# 30 "app/App.cpp" 2
# 1 "include/gio.h" 1
# 46 "include/gio.h"
# 1 "include/reg_gio.h" 1
# 70 "include/reg_gio.h"
typedef volatile struct gioBase
{
    uint32 GCR0;
    uint32 rsvd;
    uint32 INTDET;
    uint32 POL;
    uint32 ENASET;
    uint32 ENACLR;
    uint32 LVLSET;
    uint32 LVLCLR;
    uint32 FLG;
    uint32 OFF1;
    uint32 OFF2;
    uint32 EMU1;
 uint32 EMU2;
} gioBASE_t;
# 96 "include/reg_gio.h"
typedef volatile struct gioPort
{
    uint32 DIR;
    uint32 DIN;
    uint32 DOUT;
    uint32 DSET;
    uint32 DCLR;
    uint32 PDR;
    uint32 PULDIS;
    uint32 PSL;
} gioPORT_t;
# 47 "include/gio.h" 2


extern "C" {





typedef struct gio_config_reg
{
    uint32 CONFIG_INTDET;
    uint32 CONFIG_POL;
    uint32 CONFIG_INTENASET;
    uint32 CONFIG_LVLSET;

 uint32 CONFIG_PORTADIR;
 uint32 CONFIG_PORTAPDR;
 uint32 CONFIG_PORTAPSL;
 uint32 CONFIG_PORTAPULDIS;

 uint32 CONFIG_PORTBDIR;
 uint32 CONFIG_PORTBPDR;
 uint32 CONFIG_PORTBPSL;
 uint32 CONFIG_PORTBPULDIS;
}gio_config_reg_t;
# 154 "include/gio.h"
void gioInit(void);
void gioSetDirection(gioPORT_t *port, uint32 dir);
void gioSetBit(gioPORT_t *port, uint32 bit, uint32 value);
void gioSetPort(gioPORT_t *port, uint32 value);
uint32 gioGetBit(gioPORT_t *port, uint32 bit);
uint32 gioGetPort(gioPORT_t *port);
void gioToggleBit(gioPORT_t *port, uint32 bit);
void gioEnableNotification(gioPORT_t *port, uint32 bit);
void gioDisableNotification(gioPORT_t *port, uint32 bit);
void gioNotification(gioPORT_t *port, uint32 bit);
void gioGetConfigValue(gio_config_reg_t *config_reg, config_value_type_t type);






}
# 31 "app/App.cpp" 2
# 1 "include/rti.h" 1
# 53 "include/rti.h"
# 1 "include/reg_rti.h" 1
# 70 "include/reg_rti.h"
typedef volatile struct rtiBase
{
    uint32 GCTRL;
    uint32 TBCTRL;
    uint32 CAPCTRL;
    uint32 COMPCTRL;
    struct
    {
        uint32 FRCx;
        uint32 UCx;
        uint32 CPUCx;
        uint32 rsvd1;
        uint32 CAFRCx;
        uint32 CAUCx;
        uint32 rsvd2[2U];
    } CNT[2U];


    struct
    {
        uint32 COMPx;
        uint32 UDCPx;
    } CMP[4U];




    uint32 TBLCOMP;
    uint32 TBHCOMP;
    uint32 rsvd3[2U];
    uint32 SETINTENA;
    uint32 CLEARINTENA;
    uint32 INTFLAG;
    uint32 rsvd4;
    uint32 DWDCTRL;
    uint32 DWDPRLD;
    uint32 WDSTATUS;
    uint32 WDKEY;
    uint32 DWDCNTR;
    uint32 WWDRXNCTRL;
    uint32 WWDSIZECTRL;
    uint32 INTCLRENABLE;
    uint32 COMP0CLR;
    uint32 COMP1CLR;
    uint32 COMP2CLR;
    uint32 COMP3CLR;
} rtiBASE_t;
# 54 "include/rti.h" 2


extern "C" {
# 187 "include/rti.h"
typedef enum dwdViolationTag
{
 NoTime_Violation = 0U,
 Time_Window_Violation = 1U,
 EndTime_Window_Violation = 2U,
 StartTime_Window_Violation = 3U,
 Key_Seq_Violation = 4U
}dwdViolation_t;







typedef enum dwdResetStatusTag
{
 No_Reset_Generated = 0U,
 Reset_Generated = 1U
}dwdResetStatus_t;







typedef enum dwwdReactionTag
{
 Generate_Reset = 0x00000005U,
 Generate_NMI = 0x0000000AU
}dwwdReaction_t;







typedef enum dwwdWindowSizeTag
{
 Size_100_Percent = 0x00000005U,
 Size_50_Percent = 0x00000050U,
 Size_25_Percent = 0x00000500U,
 Size_12_5_Percent = 0x00005000U,
 Size_6_25_Percent = 0x00050000U,
 Size_3_125_Percent = 0x00500000U
}dwwdWindowSize_t;





typedef struct rti_config_reg
{
    uint32 CONFIG_GCTRL;
    uint32 CONFIG_TBCTRL;
    uint32 CONFIG_CAPCTRL;
    uint32 CONFIG_COMPCTRL;
    uint32 CONFIG_UDCP0;
    uint32 CONFIG_UDCP1;
    uint32 CONFIG_UDCP2;
    uint32 CONFIG_UDCP3;
} rti_config_reg_t;
# 282 "include/rti.h"
void rtiInit(void);
void rtiInitPeriodicCompares(void);
void rtiStartCounter(uint32 counter);
void rtiStopCounter(uint32 counter);
uint32 rtiResetCounter(uint32 counter);
void rtiSetPeriod(uint32 compare, uint32 period);
uint32 rtiGetPeriod(uint32 compare);
uint32 rtiGetCurrentTick(uint32 compare);
void rtiEnableNotification(uint32 notification);
void rtiDisableNotification(uint32 notification);
void dwdInit(uint16 dwdPreload);
void dwwdInit(dwwdReaction_t Reaction, uint16 dwdPreload, dwwdWindowSize_t Window_Size);
uint32 dwwdGetCurrentDownCounter(void);
void dwdCounterEnable(void);
void dwdSetPreload(uint16 dwdPreload);
void dwdReset(void);
void dwdGenerateSysReset(void);
boolean IsdwdKeySequenceCorrect(void);
dwdResetStatus_t dwdGetStatus(void);
dwdViolation_t dwdGetViolationStatus(void);
void dwdClearFlag(void);
void rtiGetConfigValue(rti_config_reg_t *config_reg, config_value_type_t type);
# 317 "include/rti.h"
void rtiNotification(uint32 notification);






}
# 32 "app/App.cpp" 2
# 1 "include/sci.h" 1
# 47 "include/sci.h"
# 1 "include/reg_sci.h" 1
# 71 "include/reg_sci.h"
typedef volatile struct sciBase
{
    uint32 GCR0;
    uint32 GCR1;
    uint32 GCR2;
    uint32 SETINT;
    uint32 CLEARINT;
    uint32 SETINTLVL;
    uint32 CLEARINTLVL;
    uint32 FLR;
    uint32 INTVECT0;
    uint32 INTVECT1;
    uint32 FORMAT;
    uint32 BRS;
    uint32 ED;
    uint32 RD;
    uint32 TD;
    uint32 PIO0;
    uint32 PIO1;
    uint32 PIO2;
    uint32 PIO3;
    uint32 PIO4;
    uint32 PIO5;
    uint32 PIO6;
    uint32 PIO7;
    uint32 PIO8;
    uint32 rsdv2[12U];
    uint32 IODFTCTRL;
} sciBASE_t;
# 48 "include/sci.h" 2


extern "C" {
# 61 "include/sci.h"
enum sciIntFlags
{
    SCI_FE_INT = 0x04000000U,
    SCI_OE_INT = 0x02000000U,
    SCI_PE_INT = 0x01000000U,
    SCI_RX_INT = 0x00000200U,
    SCI_TX_INT = 0x00000100U,
    SCI_WAKE_INT = 0x00000002U,
    SCI_BREAK_INT = 0x00000001U
};
# 91 "include/sci.h"
enum sciPinSelect
{
    PIN_SCI_TX = 4U,
    PIN_SCI_RX = 2U
};



typedef struct sci_config_reg
{
    uint32 CONFIG_GCR0;
    uint32 CONFIG_GCR1;
    uint32 CONFIG_SETINT;
    uint32 CONFIG_SETINTLVL;
    uint32 CONFIG_FORMAT;
    uint32 CONFIG_BRS;
    uint32 CONFIG_PIO0;
    uint32 CONFIG_PIO1;
    uint32 CONFIG_PIO6;
    uint32 CONFIG_PIO7;
 uint32 CONFIG_PIO8;
} sci_config_reg_t;
# 167 "include/sci.h"
void sciInit(void);
void sciSetFunctional(sciBASE_t *sci, uint32 port);
void sciSetBaudrate(sciBASE_t *sci, uint32 baud);
uint32 sciIsTxReady(sciBASE_t *sci);
void sciSendByte(sciBASE_t *sci, uint8 byte);
void sciSend(sciBASE_t *sci, uint32 length, uint8 * data);
uint32 sciIsRxReady(sciBASE_t *sci);
uint32 sciIsIdleDetected(sciBASE_t *sci);
uint32 sciRxError(sciBASE_t *sci);
uint32 sciReceiveByte(sciBASE_t *sci);
void sciReceive(sciBASE_t *sci, uint32 length, uint8 * data);
void sciEnableNotification(sciBASE_t *sci, uint32 flags);
void sciDisableNotification(sciBASE_t *sci, uint32 flags);
void sciEnableLoopback(sciBASE_t *sci, loopBackType_t Loopbacktype);
void sciDisableLoopback(sciBASE_t *sci);
void sciEnterResetState(sciBASE_t *sci);
void sciExitResetState(sciBASE_t *sci);
void sciGetConfigValue(sci_config_reg_t *config_reg, config_value_type_t type);
# 194 "include/sci.h"
void sciNotification(sciBASE_t *sci, uint32 flags);





}
# 33 "app/App.cpp" 2
# 1 "include/spi.h" 1
# 46 "include/spi.h"
# 1 "include/reg_spi.h" 1
# 71 "include/reg_spi.h"
typedef volatile struct spiBase
{
    uint32 GCR0;
    uint32 GCR1;
    uint32 INT0;
    uint32 LVL;
    uint32 FLG;
    uint32 PC0;
    uint32 PC1;
    uint32 PC2;
    uint32 PC3;
    uint32 PC4;
    uint32 PC5;
    uint32 PC6;
    uint32 PC7;
    uint32 PC8;
    uint32 DAT0;
    uint32 DAT1;
    uint32 BUF;
    uint32 EMU;
    uint32 DELAY;
    uint32 DEF;
    uint32 FMT0;
    uint32 FMT1;
    uint32 FMT2;
    uint32 FMT3;
    uint32 INTVECT0;
    uint32 INTVECT1;
    uint32 RESERVED[51U];
    uint32 IOLPKTSTCR;
} spiBASE_t;
# 47 "include/spi.h" 2


extern "C" {






enum spiChipSelect
{
    SPI_CS_NONE = 0xFFU,
    SPI_CS_0 = 0xFEU,
    SPI_CS_1 = 0xFDU,
    SPI_CS_2 = 0xFBU,
    SPI_CS_3 = 0xF7U,
    SPI_CS_4 = 0xEFU,
    SPI_CS_5 = 0xDFU,
    SPI_CS_6 = 0xBFU,
    SPI_CS_7 = 0x7FU
};




enum spiPinSelect
{
    SPI_PIN_CS0 = 0U,
    SPI_PIN_CS1 = 1U,
    SPI_PIN_CS2 = 2U,
    SPI_PIN_CS3 = 3U,
    SPI_PIN_CS4 = 4U,
    SPI_PIN_CS5 = 5U,
    SPI_PIN_CS6 = 6U,
    SPI_PIN_CS7 = 7U,
    SPI_PIN_ENA = 8U,
    SPI_PIN_CLK = 9U,
    SPI_PIN_SIMO = 10U,
    SPI_PIN_SOMI = 11U,
    SPI_PIN_SIMO_1 = 17U,
    SPI_PIN_SIMO_2 = 18U,
    SPI_PIN_SIMO_3 = 19U,
    SPI_PIN_SIMO_4 = 20U,
    SPI_PIN_SIMO_5 = 21U,
    SPI_PIN_SIMO_6 = 22U,
    SPI_PIN_SIMO_7 = 23U,
    SPI_PIN_SOMI_1 = 25U,
    SPI_PIN_SOMI_2 = 26U,
    SPI_PIN_SOMI_3 = 27U,
    SPI_PIN_SOMI_4 = 28U,
    SPI_PIN_SOMI_5 = 29U,
    SPI_PIN_SOMI_6 = 30U,
    SPI_PIN_SOMI_7 = 31U
};




typedef enum dataformat
{
    SPI_FMT_0 = 0U,
    SPI_FMT_1 = 1U,
    SPI_FMT_2 = 2U,
    SPI_FMT_3 = 3U
}SPIDATAFMT_t;




typedef struct spiDAT1RegConfig
{
    boolean CS_HOLD;
    boolean WDEL;
    SPIDATAFMT_t DFSEL;
    uint8 CSNR;
}spiDAT1_t;




typedef enum SpiTxRxDataStatus
{
    SPI_READY = 0U,
    SPI_PENDING = 1U,
    SPI_COMPLETED = 2U
}SpiDataStatus_t;




typedef struct spi_config_reg
{
    uint32 CONFIG_GCR1;
    uint32 CONFIG_INT0;
    uint32 CONFIG_LVL;
    uint32 CONFIG_PC0;
    uint32 CONFIG_PC1;
    uint32 CONFIG_PC6;
    uint32 CONFIG_PC7;
    uint32 CONFIG_PC8;
    uint32 CONFIG_DELAY;
    uint32 CONFIG_FMT0;
    uint32 CONFIG_FMT1;
    uint32 CONFIG_FMT2;
    uint32 CONFIG_FMT3;
}spi_config_reg_t;
# 196 "include/spi.h"
void spiInit(void);
void spiSetFunctional(spiBASE_t *spi, uint32 port);
void spiEnableNotification(spiBASE_t *spi, uint32 flags);
void spiDisableNotification(spiBASE_t *spi, uint32 flags);
uint32 spiTransmitData(spiBASE_t *spi, spiDAT1_t *dataconfig_t, uint32 blocksize, uint16 * srcbuff);
void spiSendData(spiBASE_t *spi, spiDAT1_t *dataconfig_t, uint32 blocksize, uint16 * srcbuff);
uint32 spiReceiveData(spiBASE_t *spi, spiDAT1_t *dataconfig_t, uint32 blocksize, uint16 * destbuff);
void spiGetData(spiBASE_t *spi, spiDAT1_t *dataconfig_t, uint32 blocksize, uint16 * destbuff);
uint32 spiTransmitAndReceiveData(spiBASE_t *spi, spiDAT1_t *dataconfig_t, uint32 blocksize, uint16 * srcbuff, uint16 * destbuff);
void spiSendAndGetData(spiBASE_t *spi, spiDAT1_t *dataconfig_t, uint32 blocksize, uint16 * srcbuff, uint16 * destbuff);
void spiEnableLoopback(spiBASE_t *spi, loopBackType_t Loopbacktype);
void spiDisableLoopback(spiBASE_t *spi);
SpiDataStatus_t SpiTxStatus(spiBASE_t *spi);
SpiDataStatus_t SpiRxStatus(spiBASE_t *spi);
void spi4GetConfigValue(spi_config_reg_t *config_reg, config_value_type_t type);
# 221 "include/spi.h"
void spiNotification(spiBASE_t *spi, uint32 flags);
# 230 "include/spi.h"
void spiEndNotification(spiBASE_t *spi);






}
# 34 "app/App.cpp" 2
# 1 "include/ti_fee.h" 1
# 131 "include/ti_fee.h"
# 1 "include/fee_interface.h" 1
# 85 "include/fee_interface.h"
# 1 "include/ti_fee_cfg.h" 1
# 86 "include/fee_interface.h" 2
# 132 "include/ti_fee.h" 2
# 1 "include/ti_fee_types.h" 1
# 74 "include/ti_fee_types.h"
# 1 "include/Device_header.h" 1
# 63 "include/Device_header.h"
# 1 "include/Device_TMS570LS12.h" 1
# 105 "include/Device_TMS570LS12.h"
# 1 "include/F021.h" 1
# 41 "include/F021.h"
# 1 "include/Types.h" 1
# 35 "include/Types.h"
# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 1 3 4
# 36 "include/Types.h" 2
# 47 "include/Types.h"
# 1 "include/CGT.gcc.h" 1
# 48 "include/Types.h" 2
# 86 "include/Types.h"
typedef unsigned char boolean_t;




typedef enum
{
   Fapi_MasterCpu,
   Fapi_SlaveCpu0
} __attribute__((packed)) Fapi_CpuSelectorType;




typedef enum
{
   ARM7 = 0U,
   M3 = 1U,
   R4 = 2U,
   R4F = 3U,
   C28 = 4U,
   Undefined1 = 5U,
   Undefined2 = 6U,
   Undefined3 = 7U
} __attribute__((packed)) Fapi_CpuType;




typedef enum
{
   Family_FMC = 0x00,
   Family_L2FMC = 0x10,
   Family_Sonata = 0x20,
   Family_Stellaris = 0x30,
   Family_Future = 0x40
} __attribute__((packed)) Fapi_FamilyType;




typedef enum
{
   Fapi_Flash,
   Fapi_FlashEcc,
   Fapi_Otp,
   Fapi_OtpEcc,
   Fapi_Undefined
} __attribute__((packed)) Fapi_AddressMemoryType;




typedef struct
{
   uint32_t u32AsicId;
   uint8_t u8Revision;
   uint32_t u32LotNumber;
   uint16_t u16FlowCheck;
   uint16_t u16WaferNumber;
   uint16_t u16XCoordinate;
   uint16_t u16YCoordinate;
} __attribute__((packed)) Fapi_EngineeringRowType;

typedef struct
{
   uint32_t au32StatusWord[4];
} __attribute__((packed)) Fapi_FlashStatusWordType;




typedef enum
{
   Fapi_AutoEccGeneration,

   Fapi_DataOnly,
   Fapi_EccOnly,
   Fapi_DataAndEcc
} __attribute__((packed)) Fapi_FlashProgrammingCommandsType;




typedef enum
{
   Fapi_FlashBank0=0,
   Fapi_FlashBank1=1,
   Fapi_FlashBank2=2,
   Fapi_FlashBank3=3,
   Fapi_FlashBank4=4,
   Fapi_FlashBank5=5,
   Fapi_FlashBank6=6,
   Fapi_FlashBank7=7
} __attribute__((packed)) Fapi_FlashBankType;




typedef enum
{
    Fapi_FLEP=0,
    Fapi_FLEE=1,
    Fapi_FLES=2,
    Fapi_FLHV=3
} __attribute__((packed)) Fapi_FlashBankTechType;




typedef enum
{
   Fapi_FlashSector0,
   Fapi_FlashSector1,
   Fapi_FlashSector2,
   Fapi_FlashSector3,
   Fapi_FlashSector4,
   Fapi_FlashSector5,
   Fapi_FlashSector6,
   Fapi_FlashSector7,
   Fapi_FlashSector8,
   Fapi_FlashSector9,
   Fapi_FlashSector10,
   Fapi_FlashSector11,
   Fapi_FlashSector12,
   Fapi_FlashSector13,
   Fapi_FlashSector14,
   Fapi_FlashSector15,
   Fapi_FlashSector16,
   Fapi_FlashSector17,
   Fapi_FlashSector18,
   Fapi_FlashSector19,
   Fapi_FlashSector20,
   Fapi_FlashSector21,
   Fapi_FlashSector22,
   Fapi_FlashSector23,
   Fapi_FlashSector24,
   Fapi_FlashSector25,
   Fapi_FlashSector26,
   Fapi_FlashSector27,
   Fapi_FlashSector28,
   Fapi_FlashSector29,
   Fapi_FlashSector30,
   Fapi_FlashSector31,
   Fapi_FlashSector32,
   Fapi_FlashSector33,
   Fapi_FlashSector34,
   Fapi_FlashSector35,
   Fapi_FlashSector36,
   Fapi_FlashSector37,
   Fapi_FlashSector38,
   Fapi_FlashSector39,
   Fapi_FlashSector40,
   Fapi_FlashSector41,
   Fapi_FlashSector42,
   Fapi_FlashSector43,
   Fapi_FlashSector44,
   Fapi_FlashSector45,
   Fapi_FlashSector46,
   Fapi_FlashSector47,
   Fapi_FlashSector48,
   Fapi_FlashSector49,
   Fapi_FlashSector50,
   Fapi_FlashSector51,
   Fapi_FlashSector52,
   Fapi_FlashSector53,
   Fapi_FlashSector54,
   Fapi_FlashSector55,
   Fapi_FlashSector56,
   Fapi_FlashSector57,
   Fapi_FlashSector58,
   Fapi_FlashSector59,
   Fapi_FlashSector60,
   Fapi_FlashSector61,
   Fapi_FlashSector62,
   Fapi_FlashSector63
} __attribute__((packed)) Fapi_FlashSectorType;




typedef enum
{
   Fapi_ProgramData = 0x0002,
   Fapi_EraseSector = 0x0006,
   Fapi_EraseBank = 0x0008,
   Fapi_ValidateSector = 0x000E,
   Fapi_ClearStatus = 0x0010,
   Fapi_ProgramResume = 0x0014,
   Fapi_EraseResume = 0x0016,
   Fapi_ClearMore = 0x0018
} __attribute__((packed)) Fapi_FlashStateCommandsType;


typedef uint32_t Fapi_FlashStatusType;




typedef enum
{
   Fapi_NormalRead = 0x0,
   Fapi_RM0 = 0x1,
   Fapi_RM1 = 0x2
} __attribute__((packed)) Fapi_FlashReadMarginModeType;




typedef enum
{
   Fapi_Status_Success=0,
   Fapi_Status_FsmBusy,
   Fapi_Status_FsmReady,
   Fapi_Error_Fail,
   Fapi_Error_NullPointer,
   Fapi_Error_InvalidCommand,
   Fapi_Error_InvalidEccAddress,
   Fapi_Error_OtpChecksumMismatch,
   Fapi_Error_InvalidHclkValue,

   Fapi_Error_InvalidBank,
   Fapi_Error_InvalidAddress,
   Fapi_Error_InvalidReadMode,
   Fapi_Error_AsyncIncorrectDataBufferLength,
   Fapi_Error_AsyncIncorrectEccBufferLength,
   Fapi_Error_AsyncDataEccBufferLengthMismatch,

   Fapi_Error_FeatureNotAvailable
} __attribute__((packed)) Fapi_StatusType;
# 437 "include/Types.h"
typedef union
{
 volatile struct
   {
      uint32_t OtpVersion:16;
      uint32_t ChecksumLength:16;
      uint32_t OtpChecksum;
      uint32_t NumberOfSectors:16;
      uint32_t NumberOfBanks:16;
      uint32_t Package:16;
      uint32_t MemorySize:16;
      uint32_t AsicNumber:24;
      uint32_t SiliconRevision:8;
      uint32_t LotNumber;
      uint32_t Flowbits:16;
      uint32_t WaferNumber:16;
      uint32_t XCoordinate:16;
      uint32_t YCoordinate:16;
      uint32_t PSU:8;
      uint32_t ESU:8;
      uint32_t PVSU:8;
      uint32_t EVSU:8;
      uint32_t RVSU:8;
      uint32_t PVAcc:8;
      uint32_t Add_EXEZSU:4;
      uint32_t CVSU:12;
      uint32_t PH:8;
      uint32_t RH:8;
      uint32_t PVH:8;
      uint32_t PVH2:8;
      uint32_t EH:8;
      uint32_t Sequence:8;
      uint32_t VSTAT:4;
      uint32_t SmFrequency:12;
      uint32_t VHV_EStart:16;
      uint32_t VHV_EStep:16;
      uint32_t OtpReserved1:16;
      uint32_t MAX_PP:16;
      uint32_t MAX_EP:16;
      uint32_t PROG_PW:16;
      uint32_t ERA_PW;
      uint32_t VHV_P:16;
      uint32_t VHV_E:16;
      uint32_t VHV_PV:16;
      uint32_t VCG:8;
      uint32_t VINH:8;
      uint32_t VSL_P:8;
      uint32_t VWL_P:8;
      uint32_t VRead:8;
      uint32_t OtpReserved2:8;
      uint32_t ApiChecksum:32;
      uint32_t OtpReserved3:32;
      uint32_t OtpReserved4:32;
      uint32_t OtpReserved5:32;
      uint32_t OtpReserved6:32;
   } OTP_VALUE;
   volatile uint8_t au8OtpWord[0x60];
   volatile uint16_t au16OtpWord[0x30];
   volatile uint32_t au32OtpWord[0x18];
}Fapi_TiOtpBytesType;



typedef struct TI_OTP_TYPE
{
   Fapi_TiOtpBytesType aOtpBank[8];
}Fapi_TiOtpType;




typedef enum
{
   Alpha_Internal,
   Alpha,
   Beta_Internal,
   Beta,
   Production
} __attribute__((packed)) Fapi_ApiProductionStatusType;

typedef struct
{
   uint8_t u8ApiMajorVersion;
   uint8_t u8ApiMinorVersion;
   uint8_t u8ApiRevision;
   Fapi_ApiProductionStatusType oApiProductionStatus;
   uint32_t u32ApiBuildNumber;
   uint8_t u8ApiTechnologyType;
   uint8_t u8ApiTechnologyRevision;
   uint8_t u8ApiEndianness;
   uint32_t u32ApiCompilerVersion;
}Fapi_LibraryInfoType;

typedef struct
{
# 544 "include/Types.h"
   uint16_t u16Reserved;
   uint16_t u16NumberOfBanks;
   uint16_t u16DevicePackage;
   uint16_t u16DeviceMemorySize;
   uint32_t u32AsicId;
   uint32_t u32LotNumber;
   uint16_t u16FlowCheck;
   uint16_t u16WaferNumber;
   uint16_t u16WaferXCoordinate;
   uint16_t u16WaferYCoordinate;

}Fapi_DeviceInfoType;

typedef struct
{
    Fapi_FlashBankTechType oFlashBankTech;
    uint32_t u32NumberOfSectors;
    uint32_t u32BankStartAddress;
    uint16_t au16SectorSizes[16];
}Fapi_FlashBankSectorsType;
# 42 "include/F021.h" 2
# 1 "include/Helpers.h" 1
# 43 "include/F021.h" 2
# 1 "include/Constants.h" 1
# 44 "include/F021.h" 2
# 1 "include/Registers.h" 1
# 40 "include/Registers.h"
# 1 "include/Registers_FMC_BE.h" 1
# 759 "include/Registers_FMC_BE.h"
typedef volatile struct FMC_REGISTERS
{



   union FRDCNTL
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FRDCNTL_Reserved_31_12 :20;
         uint32_t RWAIT :4;
         uint32_t _FRDCNTL_Reserved_07_05 :3;
         uint32_t ASWSTEN :1;
         uint32_t _FRDCNTL_Reserved_03_01 :3;
         uint32_t ENPIPE :1;
      } FRDCNTRL_BITS;
   } FrdCntl;

   uint32_t _Reserved_04;




   union FEDACCTRL1
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FEDACCTRL1_Reserved_31_25 :7;
         uint32_t SUSP_IGNR :1;
         uint32_t _FEDACCTRL1_Reserved_23_20 :4;
         uint32_t EDACMODE :4;
         uint32_t _FEDACCTRL1_Reserved_15_11 :5;
         uint32_t EOFEN :1;
         uint32_t EZFEN :1;
         uint32_t EPEN :1;
         uint32_t _FEDACCTRL1_Reserved_07_06 :2;
         uint32_t EOCV :1;
         uint32_t EZCV :1;
         uint32_t EDACEN :4;
      } FEDACCTRL1_BITS;
   } FedAcCtrl1;




   union FEDACCTRL2
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FEDACCTRL2_Reserved_31_16 :16;
         uint32_t SEC_THRESHOLD :16;
      } FEDACCTRL2_BITS;
   } FedAcCtrl2;




   union FCOR_ERR_CNT
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FCOR_ERR_CNT_Reserved_31_16 :16;
         uint32_t FERRCNT :16;
      } FCOR_ERR_CNT_BITS;
   } FcorErrCnt;




   union FCOR_ERR_ADD
   {
      uint32_t u32Register;
      struct
      {
         uint32_t COR_ERR_ADD :29;
         uint32_t B_OFF :3;
      } FCOR_ERR_ADD_BITS;
   } FcorErrAdd;




   union FCOR_ERR_POS
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FCOR_ERR_POS_Reserved_31_10 :22;
         uint32_t BUS2 :1;
         uint32_t TYPE :1;
         uint32_t ERR_POS :8;
      } FCOR_ERR_POS_BITS;
   } FcorErrPos;




   union FEDACSTATUS
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FEDACSTATUS_Reserved_31_25 :7;
         uint32_t FSM_DONE :1;
         uint32_t _FEDACSTATUS_Reserved_23_20 :4;
         uint32_t COMB2_MAL_G :1;
         uint32_t ECC_B2_MAL_ERR :1;
         uint32_t B2_UNC_ERR :1;
         uint32_t B2_COR_ERR :1;
         uint32_t _FEDACSTATUS_Reserved_15_13 :3;
         uint32_t D_UNC_ERR :1;
         uint32_t ADD_TAG_ERR :1;
         uint32_t ADD_PAR_ERR :1;
         uint32_t _FEDACSTATUS_Reserved_9 :1;
         uint32_t B1_UNC_ERR :1;
         uint32_t _FEDACSTATUS_Reserved_7_4 :4;
         uint32_t D_COR_ERR :1;
         uint32_t ERR_ONE_FLG :1;
         uint32_t ERR_ZERO_FLG :1;
         uint32_t ERR_PRF_FLG :1;
      } FEDACSTATUS_BITS;
   } FedAcStatus;




   union FUNC_ERR_ADD
   {
      uint32_t u32Register;
      struct
      {
         uint32_t UNC_ERR_ADD :29;
         uint32_t B_OFF :3;
      } FUNC_ERR_ADD_BITS;
   } FuncErrAdd;




   union FEDACSDIS
   {
      uint32_t u32Register;
      struct
      {
         uint32_t BankID1_inverse :3;
         uint32_t _FEDACSDIS_Reserved_28 :1;
         uint32_t SectorID1_inverse :4;
         uint32_t BankID1 :3;
         uint32_t _FEDACSDIS_Reserved_20 :1;
         uint32_t SectorID1 :4;
         uint32_t BankID0_inverse :3;
         uint32_t _FEDACSDIS_Reserved_12 :1;
         uint32_t SectorID0_inverse :4;
         uint32_t BankID0 :3;
         uint32_t _FEDACSDIS_Reserved_04 :1;
         uint32_t SectorID0 :4;
      } FEDACSDIS_BITS;
   } FedAcsDis;




   union FPRIM_ADD_TAG
   {
      uint32_t u32Register;
      struct
      {
         uint32_t PRIM_ADD_TAG :28;
         uint32_t RET_ZERO :4;
      } FPRIM_ADD_TAG_BITS;
   } FprimAddTag;




   union FDUP_ADD_TAG
   {
      uint32_t u32Register;
      struct
      {
         uint32_t DUP_ADD_TAG :28;
         uint32_t RET_ZERO :4;
      } FDUP_ADD_TAG_BITS;
   } FdupAddTag;




   union FBPROT
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FBPROT_Reserved_31_01 :31;
         uint32_t PROTL1DIS :1;
      } FBPROT_BITS;
   } Fbprot;




   union FBSE
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FBSE_Reserved_31_16 :16;
         uint32_t BSE :16;
      } FBSE_BITS;
   } Fbse;




   union FBBUSY
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FBBUSY_Reserved_31_08 :24;
         uint32_t BUSY :8;
      } FBPROT_BITS;
   } Fbbusy;




   union FBAC
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FBAC_Reserved_31_24 :8;
         uint32_t OTPPROTDIS :8;
         uint32_t BAGP :8;
         uint32_t VREADS :8;
      } FBAC_BITS;
   } Fbac;




   union FBFALLBACK
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FBAC_Reserved_31_28 :4;
         uint32_t FSM_PWRSAV :4;
         uint32_t _FBAC_Reserved_23_20 :4;
         uint32_t REG_PWRSAV :4;
         uint32_t BANKPWR7 :2;
         uint32_t BANKPWR6 :2;
         uint32_t BANKPWR5 :2;
         uint32_t BANKPWR4 :2;
         uint32_t BANKPWR3 :2;
         uint32_t BANKPWR2 :2;
         uint32_t BANKPWR1 :2;
         uint32_t BANKPWR0 :2;
      } FBFALLBACK_BITS;
   } Fbfallback;




   union FBPRDY
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FBPRDY_Reserved_31_24 :8;
         uint32_t BANKBUSY :8;
         uint32_t PUMPRDY :1;
         uint32_t _FBPRDY_Reserved_14_08 :7;
         uint32_t BANKRDY :8;
      } FBPRDY_BITS;
   } Fbprdy;





   union FPAC1
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FPAC1_Reserved_31_27 :5;
         uint32_t PSBEEP :11;
         uint32_t _FPAC1_Reserved_15_01 :15;
         uint32_t PUMPPWR :1;
      } FPAC1_BITS;
   } Fpac1;




   union FPAC2
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FPAC2_Reserved_31_16 :16;
         uint32_t PAGP :16;
      } FPAC2_BITS;
   } Fpac2;




   union FMAC
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FMAC_Reserved_31_03 :29;
         uint32_t BANK :3;
      } FMAC_BITS;
   } Fmac;




   union FMSTAT
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FMSTAT_Reserved_31_18 :14;
         uint32_t RVSUSP :1;
         uint32_t RDVER :1;
         uint32_t RVF :1;
         uint32_t ILA :1;
         uint32_t DBT :1;
         uint32_t PGV :1;
         uint32_t PCV :1;
         uint32_t EV :1;
         uint32_t CV :1;
         uint32_t BUSY :1;
         uint32_t ERS :1;
         uint32_t PGM :1;
         uint32_t INVDAT :1;
         uint32_t CSTAT :1;
         uint32_t VOLSTAT :1;
         uint32_t ESUSP :1;
         uint32_t PSUSP :1;
         uint32_t SLOCK :1;
      } FMSTAT_BITS;
   } FmStat;




   union FEMU_DMSW
   {
      uint32_t u32Register;
   } FemuDmsw;




   union FEMU_DLSW
   {
      uint32_t u32Register;
   } FemuDlsw;




   union FEMU_ECC
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FEMU_ECC_Reserved_31_08 :24;
         uint32_t EMU_ECC :8;
      } FEMU_ECC_BITS;
   } FemuEcc;

   uint32_t _Reserved_64;




   union FEMU_ADDR
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FEMU_ADDR_Reserved_31_22 :10;
         uint32_t EMU_ADDR :22;
      } FEMU_ADDR_BITS;
   } FemuAddr;




   union FDIAGCTRL
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FDIAGCTRL_Reserved_31_25 :7;
         uint32_t DIAG_TRIG :1;
         uint32_t _FDIAGCTRL_Reserved_23_20 :4;
         uint32_t DIAG_EN_KEY :4;
         uint32_t _FDIAGCTRL_Reserved_15 :1;
         uint32_t DIAG_ECC_SEL :3;
         uint32_t _FDIAGCTRL_Reserved_11_10 :2;
         uint32_t DIAG_BUF_SEL :2;
         uint32_t _FDIAGCTRL_Reserved_07_03 :5;
         uint32_t DIAGMODE :3;
      } FDIAGCTRL_BITS;
   } FdiagCtrl;




   union FRAW_DATAH
   {
      uint32_t u32Register;
   } FrawDatah;




   union FRAW_DATAL
   {
      uint32_t u32Register;
   } FrawDatal;




   union FRAW_ECC
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FRAW_ECC_Reserved_31_08 :24;
         uint32_t RAW_ECC :8;
      } FRAW_ECC_BITS;
   } FrawEcc;




   union FPAR_OVR
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FPAR_OVR_Reserved_31_17 :15;
         uint32_t BNK_INV_PAR :1;
         uint32_t BUS_PAR_DIS :4;
         uint32_t PAR_OVR_KEY :3;
         uint32_t ADD_INV_PAR :1;
         uint32_t DAT_INV_PAR :8;
      } FPAR_OVR_BITS;
   } FparOvr;

   uint32_t _Reserved_80_BC[16];




   union FEDACSDIS2
   {
      uint32_t u32Register;
      struct
      {
         uint32_t BankID3_inverse :3;
         uint32_t _FEDACSDIS2_Reserved_28 :1;
         uint32_t SectorID3_inverse :4;
         uint32_t BankID3 :3;
         uint32_t _FEDACSDIS2_Reserved_20 :1;
         uint32_t SectorID3 :4;
         uint32_t BankID2_inverse :3;
         uint32_t _FEDACSDIS2_Reserved_12 :1;
         uint32_t SectorID2_inverse :4;
         uint32_t BankID2 :3;
         uint32_t _FEDACSDIS2_Reserved_4 :1;
         uint32_t SectorID2 :4;
      } FEDACSDIS2_BITS;
   } FedAcsDis2;

   uint32_t _Reserved_C4_10C[19];





   union FADDR
   {
      uint32_t u32Register;
   } Faddr;

   uint32_t _Reserved_114_118[2];

    union FTCTRL
      {
         uint32_t u32Register;
         struct
         {
            uint32_t Reserved_31_25 :7;
            uint32_t AutoCalc_EN :1;
            uint32_t Reserved_23_17 :7;
   uint32_t WKData_Blk_Clr :1;
   uint32_t Reserved_23_02 :14;
   uint32_t Test_EN :1;
   uint32_t Reserved_00 :1;
         } FTCTRL_BITS;
      } Ftctrl;





   union FWPWRITE0
   {
      uint32_t u32Register;
   } Fwpwrite0;




   union FWPWRITE1
   {
      uint32_t u32Register;
   } Fwpwrite1;




   union FWPWRITE2
   {
      uint32_t u32Register;
   } Fwpwrite2;




   union FWPWRITE3
   {
      uint32_t u32Register;
   } Fwpwrite3;




   union FWPWRITE4
   {
      uint32_t u32Register;
   } Fwpwrite4;




   union FWPWRITE5
   {
      uint32_t u32Register;
   } Fwpwrite5;




   union FWPWRITE6
   {
      uint32_t u32Register;
   } Fwpwrite6;




   union FWPWRITE7
   {
      uint32_t u32Register;
   } Fwpwrite7;




   union FWPWRITE_ECC
   {
      uint32_t u32Register;
      struct
      {
         uint32_t WPDATA_287_256;
      } FWPWRITE_ECC_BITS;
      struct
      {
         uint32_t u8Bytes07_00:8;
         uint32_t u8Bytes15_08:8;
         uint32_t u8Bytes23_16:8;
         uint32_t u8Bytes31_24:8;
      } FWPWRITE_ECC_BYTES;
   } FwpwriteEcc;

   uint32_t _Reserved_144_208[50];




   union FSM_COMMAND
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FSM_COMMAND_Reserved_31_06 :26;
         uint32_t FSMCMD :6;
      } FSM_COMMAND_BITS;
   } FsmCommand;

   uint32_t _Reserved_210_284[30];




   union FSM_WR_ENA
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FSM_WR_ENA_Reserved_31_03 :29;
         uint32_t WR_ENA :3;
      } FSM_WR_ENA_BITS;
   } FsmWrEna;

   uint32_t _Reserved_28C_2A0[6];




   union FSM_SECTOR
   {
      uint32_t u32Register;
      struct
      {
         uint32_t SECT_ERASED :16;
         uint32_t _FSM_SECTOR_Reserved_15_0 :16;
      } FSM_SECTOR_BITS;
   } FsmSector;

   uint32_t _Reserved_2A8_2B0[3];




   union FSM_EXECUTE
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _FSM_EXECUTE_Reserved_31_20 :12;
         uint32_t SUSPEND_NOW :4;
         uint32_t _FSM_EXECUTE_Reserved_15_05 :11;
         uint32_t FSMEXECUTE :5;
      } FSM_EXECUTE_BITS;
   } FsmExecute;




   union EEPROM_CONFIG
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _EEPROM_CONFIG_Reserved_31_20 :12;
         uint32_t EWAIT :4;
         uint32_t _EEPROM_CONFIG_Reserved_15_09 :7;
         uint32_t AUTOSUSP_EN :1;
         uint32_t AUTOSTART_GRACE :8;
      } EEPROM_CONFIG_BITS;
   } EepromConfig;

   uint32_t _Reserved_2BC;




   union FSM_SECTOR_1
   {
      uint32_t u32Register;
   } FsmSector1;




   union FSM_SECTOR_2
   {
      uint32_t u32Register;
   } FsmSector2;

   uint32_t _Reserved_2C8_304[16];




   union EE_CTRL1
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _EE_CTRL1_Reserved_31_20 :12;
         uint32_t EE_EDACMODE :4;
         uint32_t _EE_CTRL1_Reserved_15_11 :5;
         uint32_t EE_EOFEN :1;
         uint32_t EE_EZFEN :1;
         uint32_t EE_PEN :1;
         uint32_t _EE_CTRL1_Reserved_07_06 :2;
         uint32_t EE_ALL1_OK :1;
         uint32_t EE_ALL0_OK :1;
         uint32_t EE_EDACEN :4;
      } EE_CTRL1_BITS;
   } EeCtrl1;




   union EE_CTRL2
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _EE_CTRL2_Reserved_31_16 :16;
         uint32_t EE_SEC_THRESHOLD :16;
      } EE_CTRL2_BITS;
   } EeCtrl2;




   union EE_COR_ERR_CNT
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _EE_COR_ERR_CNT_Reserved_31_16 :16;
         uint32_t EE_ERRCNT :16;
      } EE_COR_ERR_CNT_BITS;
   } EeCorErrCnt;




   union EE_COR_ERR_ADD
   {
      uint32_t u32Register;
      struct
      {
         uint32_t COR_ERR_ADD :29;
         uint32_t B_OFF :3;
      } EE_COR_ERR_ADD_BITS;
   } EeCorErrAdd;




   union EE_COR_ERR_POS
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _EE_COR_ERR_POS_Reserved_31_9 :22;
         uint32_t TYPE :1;
         uint32_t EE_ERR_POS :8;
      } EE_COR_ERR_POS_BITS;
   } EeCorErrPos;




   union EE_STATUS
   {
      uint32_t u32Register;
      struct
      {
         uint32_t _EE_STATUS_Reserved_31_13 :19;
         uint32_t EE_D_UNC_ERR :1;
         uint32_t _EE_STATUS_Reserved_11_9 :3;
         uint32_t EE_UNC_ERR :1;
         uint32_t _EE_STATUS_Reserved_7 :1;
         uint32_t EE_CMG :1;
         uint32_t _EE_STATUS_Reserved_5 :1;
         uint32_t EE_CME :1;
         uint32_t EE_D_COR_ERR :1;
         uint32_t EE_ERR_ONE_FLG :1;
         uint32_t EE_ERR_ZERO_FLG :1;
         uint32_t EE_ERR_PRF_FLG :1;
      } EE_STATUS_BITS;
   } EeStatus;




   union EE_UNC_ERR_ADD
   {
      uint32_t u32Register;
      struct
      {
         uint32_t UNC_ERR_ADD :29;
         uint32_t B_OFF :3;
      } EE_UNC_ERR_ADD_BITS;
   } EeUncErrAdd;

   uint32_t _Reserved_324_3FC[55];




   union FCFG_BANK_1
   {
      uint32_t u32Register;
      struct
      {
         uint32_t EE_BANK_WIDTH :12;
         uint32_t _FCFG_BANK_Reserved_19_16 :4;
         uint32_t MAIN_BANK_WIDTH :12;
         uint32_t _FCFG_BANK_Reserved_3_0 :4;
      } FCFG_BANK_BITS;
   } FcfgBank;

} Fapi_FmcRegistersType;
# 41 "include/Registers.h" 2
# 66 "include/Registers.h"
typedef Fapi_FmcRegistersType* pFapi_FmcRegistersType;


typedef volatile uint8_t FwpWriteByteAccessorType;

typedef volatile uint32_t FwpWriteDWordAccessorType;
# 45 "include/F021.h" 2
# 1 "include/FapiFunctions.h" 1
# 31 "include/FapiFunctions.h"
extern "C" {





extern Fapi_StatusType Fapi_enableMainBankSectors(uint16_t u16SectorsEnables);

extern Fapi_StatusType Fapi_enableEepromBankSectors(
                                                    uint32_t u32SectorsEnables_31_0,
                                                    uint32_t u32SectorsEnables_63_32
                                                   );

extern Fapi_StatusType Fapi_enableFsmDoneEvent(void);

extern Fapi_StatusType Fapi_disableFsmDoneEvent(void);

extern Fapi_StatusType Fapi_initializeFlashBanks(uint32_t u32HclkFrequency);

extern Fapi_StatusType Fapi_setActiveFlashBank(Fapi_FlashBankType oNewFlashBank);

extern Fapi_StatusType Fapi_enableBanksForOtpWrite(uint8_t u8Banks);

extern Fapi_StatusType Fapi_disableBanksForOtpWrite(void);
# 64 "include/FapiFunctions.h"
extern void Fapi_flushPipeline(void);

extern uint32_t* Fapi_remapEccAddress(uint32_t u32EccAddress);

extern uint32_t Fapi_remapMainAddress(uint32_t u32MainAddress);

extern boolean_t Fapi_isAddressEcc(uint32_t u32Address);

extern boolean_t Fapi_isAddressEEPROM(uint32_t u32Address);


extern Fapi_StatusType Fapi_issueAsyncCommandWithAddress(
                                                         Fapi_FlashStateCommandsType oCommand,
                                                         uint32_t* pu32StartAddress
                                                        );

extern Fapi_StatusType Fapi_issueAsyncCommand(Fapi_FlashStateCommandsType oCommand);


extern Fapi_LibraryInfoType Fapi_getLibraryInfo(void);
extern Fapi_DeviceInfoType Fapi_getDeviceInfo(void);
extern Fapi_StatusType Fapi_getBankSectors(
                                           Fapi_FlashBankType oBank,
                                           Fapi_FlashBankSectorsType* poFlashBankSectors
                                          );
extern uint32_t Fapi_getNumberOfBankSectors( uint32_t u32Bank);

extern Fapi_StatusType Fapi_doBlankCheck(
                                         uint32_t* pu32StartAddress,
                                         uint32_t u32Length,
                                         Fapi_FlashStatusWordType* poFlashStatusWord
                                        );

extern Fapi_StatusType Fapi_doMarginRead(
                                         uint32_t* pu32StartAddress,
                                         uint32_t* pu32ReadBuffer,
                                         uint32_t u32Length,
                                         Fapi_FlashReadMarginModeType oReadMode
                                        );

extern Fapi_StatusType Fapi_doVerify(
                                     uint32_t* pu32StartAddress,
                                     uint32_t u32Length,
                                     uint32_t* pu32CheckValueBuffer,
                                     Fapi_FlashStatusWordType* poFlashStatusWord
                                    );

extern uint32_t Fapi_calculatePsa(
                                uint32_t* pu32StartAddress,
                                uint32_t u32Length,
                                uint32_t u32PsaSeed,
                                Fapi_FlashReadMarginModeType oReadMode
                               );

extern Fapi_StatusType Fapi_doPsaVerify(
                                        uint32_t* pu32StartAddress,
                                        uint32_t u32Length,
                                        uint32_t u32PsaValue,
                                        Fapi_FlashStatusWordType* poFlashStatusWord
                                       );

extern Fapi_StatusType Fapi_doBlankCheckByByte(
                                               uint8_t* pu8StartAddress,
                                               uint32_t u32Length,
                                               Fapi_FlashStatusWordType* poFlashStatusWord
                                              );

extern Fapi_StatusType Fapi_doMarginReadByByte(
                                               uint8_t* pu8StartAddress,
                                               uint8_t* pu8ReadBuffer,
                                               uint32_t u32Length,
                                               Fapi_FlashReadMarginModeType oReadMode
                                              );

extern Fapi_StatusType Fapi_doVerifyByByte(
                                           uint8_t* pu8StartAddress,
                                           uint32_t u32Length,
                                           uint8_t* pu8CheckValueBuffer,
                                           Fapi_FlashStatusWordType* poFlashStatusWord
                                          );


extern Fapi_StatusType Fapi_issueProgrammingCommand(
                                                    uint32_t* pu32StartAddress,
                                                    uint8_t* pu8DataBuffer,
                                                    uint8_t u8DataBufferSizeInBytes,
                                                    uint8_t* pu8EccBuffer,
                                                    uint8_t u8EccBufferSizeInBytes,
                                                    Fapi_FlashProgrammingCommandsType oMode
                                                   );

extern Fapi_StatusType Fapi_issueProgrammingCommandForEccAddresses(
                                                                   uint32_t* pu32StartAddress,
                                                                   uint8_t* pu8EccBuffer,
                                                                   uint8_t u8EccBufferSizeInBytes
                                                                  );


extern Fapi_StatusType Fapi_waitDelay(volatile uint32_t u32WaitDelay);

extern uint32_t Fapi_calculateFletcherChecksum(
                                             uint32_t u32Address,
                                             uint32_t u32Length
                                            );

extern uint8_t Fapi_calculateEcc(
                               uint32_t u32Address,
                               uint64_t u64Data
                              );


extern Fapi_StatusType Fapi_serviceWatchdogTimer(void);


}
# 46 "include/F021.h" 2
# 1 "include/Compatibility.h" 1
# 47 "include/F021.h" 2
# 106 "include/Device_TMS570LS12.h" 2

# 1 "include/Device_types.h" 1
# 66 "include/Device_types.h"
typedef enum
{
   Device_ErrorHandlingNone,
   Device_ErrorHandlingParity,
   Device_ErrorHandlingEcc
} Device_FlashErrorCorrectionProcessType;


typedef enum
{
 Device_CoreNone,
 Device_Arm7,
 Device_CortexR4,
 Device_CortexM3
}Device_ArmCoreType;


typedef struct
{
   Fapi_FlashSectorType Device_Sector;
   uint32 Device_SectorStartAddress;
   uint32 Device_SectorLength;
   uint32 Device_MaxWriteCycles;
   uint32 Device_EccAddress;
   uint32 Device_EccLength;
} Device_SectorType;


typedef struct
{
   Fapi_FmcRegistersType * Device_ControlRegister;
   Fapi_FlashBankType Device_Core;
   Device_SectorType Device_SectorInfo[4U];
} Device_BankType;


typedef struct
{
   uint8 Device_DeviceName[12];
   uint32 Device_EngineeringId;
   Device_FlashErrorCorrectionProcessType Device_FlashErrorHandlingProcessInfo;
   Device_ArmCoreType Device_MasterCore;
   boolean Device_SupportsInterrupts;
   uint32 Device_NominalWriteTime;
   uint32 Device_MaximumWriteTime;
   Device_BankType Device_BankInfo[1U];
} Device_FlashType;
# 108 "include/Device_TMS570LS12.h" 2
# 64 "include/Device_header.h" 2
# 75 "include/ti_fee_types.h" 2
# 93 "include/ti_fee_types.h"
typedef union
{
 uint16 Fee_u16StatusWord;

 struct
 {


  uint16 Reserved: 5U;


  uint16 Erase:1U;


  uint16 ReadSync:1U;


  uint16 ProgramFailed:1U;


  uint16 Read:1U;


  uint16 WriteSync:1U;


  uint16 WriteAsync:1U;


  uint16 EraseImmediate:1U;


  uint16 InvalidateBlock:1U;


  uint16 Copy:1U;


  uint16 Initialized:1U;


  uint16 SingleBitError:1U;
 }Fee_StatusWordType_ST;
# 178 "include/ti_fee_types.h"
}TI_Fee_StatusWordType_UN;

typedef enum
{
   UNINIT,
   IDLE,

   BUSY,
   BUSY_INTERNAL
}TI_FeeModuleStatusType;

typedef enum
{
    JOB_OK,
    JOB_FAILED,
    JOB_PENDING,
    JOB_CANCELLED,
    BLOCK_INCONSISTENT,
    BLOCK_INVALID
}TI_FeeJobResultType;
# 133 "include/ti_fee.h" 2
# 184 "include/ti_fee.h"
typedef enum
{
   TI_FEE_OK = 0U,
   TI_FEE_ERROR = 1U
} TI_Fee_StatusType;


typedef enum
{
   VsState_Invalid=1U,
   VsState_Empty=2U,
   VsState_Copy=3U,
   VsState_Active=4U,
   VsState_ReadyForErase=5U
}VirtualSectorStatesType;


typedef enum
{
   Block_StartProg=1U,
   Block_Valid=2U,
   Block_Invalid=3U
}BlockStatesType;


typedef enum
{
  Error_Nil=0U,
  Error_TwoActiveVS=1U,
 Error_TwoCopyVS=2U,
 Error_SetupStateMachine=3U,
 Error_CopyButNoActiveVS=4U,
 Error_NoActiveVS=5U,
 Error_BlockInvalid=6U,
 Error_NullDataPtr=7U,
 Error_NoFreeVS=8U,
 Error_InvalidVirtualSectorParameter=9U,
 Error_ExceedSectorOnBank=10U,
 Error_EraseVS=11U,
 Error_BlockOffsetGtBlockSize=12U,
 Error_LengthParam=13U,
 Error_FeeUninit=14U,
 Error_Suspend=15U,
 Error_InvalidBlockIndex=16U,
 Error_NoErase=17U,
 Error_CurrentAddress=18U,
 Error_Exceed_No_Of_DataSets=19U
}TI_Fee_ErrorCodeType;

typedef enum
{
 Suspend_Erase=0U,
 Resume_Erase
}TI_Fee_EraseCommandType;


typedef enum
{
   CHAMPION = 0U,
   ARCHER = 1U
} TI_Fee_DeviceType;

typedef uint32 TI_Fee_AddressType;

typedef uint32 TI_Fee_LengthType;

typedef TI_Fee_ErrorCodeType Fee_ErrorCodeType;
# 259 "include/ti_fee.h"
typedef struct
{
   uint16 FeeVirtualSectorNumber;

   uint16 FeeFlashBank;



   Fapi_FlashSectorType FeeStartSector;
   Fapi_FlashSectorType FeeEndSector;







} Fee_VirtualSectorConfigType;


typedef struct
{
   uint16 FeeBlockNumber;

   uint16 FeeBlockSize;


   boolean FeeImmediateData;

   uint32 FeeNumberOfWriteCycles;



   uint8 FeeDeviceIndex;

   uint8 FeeNumberOfDataSets;

   uint8 FeeEEPNumber;
} Fee_BlockConfigType;



typedef struct
{
  TI_Fee_AddressType Fee_oFlashNextAddress;
 TI_Fee_AddressType Fee_oCopyCurrentAddress;

 TI_Fee_AddressType Fee_oCopyNextAddress;

 TI_Fee_AddressType Fee_u32nextwriteaddress;

 TI_Fee_AddressType Fee_oVirtualSectorStartAddress;
 TI_Fee_AddressType Fee_oVirtualSectorEndAddress;
 TI_Fee_AddressType Fee_oCopyVirtualSectorAddress;
 TI_Fee_AddressType Fee_oCurrentStartAddress;
 TI_Fee_AddressType Fee_oCurrentBlockHeader;

 TI_Fee_AddressType Fee_oWriteAddress;
 TI_Fee_AddressType Fee_oCopyWriteAddress;
 TI_Fee_AddressType Fee_oActiveVirtualSectorAddress;
 TI_Fee_AddressType Fee_oBlankFailAddress;
 TI_Fee_AddressType Fee_oActiveVirtualSectorStartAddress;
 TI_Fee_AddressType Fee_oActiveVirtualSectorEndAddress;
 TI_Fee_AddressType Fee_oCopyVirtualSectorStartAddress;
 TI_Fee_AddressType Fee_oCopyVirtualSectorEndAddress;
 TI_Fee_AddressType Fee_u32nextActiveVSwriteaddress;
 TI_Fee_AddressType Fee_u32nextCopyVSwriteaddress;
 uint16 Fee_u16CopyBlockSize;

 uint8 Fee_u8VirtualSectorStart;
 uint8 Fee_u8VirtualSectorEnd;
 uint32 Fee_au32VirtualSectorStateValue[16U >> 2U];


 uint8 Fee_au8VirtualSectorState[2U];

 uint32 Fee_au32VirtualSectorEraseCount[2U];


 uint16 Fee_au16BlockOffset[1U];
 uint32 Fee_au32BlockHeader[24U >> 2U];
 uint8 Fee_au8BlockCopyStatus[1U];
 uint8 Fee_u8InternalVirtualSectorStart;
    uint8 Fee_u8InternalVirtualSectorEnd;
 TI_FeeModuleStatusType Fee_ModuleState;
 TI_FeeJobResultType Fee_u16JobResult;
 TI_Fee_StatusType Fee_oStatus;
 TI_Fee_ErrorCodeType Fee_Error;
 uint16 Fee_u16CopyBlockNumber;
 uint16 Fee_u16BlockIndex;
 uint16 Fee_u16BlockCopyIndex;
 uint16 Fee_u16DataSetIndex;
 uint16 Fee_u16ArrayIndex;
 uint16 Fee_u16BlockSize;
 uint16 Fee_u16BlockSizeinBlockHeader;
 uint16 Fee_u16BlockNumberinBlockHeader;
 uint8 Fee_u8ActiveVirtualSector;
 uint8 Fee_u8CopyVirtualSector;
 uint32 Fee_u32InternalEraseQueue;

 uint8 Fee_u8WriteCopyVSHeader;

 uint8 Fee_u8WriteCount;

 uint8 * Fee_pu8ReadDataBuffer;
 uint8 * Fee_pu8ReadAddress;
 uint8 * Fee_pu8Data;
 uint8 * Fee_pu8CopyData;
 uint8 * Fee_pu8DataStart;
 boolean Fee_bInvalidWriteBit;

 boolean Fee_bWriteData;

 boolean Fee_bWriteBlockHeader;
 boolean bWriteFirstTime;
 boolean Fee_bFindNextVirtualSector;
 boolean Fee_bWriteVSHeader;
 boolean Fee_bWriteStartProgram;
 boolean Fee_bWritePartialBlockHeader;






}TI_Fee_GlobalVarsType;





extern const Fee_BlockConfigType Fee_BlockConfiguration[1U];

extern const Fee_VirtualSectorConfigType Fee_VirtualSectorConfiguration[2U];
extern const Device_FlashType Device_FlashDevice;






extern TI_Fee_GlobalVarsType TI_Fee_GlobalVariables[1U];
extern TI_Fee_StatusWordType_UN TI_Fee_oStatusWord[1U];

extern uint32 TI_Fee_u32FletcherChecksum;

extern uint32 TI_Fee_u32BlockEraseCount;
extern uint8 TI_Fee_u8DataSets;
extern uint8 TI_Fee_u8DeviceIndex;
extern uint32 TI_Fee_u32ActCpyVS;
extern uint8 TI_Fee_u8ErrEraseVS;
# 420 "include/ti_fee.h"
extern boolean TI_Fee_FapiInitCalled;
extern boolean TI_Fee_bEraseSuspended;
extern boolean TI_Fee_bIsMainFunctionCalled;






extern void TI_Fee_Cancel(uint8 u8EEPIndex);
extern Std_ReturnType TI_Fee_EraseImmediateBlock(uint16 BlockNumber);
extern TI_FeeModuleStatusType TI_Fee_GetStatus(uint8 u8EEPIndex);
extern void TI_Fee_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
extern void TI_Fee_Init(void);
extern Std_ReturnType TI_Fee_InvalidateBlock(uint16 BlockNumber);
extern Std_ReturnType TI_Fee_Read(uint16 BlockNumber,
                               uint16 BlockOffset,
                               uint8* DataBufferPtr,
                               uint16 Length);
extern Std_ReturnType TI_Fee_WriteAsync(uint16 BlockNumber, uint8* DataBufferPtr);
extern void TI_Fee_MainFunction(void);
extern TI_Fee_ErrorCodeType TI_FeeErrorCode(uint8 u8EEPIndex);
extern void TI_Fee_ErrorRecovery(TI_Fee_ErrorCodeType ErrorCode, uint8 u8VirtualSector);
extern TI_FeeJobResultType TI_Fee_GetJobResult(uint8 u8EEPIndex);
extern void TI_Fee_SuspendResumeErase(TI_Fee_EraseCommandType Command);







extern Std_ReturnType TI_Fee_WriteSync(uint16 BlockNumber, uint8* DataBufferPtr);
extern Std_ReturnType TI_Fee_Shutdown(void);
extern boolean TI_Fee_Format(uint32 u32FormatKey);
extern Std_ReturnType TI_Fee_ReadSync(uint16 BlockNumber,uint16 BlockOffset,uint8* DataBufferPtr,uint16 Length);



TI_Fee_AddressType TI_FeeInternal_GetNextFlashAddress(uint8 u8EEPIndex);
TI_Fee_AddressType TI_FeeInternal_AlignAddressForECC(TI_Fee_AddressType oAddress);
TI_Fee_AddressType TI_FeeInternal_GetCurrentBlockAddress(uint16 BlockNumber,uint16 DataSetNumber, uint8 u8EEPIndex);

uint32 TI_FeeInternal_GetVirtualSectorParameter(Fapi_FlashSectorType oSector, uint16 u16Bank, boolean VirtualSectorInfo,
                                                uint8 u8EEPIndex);
uint32 TI_FeeInternal_PollFlashStatus(void);
uint16 TI_FeeInternal_GetBlockSize(uint16 BlockIndex);
uint16 TI_FeeInternal_GetBlockIndex(uint16 BlockNumber);
uint16 TI_FeeInternal_GetDataSetIndex(uint16 BlockNumber);
uint16 TI_FeeInternal_GetBlockNumber(uint16 BlockNumber);
uint8 TI_FeeInternal_FindNextVirtualSector(uint8 u8EEPIndex);
uint8 TI_FeeInternal_WriteDataF021(boolean bCopy,uint16 u16WriteSize, uint8 u8EEPIndex);
boolean TI_FeeInternal_BlankCheck(uint32 u32StartAddress, uint32 u32EndAddress, uint16 u16Bank, uint8 u8EEPIndex);
Std_ReturnType TI_FeeInternal_CheckReadParameters(uint32 u32BlockSize,uint16 BlockOffset, const uint8* DataBufferPtr,
                                                  uint16 Length, uint8 u8EEPIndex);
Std_ReturnType TI_FeeInternal_CheckModuleState(uint8 u8EEPIndex);
Std_ReturnType TI_FeeInternal_InvalidateErase(uint16 BlockNumber);
TI_Fee_StatusType TI_FeeInternal_FeeManager(uint8 u8EEPIndex);
void TI_FeeInternal_WriteVirtualSectorHeader(uint8 FeeVirtualSectorNumber, VirtualSectorStatesType VsState,
                                             uint8 u8EEPIndex) ;

void TI_FeeInternal_GetVirtualSectorIndex(Fapi_FlashSectorType oSectorStart, Fapi_FlashSectorType oSectorEnd,
                                          uint16 u16Bank, boolean bOperation, uint8 u8EEPIndex);
void TI_FeeInternal_WritePreviousBlockHeader(boolean bWrite, uint8 u8EEPIndex);
void TI_FeeInternal_WriteBlockHeader(boolean bWrite, uint8 u8EEPIndex,uint16 Fee_BlockSize_u16,uint16 u16BlockNumber);
void TI_FeeInternal_SetClearCopyBlockState(uint8 u8EEPIndex, boolean bSetClear);
void TI_FeeInternal_SanityCheck(uint16 BlockSize, uint8 u8EEPIndex);
void TI_FeeInternal_StartProgramBlock(uint8 u8EEPIndex);
void TI_FeeInternal_UpdateBlockOffsetArray(uint8 u8EEPIndex, boolean bActCpyVS,uint8 u8VirtualSector);
void TI_FeeInternal_WriteInitialize(TI_Fee_AddressType oFlashNextAddress, uint8* DataBufferPtr, uint8 u8EEPIndex);
void TI_FeeInternal_CheckForError(uint8 u8EEPIndex);
void TI_FeeInternal_EnableRequiredFlashSector(uint32 u32VirtualSectorStartAddress);
uint16 TI_FeeInternal_GetArrayIndex(uint16 BlockNumber, uint16 DataSetNumber, uint8 u8EEPIndex, boolean bCallContext);

uint32 TI_FeeInternal_Fletcher16( uint8 const *pu8data, uint16 u16Length);
# 35 "app/App.cpp" 2
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 1 3
# 10 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/_ansi.h" 1 3
# 10 "/opt/arm-none-eabi-be/arm-none-eabi/include/_ansi.h" 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/newlib.h" 1 3
# 11 "/opt/arm-none-eabi-be/arm-none-eabi/include/_ansi.h" 2 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/config.h" 1 3



# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/ieeefp.h" 1 3
# 5 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/config.h" 2 3
# 12 "/opt/arm-none-eabi-be/arm-none-eabi/include/_ansi.h" 2 3
# 11 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 2 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 1 3
# 9 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3

# 9 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
extern "C" {



# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/_ansi.h" 1 3
# 14 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 2 3
# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 1 3 4
# 15 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 2 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 1 3
# 24 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 3
# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 1 3 4
# 359 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 3 4
typedef unsigned int wint_t;
# 25 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 2 3


# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/machine/_types.h" 1 3
# 28 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 2 3


typedef long __blkcnt_t;



typedef long __blksize_t;



typedef __uint64_t __fsblkcnt_t;



typedef __uint32_t __fsfilcnt_t;



typedef long _off_t;





typedef int __pid_t;



typedef short __dev_t;



typedef unsigned short __uid_t;


typedef unsigned short __gid_t;



typedef __uint32_t __id_t;







typedef unsigned short __ino_t;
# 90 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 3
typedef __uint32_t __mode_t;





__extension__ typedef long long _off64_t;





typedef _off_t __off_t;


typedef _off64_t __loff_t;


typedef long __key_t;







typedef long _fpos_t;
# 131 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 3
typedef unsigned int __size_t;
# 147 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 3
typedef signed int _ssize_t;
# 158 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_types.h" 3
typedef _ssize_t __ssize_t;



typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    unsigned char __wchb[4];
  } __value;
} _mbstate_t;




typedef void *_iconv_t;






typedef unsigned long __clock_t;






typedef __int_least64_t __time_t;





typedef unsigned long __clockid_t;


typedef long __daddr_t;



typedef unsigned long __timer_t;


typedef __uint8_t __sa_family_t;



typedef __uint32_t __socklen_t;


typedef int __nl_item;
typedef unsigned short __nlink_t;
typedef long __suseconds_t;
typedef unsigned long __useconds_t;







typedef __builtin_va_list __va_list;
# 16 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 2 3






typedef unsigned long __ULong;
# 34 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/lock.h" 1 3
# 11 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/lock.h" 3
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
# 35 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 2 3
typedef _LOCK_RECURSIVE_T _flock_t;







struct _reent;

struct __locale_t;






struct _Bigint
{
  struct _Bigint *_next;
  int _k, _maxwds, _sign, _wds;
  __ULong _x[1];
};


struct __tm
{
  int __tm_sec;
  int __tm_min;
  int __tm_hour;
  int __tm_mday;
  int __tm_mon;
  int __tm_year;
  int __tm_wday;
  int __tm_yday;
  int __tm_isdst;
};







struct _on_exit_args {
 void * _fnargs[32];
 void * _dso_handle[32];

 __ULong _fntypes;


 __ULong _is_cxa;
};


struct _atexit {
 struct _atexit *_next;
 int _ind;
 void (*_fns[32])(void);
        struct _on_exit_args * _on_exit_args_ptr;
};
# 115 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 152 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
struct __sFILE {
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;


  struct _reent *_data;



  void * _cookie;

  int (*_read) (struct _reent *, void *,
        char *, int);
  int (*_write) (struct _reent *, void *,
         const char *,
         int);
  _fpos_t (*_seek) (struct _reent *, void *, _fpos_t, int);
  int (*_close) (struct _reent *, void *);


  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;


  unsigned char _ubuf[3];
  unsigned char _nbuf[1];


  struct __sbuf _lb;


  int _blksize;
  _off_t _offset;






  _flock_t _lock;

  _mbstate_t _mbstate;
  int _flags2;
};
# 269 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
typedef struct __sFILE __FILE;



extern __FILE __sf[3];

struct _glue
{
  struct _glue *_next;
  int _niobs;
  __FILE *_iobs;
};

extern struct _glue __sglue;
# 305 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
struct _rand48 {
  unsigned short _seed[3];
  unsigned short _mult[3];
  unsigned short _add;


  __extension__ unsigned long long _rand_next;

};
# 347 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
struct _mprec
{

  struct _Bigint *_result;
  int _result_k;
  struct _Bigint *_p5s;
  struct _Bigint **_freelist;
};


struct _misc_reent
{

  char *_strtok_last;
  _mbstate_t _mblen_state;
  _mbstate_t _wctomb_state;
  _mbstate_t _mbtowc_state;
  char _l64a_buf[8];
  int _getdate_err;
  _mbstate_t _mbrlen_state;
  _mbstate_t _mbrtowc_state;
  _mbstate_t _mbsrtowcs_state;
  _mbstate_t _wcrtomb_state;
  _mbstate_t _wcsrtombs_state;
};



struct _reent
{


  int _errno;




  __FILE *_stdin, *_stdout, *_stderr;

  int _inc;

  char *_emergency;





  struct __locale_t *_locale;

  struct _mprec *_mp;

  void (*__cleanup) (struct _reent *);

  int _gamma_signgam;


  int _cvtlen;
  char *_cvtbuf;

  struct _rand48 *_r48;
  struct __tm *_localtime_buf;
  char *_asctime_buf;


  void (** _sig_func)(int);







  __FILE *__sf;
  struct _misc_reent *_misc;
  char *_signal_buf;
};
# 458 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/assert.h" 1 3





extern "C" {
# 39 "/opt/arm-none-eabi-be/arm-none-eabi/include/assert.h" 3
void __assert (const char *, int, const char *)
     __attribute__ ((__noreturn__));
void __assert_func (const char *, int, const char *, const char *)
     __attribute__ ((__noreturn__));






}
# 459 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 2 3
# 765 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
extern struct _reent *_impure_ptr ;





extern struct _reent _impure_data ;
# 883 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/reent.h" 3
extern struct _atexit *__atexit;
extern struct _atexit __atexit0;

extern void (*__stdio_exit_handler) (void);

void _reclaim_reent (struct _reent *);

extern int _fwalk_sglue (struct _reent *, int (*)(struct _reent *, __FILE *),
    struct _glue *);


}
# 12 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 2 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/cdefs.h" 1 3
# 47 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/cdefs.h" 3
# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 1 3 4
# 48 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/cdefs.h" 2 3
# 13 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 2 3




# 1 "/opt/arm-none-eabi-be/lib/gcc/arm-none-eabi/12.3.0/include/stddef.h" 1 3 4
# 18 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 2 3


# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_locale.h" 1 3
# 9 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/_locale.h" 3
struct __locale_t;
typedef struct __locale_t *locale_t;
# 21 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 2 3



# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/strings.h" 1 3
# 44 "/opt/arm-none-eabi-be/arm-none-eabi/include/strings.h" 3
extern "C" {

int bcmp(const void *, const void *, size_t) __attribute__((__pure__));
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);


void explicit_bzero(void *, size_t);


int ffs(int) __attribute__((__const__));


int ffsl(long) __attribute__((__const__));
int ffsll(long long) __attribute__((__const__));
int fls(int) __attribute__((__const__));
int flsl(long) __attribute__((__const__));
int flsll(long long) __attribute__((__const__));


char *index(const char *, int) __attribute__((__pure__));
char *rindex(const char *, int) __attribute__((__pure__));

int strcasecmp(const char *, const char *) __attribute__((__pure__));
int strncasecmp(const char *, const char *, size_t) __attribute__((__pure__));


int strcasecmp_l (const char *, const char *, locale_t);
int strncasecmp_l (const char *, const char *, size_t, locale_t);

}
# 25 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 2 3


extern "C" {

void * memchr (const void *, int, size_t);
int memcmp (const void *, const void *, size_t);
void * memcpy (void *, const void *, size_t);
void * memmove (void *, const void *, size_t);
void * memset (void *, int, size_t);
char *strcat (char *, const char *);
char *strchr (const char *, int);
int strcmp (const char *, const char *);
int strcoll (const char *, const char *);
char *strcpy (char *, const char *);
size_t strcspn (const char *, const char *);
char *strerror (int);
size_t strlen (const char *);
char *strncat (char *, const char *, size_t);
int strncmp (const char *, const char *, size_t);
char *strncpy (char *, const char *, size_t);
char *strpbrk (const char *, const char *);
char *strrchr (const char *, int);
size_t strspn (const char *, const char *);
char *strstr (const char *, const char *);

char *strtok (char *, const char *);

size_t strxfrm (char *, const char *, size_t);


int strcoll_l (const char *, const char *, locale_t);
char *strerror_l (int, locale_t);
size_t strxfrm_l (char *, const char *, size_t, locale_t);


char *strtok_r (char *, const char *, char **);


int timingsafe_bcmp (const void *, const void *, size_t);
int timingsafe_memcmp (const void *, const void *, size_t);


void * memccpy (void *, const void *, int, size_t);
# 76 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 3
char *stpcpy (char *, const char *);
char *stpncpy (char *, const char *, size_t);






char *strdup (const char *) __attribute__((__malloc__)) __attribute__((__warn_unused_result__));

char *_strdup_r (struct _reent *, const char *);

char *strndup (const char *, size_t) __attribute__((__malloc__)) __attribute__((__warn_unused_result__));

char *_strndup_r (struct _reent *, const char *, size_t);
# 100 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 3
int strerror_r (int, char *, size_t)

             __asm__ ("" "__xpg_strerror_r")

  ;







char * _strerror_r (struct _reent *, int, int, int *);


size_t strlcat (char *, const char *, size_t);
size_t strlcpy (char *, const char *, size_t);


size_t strnlen (const char *, size_t);


char *strsep (char **, const char *);


char *strnstr(const char *, const char *, size_t) __attribute__((__pure__));



char *strlwr (char *);
char *strupr (char *);



char *strsignal (int __signo);
# 175 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 3
# 1 "/opt/arm-none-eabi-be/arm-none-eabi/include/sys/string.h" 1 3
# 176 "/opt/arm-none-eabi-be/arm-none-eabi/include/string.h" 2 3

}
# 36 "app/App.cpp" 2



# 38 "app/App.cpp"
}
# 48 "app/App.cpp"
# 1 "include/data.hpp" 1
# 35 "include/data.hpp"
typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t tps1;
            uint16_t tps2;
            uint16_t tps1_raw;
            uint16_t tps2_raw;
        } values;
    };
    volatile bool new_data;
} sensors_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t wheel_speed_fl;
            uint16_t wheel_speed_fr;
            uint16_t steering_angle;
            uint16_t front_brake;
        } values;
    };
    volatile bool new_data;
} speed_data_t;

typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t rpm;
            uint16_t battey_voltage;
            uint16_t battery_current;
        } values;
    };
    volatile bool new_data;
} dynamic_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint16_t motor_voltage;
            uint16_t motor_current;
            uint16_t motor_temp;
        } values;
    };
    volatile bool new_data;
} motor_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint8_t motor1_fault;
            uint8_t motor2_fault;
            uint8_t drive_state;
        } values;
    };
    volatile bool new_data;
} drivers_status_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint8_t drive_enabled;
            uint8_t traction_on;
            uint8_t mode;
            uint8_t telemetry_enabled;
        } values;
    };
    volatile bool new_data;
} buttons_data_t;


typedef struct
{
    union
    {
        uint8_t raw[8];

        struct
        {
            uint8_t on_board_ok;
            uint8_t can_ok;
        } values;
    };
    volatile bool new_data;
} on_board_status_data_t;


extern sensors_data_t sensors_data;
extern speed_data_t speed_data;
extern dynamic_data_t dynamic_data;
extern motor_data_t motor1_data;
extern motor_data_t motor2_data;
extern drivers_status_data_t drivers_status_data;
extern buttons_data_t buttons_data;
extern on_board_status_data_t on_board_status_data;



void init_data(void);
void update_data(void);
# 49 "app/App.cpp" 2
# 143 "app/App.cpp"
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
} AppTasks_t;


typedef struct
{



    EventGroupHandle_t xSysEvents;
    SemaphoreHandle_t xDataMutex;
    TimerHandle_t xInitTimer;
    TimerHandle_t xPeriodicTimer;
    TimerHandle_t xTxTimeoutTimer;
} AppSync_t;


typedef struct
{

    StaticTask_t xInitTaskBuffer;
    StaticTask_t xDataUpdateTaskBuffer;
    StaticTask_t xRulesTaskBuffer;
    StaticTask_t xDriversFSMTaskBuffer;
    StaticTask_t xRFDataRxTaskBuffer;
    StaticTask_t xCmdTaskBuffer;
    StaticTask_t xTelemetryTaskBuffer;
    StaticTask_t xUIInputTaskBuffer;





    StackType_t xInitStack[((( ( unsigned short ) 128 )) / sizeof(StackType_t))];
    StackType_t xDataUpdateStack[((512) / sizeof(StackType_t))];
    StackType_t xRulesStack[((512) / sizeof(StackType_t))];
    StackType_t xDriversFSMStack[((1024) / sizeof(StackType_t))];
    StackType_t xRFDataRxStack[((512) / sizeof(StackType_t))];
    StackType_t xCmdStack[((512) / sizeof(StackType_t))];
    StackType_t xTelemetryStack[((512) / sizeof(StackType_t))];
    StackType_t xUIInputStack[((256) / sizeof(StackType_t))];


    StaticEventGroup_t xSysEventsBuffer;
    StaticSemaphore_t xDataMutexBuffer;
    StaticTimer_t xInitTimerBuffer;
    StaticTimer_t xPeriodicTimerBuffer;
    StaticTimer_t xTxTimeoutTimerBuffer;
} AppResources_t;


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


typedef struct
{
    AppResources_t *xResources;
    AppTasks_t *xTasks;
    AppSync_t *xSync;
    AppData_t *xData;
} AppContext_t;
# 234 "app/App.cpp"
static void vModulesInit(void);





static void vInitModulesWithInterrupts(void);





static void vCreateTasks(AppContext_t *pxCtx);





static void vCreateObjects(AppContext_t *pxCtx);
# 268 "app/App.cpp"
static void vInitTask(void *pvParameters);
static void vDataUpdateTask(void *pvParameters);
static void vRulesTask(void *pvParameters);
static void vDriversFSMTask(void *pvParameters);
static void vRFDataRxTask(void *pvParameters);
static void vCmdTask(void *pvParameters);
static void vTelemetryTask(void *pvParameters);
static void vUIInputTask(void *pvParameters);
# 286 "app/App.cpp"
static void vInitTimerCallback(TimerHandle_t xTimer);
static void vPeriodicTimerCallback(TimerHandle_t xTimer);
static void vTxTimeoutCallback(TimerHandle_t xTimer);





static AppResources_t xAppResources;
static AppTasks_t xAppTasks;
static AppSync_t xAppSync;
static AppData_t xAppData;
static AppContext_t xAppContext = {
    &xAppResources,
    &xAppTasks,
    &xAppSync,
    &xAppData};
# 313 "app/App.cpp"
extern "C" void vAppInit(void)
{

    vModulesInit();


    vCreateTasks(&xAppContext);
    vCreateObjects(&xAppContext);
# 337 "app/App.cpp"
    vTaskStartScheduler();
}






extern "C" void vAppRun(void)
{

}
# 364 "app/App.cpp"
void canMessageNotification(canBASE_t *node, uint32 messageBox)
{
    BaseType_t xHigherPriorityTaskWoken = ( ( BaseType_t ) 0 );
    EventBits_t xEvent = 0U;




    if (node == ((canBASE_t *)0xFFF7DC00U))
    {
        switch (messageBox)
        {
        case 1U:
            sensors_data.new_data = 1U;
            xEvent = (1 << 0);
            break;
        case 2U:
            speed_data.new_data = 1U;
            xEvent = (1 << 1);
            break;
        case 3U:
            motor1_data.new_data = 1U;
            xEvent = (1 << 2);
            break;
        case 4U:
            motor2_data.new_data = 1U;
            xEvent = (1 << 3);
            break;
        case 5U:
            drivers_status_data.new_data = 1U;
            xEvent = (1 << 4);
            break;
        case 6U:
            dynamic_data.new_data = 1U;
            xEvent = (1 << 5);
            break;
        default:
            break;
        }
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != 
# 405 "app/App.cpp" 3 4
                                                 __null
# 405 "app/App.cpp"
                                                     ))
    {
        if (xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken) == ( ( ( BaseType_t ) 0 ) ))
        {

        }

        if (xHigherPriorityTaskWoken != ( ( BaseType_t ) 0 ))
            if( xHigherPriorityTaskWoken != ( ( BaseType_t ) 0 ) ){ ( * ( ( volatile uint32_t * ) 0xFFFFFFB0 ) ) = ( 0x7500UL ); ( void ) ( * ( ( volatile uint32_t * ) 0xFFFFFFB0 ) ); };
    }
}





void rtiNotification(uint32 notification)
{
    BaseType_t xHigherPriorityTaskWoken = ( ( BaseType_t ) 0 );
    EventBits_t xEvent = 0U;


    switch (notification)
    {
    case 4U:
        xEvent = (1 << 25);
        break;
    case 8U:
        xEvent = (1 << 26);
        break;
    default:
        break;
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != 
# 439 "app/App.cpp" 3 4
                                                 __null
# 439 "app/App.cpp"
                                                     ))
    {
        if (xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken) == ( ( ( BaseType_t ) 0 ) ))
        {

        }

        if (xHigherPriorityTaskWoken != ( ( BaseType_t ) 0 ))
            if( xHigherPriorityTaskWoken != ( ( BaseType_t ) 0 ) ){ ( * ( ( volatile uint32_t * ) 0xFFFFFFB0 ) ) = ( 0x7500UL ); ( void ) ( * ( ( volatile uint32_t * ) 0xFFFFFFB0 ) ); };
    }
}






void sciNotification(sciBASE_t *sci, uint32 flags)
{
    BaseType_t xHigherPriorityTaskWoken = ( ( BaseType_t ) 0 );
    EventBits_t xEvent = 0U;

    if (sci == ((sciBASE_t *)0xFFF7E500U))
    {
        if ((flags & SCI_RX_INT) != 0U)
            xEvent = (1 << 10);
        else if ((flags & SCI_TX_INT) != 0U)
            xEvent = (1 << 11);
    }

    if ((xEvent != 0U) && (xAppSync.xSysEvents != 
# 469 "app/App.cpp" 3 4
                                                 __null
# 469 "app/App.cpp"
                                                     ))
    {
        if (xEventGroupSetBitsFromISR(xAppSync.xSysEvents, xEvent, &xHigherPriorityTaskWoken) == ( ( ( BaseType_t ) 0 ) ))
        {

        }

        if (xHigherPriorityTaskWoken != ( ( BaseType_t ) 0 ))
            if( xHigherPriorityTaskWoken != ( ( BaseType_t ) 0 ) ){ ( * ( ( volatile uint32_t * ) 0xFFFFFFB0 ) ) = ( 0x7500UL ); ( void ) ( * ( ( volatile uint32_t * ) 0xFFFFFFB0 ) ); };
    }
}
# 498 "app/App.cpp"
extern "C"
{


    void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                       StackType_t **ppxIdleTaskStackBuffer,
                                       uint32_t *pulIdleTaskStackSize)
    {
        static StaticTask_t xIdleTaskTCB;
        static StackType_t uxIdleTaskStack[( ( unsigned short ) 128 )];

        *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
        *ppxIdleTaskStackBuffer = uxIdleTaskStack;
        *pulIdleTaskStackSize = ( ( unsigned short ) 128 );
    }

    void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                        StackType_t **ppxTimerTaskStackBuffer,
                                        uint32_t *pulTimerTaskStackSize)
    {
        static StaticTask_t xTimerTaskTCB;
        static StackType_t uxTimerTaskStack[( ( unsigned short ) 128 )];

        *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
        *ppxTimerTaskStackBuffer = uxTimerTaskStack;
        *pulTimerTaskStackSize = ( ( unsigned short ) 128 );
    }


}
# 536 "app/App.cpp"
static void vModulesInit(void)
{




    canInit();
    gioInit();
    rtiInitPeriodicCompares();


    sciInit();


    init_data();


    canEnableErrorNotification(((canBASE_t *)0xFFF7DC00U));
    canEnableStatusChangeNotification(((canBASE_t *)0xFFF7DC00U));







    sciEnableNotification(((sciBASE_t *)0xFFF7E500U), 0);
}





static void vInitModulesWithInterrupts(void)
{

    rtiStartCounter(0U);




    rtiEnableNotification(4U);
    rtiEnableNotification(8U);
}




static void vCreateTasks(AppContext_t *pxCtx)
{
    AppResources_t *pxRes = pxCtx->xResources;
    AppTasks_t *pxTasks = pxCtx->xTasks;

    if( ( (pxRes != 
# 589 "app/App.cpp" 3 4
   __null
# 589 "app/App.cpp"
   ) && (pxTasks != 
# 589 "app/App.cpp" 3 4
   __null
# 589 "app/App.cpp"
   ) ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    pxTasks->xInitTask = MPU_xTaskCreateStatic(
        vInitTask,
        "Init",
        ((( ( unsigned short ) 128 )) / sizeof(StackType_t)),
        (void *)pxCtx,
        (( 8 ) - 1U),
        pxRes->xInitStack,
        &pxRes->xInitTaskBuffer);
    if( ( pxTasks->xInitTask != 
# 599 "app/App.cpp" 3 4
   __null 
# 599 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    pxTasks->xDataUpdateTask = MPU_xTaskCreateStatic(
        vDataUpdateTask,
        "DataUpdate",
        ((512) / sizeof(StackType_t)),
        (void *)pxCtx,
        (6U),
        pxRes->xDataUpdateStack,
        &pxRes->xDataUpdateTaskBuffer);
    if( ( pxTasks->xDataUpdateTask != 
# 609 "app/App.cpp" 3 4
   __null 
# 609 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    pxTasks->xRulesTask = MPU_xTaskCreateStatic(
        vRulesTask,
        "Rules",
        ((512) / sizeof(StackType_t)),
        (void *)pxCtx,
        (5U),
        pxRes->xRulesStack,
        &pxRes->xRulesTaskBuffer);
    if( ( pxTasks->xRulesTask != 
# 619 "app/App.cpp" 3 4
   __null 
# 619 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    pxTasks->xDriversFSMTask = MPU_xTaskCreateStatic(
        vDriversFSMTask,
        "DriversFSM",
        ((1024) / sizeof(StackType_t)),
        (void *)pxCtx,
        (3U),
        pxRes->xDriversFSMStack,
        &pxRes->xDriversFSMTaskBuffer);
    if( ( pxTasks->xDriversFSMTask != 
# 629 "app/App.cpp" 3 4
   __null 
# 629 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    pxTasks->xRFDataRxTask = MPU_xTaskCreateStatic(
        vRFDataRxTask,
        "RFDataRx",
        ((512) / sizeof(StackType_t)),
        (void *)pxCtx,
        (4U),
        pxRes->xRFDataRxStack,
        &pxRes->xRFDataRxTaskBuffer);
    if( ( pxTasks->xRFDataRxTask != 
# 639 "app/App.cpp" 3 4
   __null 
# 639 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    pxTasks->xCmdTask = MPU_xTaskCreateStatic(
        vCmdTask,
        "Cmd",
        ((512) / sizeof(StackType_t)),
        (void *)pxCtx,
        (3U),
        pxRes->xCmdStack,
        &pxRes->xCmdTaskBuffer);
    if( ( pxTasks->xCmdTask != 
# 649 "app/App.cpp" 3 4
   __null 
# 649 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    pxTasks->xTelemetryTask = MPU_xTaskCreateStatic(
        vTelemetryTask,
        "Telemetry",
        ((512) / sizeof(StackType_t)),
        (void *)pxCtx,
        (2U),
        pxRes->xTelemetryStack,
        &pxRes->xTelemetryTaskBuffer);
    if( ( pxTasks->xTelemetryTask != 
# 659 "app/App.cpp" 3 4
   __null 
# 659 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };



    pxTasks->xUIInputTask = MPU_xTaskCreateStatic(
        vUIInputTask,
        "UIInput",
        ((256) / sizeof(StackType_t)),
        (void *)pxCtx,
        (1U),
        pxRes->xUIInputStack,
        &pxRes->xUIInputTaskBuffer);
    if( ( pxTasks->xUIInputTask != 
# 671 "app/App.cpp" 3 4
   __null 
# 671 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };
}

static void vCreateObjects(AppContext_t *pxCtx)
{
    AppResources_t *pxRes = pxCtx->xResources;
    AppSync_t *pxSync = pxCtx->xSync;

    if( ( (pxRes != 
# 679 "app/App.cpp" 3 4
   __null
# 679 "app/App.cpp"
   ) && (pxSync != 
# 679 "app/App.cpp" 3 4
   __null
# 679 "app/App.cpp"
   ) ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };


    pxSync->xSysEvents = MPU_xEventGroupCreateStatic(&pxRes->xSysEventsBuffer);
    if( ( pxSync->xSysEvents != 
# 683 "app/App.cpp" 3 4
   __null 
# 683 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };


    pxSync->xDataMutex = MPU_xQueueCreateMutexStatic( ( ( uint8_t ) 1U ), ( &pxRes->xDataMutexBuffer ) );
    if( ( pxSync->xDataMutex != 
# 687 "app/App.cpp" 3 4
   __null 
# 687 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };
# 700 "app/App.cpp"
    pxSync->xTxTimeoutTimer = MPU_xTimerCreate("TxTimeoutTimer", ( ( TickType_t ) ( ( ( TickType_t ) ( 5 ) * ( TickType_t ) ( ( TickType_t ) 100000 ) ) / ( TickType_t ) 1000 ) ), ( ( BaseType_t ) 0 ), 
# 700 "app/App.cpp" 3 4
                                                                                       __null
# 700 "app/App.cpp"
                                                                                           , vTxTimeoutCallback);
    if( ( pxSync->xTxTimeoutTimer != 
# 701 "app/App.cpp" 3 4
   __null 
# 701 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };
}



static void vInitTask(void *pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;


    vInitModulesWithInterrupts();


    MPU_vTaskDelete(
# 714 "app/App.cpp" 3 4
               __null
# 714 "app/App.cpp"
                   );
}

static void vDataUpdateTask(void *pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData;
    EventBits_t uxBits;

    for (;;)
    {

        uxBits = MPU_xEventGroupWaitBits(
            pxSync->xSysEvents,
            (1 << 25),
            ( ( BaseType_t ) 1 ),
            ( ( BaseType_t ) 0 ),
            (TickType_t) 0xFFFFFFFFF);

        if ((uxBits & (1 << 25)) != 0U)
        {

            update_data();


            if (MPU_xQueueGenericReceive( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 740 "app/App.cpp" 3 4
               __null
# 740 "app/App.cpp"
               , ( ( ( TickType_t ) ( ( ( TickType_t ) ( 10 ) * ( TickType_t ) ( ( TickType_t ) 100000 ) ) / ( TickType_t ) 1000 ) ) ), ( ( BaseType_t ) 0 ) ) == ( ( BaseType_t ) 1 ))
            {



                pxData->sensors_data = sensors_data;
                pxData->speed_data = speed_data;
                pxData->dynamic_data = dynamic_data;
                pxData->motor1_data = motor1_data;
                pxData->motor2_data = motor2_data;
                pxData->drivers_status_data = drivers_status_data;
                pxData->buttons_data = buttons_data;
                pxData->on_board_status_data = on_board_status_data;

                MPU_xQueueGenericSend( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 754 "app/App.cpp" 3 4
               __null
# 754 "app/App.cpp"
               , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) );
                MPU_xEventGroupSetBits(pxSync->xSysEvents, (1 << 15));
            }
        }
    }

    MPU_vTaskDelete(
# 760 "app/App.cpp" 3 4
               __null
# 760 "app/App.cpp"
                   );
}

static void vRulesTask(void *pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData;
    EventBits_t uxBits;
    struct
    {
        uint16_t tps;
        uint16_t brake_front;


    } xLocalData;

    for (;;)
    {

        uxBits = MPU_xEventGroupWaitBits(
            pxSync->xSysEvents,
            (1 << 15),
            ( ( BaseType_t ) 1 ),
            ( ( BaseType_t ) 0 ),
            (TickType_t) 0xFFFFFFFFF);


        if (MPU_xQueueGenericReceive( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 788 "app/App.cpp" 3 4
           __null
# 788 "app/App.cpp"
           , ( (TickType_t) 0xFFFFFFFFF ), ( ( BaseType_t ) 0 ) ) == ( ( BaseType_t ) 1 ))
        {
            xLocalData.tps = pxData->sensors_data.values.tps1;
            xLocalData.brake_front = pxData->speed_data.values.front_brake;


            MPU_xQueueGenericSend( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 794 "app/App.cpp" 3 4
           __null
# 794 "app/App.cpp"
           , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) );


            MPU_xEventGroupSetBits(pxSync->xSysEvents, (1 << 16));
        }




    }

    MPU_vTaskDelete(
# 805 "app/App.cpp" 3 4
               __null
# 805 "app/App.cpp"
                   );
}

static void vDriversFSMTask(void *pvParameters)
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

    } xLocalData;

    for (;;)
    {

        uxBits = MPU_xEventGroupWaitBits(
            pxSync->xSysEvents,
            (1 << 16),
            ( ( BaseType_t ) 1 ),
            ( ( BaseType_t ) 0 ),
            (TickType_t) 0xFFFFFFFFF);


        if (MPU_xQueueGenericReceive( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 836 "app/App.cpp" 3 4
           __null
# 836 "app/App.cpp"
           , ( (TickType_t) 0xFFFFFFFFF ), ( ( BaseType_t ) 0 ) ) == ( ( BaseType_t ) 1 ))
        {
            xLocalData.drive_enabled = pxData->buttons_data.values.drive_enabled;
            xLocalData.motor1_voltage = pxData->motor1_data.values.motor_voltage;
            xLocalData.motor1_current = pxData->motor1_data.values.motor_current;
            xLocalData.motor2_voltage = pxData->motor2_data.values.motor_voltage;
            xLocalData.motor2_current = pxData->motor2_data.values.motor_current;
            xLocalData.drive_state = pxData->drivers_status_data.values.drive_state;


            MPU_xQueueGenericSend( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 846 "app/App.cpp" 3 4
           __null
# 846 "app/App.cpp"
           , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) );
        }




    }

    MPU_vTaskDelete(
# 854 "app/App.cpp" 3 4
               __null
# 854 "app/App.cpp"
                   );
}

static void vRFDataRxTask(void *pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;
    uint8_t xRxBuff[64], xId;

    for (;;)
    {
# 875 "app/App.cpp"
        sciReceive(((sciBASE_t *)0xFFF7E500U), 1, xRxBuff);


        uxBits = MPU_xEventGroupWaitBits(
            pxSync->xSysEvents,
            (1 << 10),
            ( ( BaseType_t ) 1 ),
            ( ( BaseType_t ) 0 ),
            (TickType_t) 0xFFFFFFFFF);

        if ((uxBits & (1 << 10)) != 0U)
        {

            xId = xRxBuff[0];






            EventBits_t uxSetBits = 0U;
            switch (xId)
            {
            case 0x01:
                uxSetBits = (1 << 12);
                break;
            case 0x7E:
                uxSetBits = (1 << 13);
                break;
            default:
                break;
            }

            if (uxSetBits != 0U)
            {
                MPU_xEventGroupSetBits(pxSync->xSysEvents, uxSetBits);
            }
        }
    }

    MPU_vTaskDelete(
# 915 "app/App.cpp" 3 4
               __null
# 915 "app/App.cpp"
                   );
}
# 932 "app/App.cpp"
static void vCmdTask(void *pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    EventBits_t uxBits;
    uint8_t xRxBuff[16U];

    for (;;)
    {

        uxBits = MPU_xEventGroupWaitBits(
            pxSync->xSysEvents,
            (1 << 12),
            ( ( BaseType_t ) 1 ),
            ( ( BaseType_t ) 0 ),
            (TickType_t) 0xFFFFFFFFF);

        if ((uxBits & (1 << 12)) != 0U)
        {



        }
    }

    MPU_vTaskDelete(
# 957 "app/App.cpp" 3 4
               __null
# 957 "app/App.cpp"
                   );
}

static void vTelemetryTask(void *pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    AppSync_t *pxSync = pxCtx->xSync;
    AppData_t *pxData = pxCtx->xData;
    EventBits_t uxBits = 0U;
    uint8_t xTxBuff[64] = {0}, xRxBuff[64] = {0};

    struct
    {
        float rpm;
        float battery_voltage;
        float battery_current;
        float wheel_speed_fl;
        float wheel_speed_fr;
        uint8_t motor1_fault;
        uint8_t motor2_fault;

    } xLocalData;

    for (;;)
    {

        uxBits = MPU_xEventGroupWaitBits(
            pxSync->xSysEvents,
            (1 << 26) | (1 << 13) | (1 << 14),
            ( ( BaseType_t ) 1 ),

            ( ( BaseType_t ) 0 ),
            (TickType_t) 0xFFFFFFFFF);

        if (((uxBits & (1 << 26)) != 0U) || ((uxBits & (1 << 14)) != 0U))
        {
            if (((uxBits & (1 << 26)) != 0U) &&
                MPU_xQueueGenericReceive( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 994 "app/App.cpp" 3 4
               __null
# 994 "app/App.cpp"
               , ( (TickType_t) 0xFFFFFFFFF ), ( ( BaseType_t ) 0 ) ) == ( ( BaseType_t ) 1 ))
            {

                xLocalData.rpm = pxData->dynamic_data.values.rpm;
                xLocalData.battery_voltage = pxData->dynamic_data.values.battey_voltage;
                xLocalData.battery_current = pxData->dynamic_data.values.battery_current;
                xLocalData.wheel_speed_fl = pxData->speed_data.values.wheel_speed_fl;
                xLocalData.wheel_speed_fr = pxData->speed_data.values.wheel_speed_fr;
                xLocalData.motor1_fault = pxData->drivers_status_data.values.motor1_fault;
                xLocalData.motor2_fault = pxData->drivers_status_data.values.motor2_fault;


                MPU_xQueueGenericSend( ( QueueHandle_t ) ( pxSync->xDataMutex ), 
# 1006 "app/App.cpp" 3 4
               __null
# 1006 "app/App.cpp"
               , ( ( TickType_t ) 0U ), ( ( BaseType_t ) 0 ) );
# 1020 "app/App.cpp"
                memset(xTxBuff, 0, sizeof(xTxBuff));
                memcpy(&xTxBuff[0], &xLocalData.rpm, sizeof(xLocalData.rpm));
                memcpy(&xTxBuff[4], &xLocalData.battery_voltage, sizeof(xLocalData.battery_voltage));
                memcpy(&xTxBuff[8], &xLocalData.battery_current, sizeof(xLocalData.battery_current));
                memcpy(&xTxBuff[12], &xLocalData.wheel_speed_fl, sizeof(xLocalData.wheel_speed_fl));
                memcpy(&xTxBuff[16], &xLocalData.wheel_speed_fr, sizeof(xLocalData.wheel_speed_fr));
                xTxBuff[20] = xLocalData.motor1_fault;
                xTxBuff[21] = xLocalData.motor2_fault;
            }



            sciSend(((sciBASE_t *)0xFFF7E500U), sizeof(xTxBuff), xTxBuff);


            MPU_xTimerGenericCommand( ( pxSync->xTxTimeoutTimer ), ( ( BaseType_t ) 1 ), ( MPU_xTaskGetTickCount() ), 
# 1035 "app/App.cpp" 3 4
           __null
# 1035 "app/App.cpp"
           , ( 0 ) );
        }
        else if ((uxBits & (1 << 13)) != 0U)
        {

            MPU_xTimerGenericCommand( ( pxSync->xTxTimeoutTimer ), ( ( BaseType_t ) 3 ), 0U, 
# 1040 "app/App.cpp" 3 4
           __null
# 1040 "app/App.cpp"
           , ( 0 ) );
        }
    }

    MPU_vTaskDelete(
# 1044 "app/App.cpp" 3 4
               __null
# 1044 "app/App.cpp"
                   );
}

static void vUIInputTask(void *pvParameters)
{
    AppContext_t *pxCtx = (AppContext_t *)pvParameters;
    (void)pxCtx;
# 1071 "app/App.cpp"
    uint16_t xCounter = 0;

    for (;;)
    {
        sensors_data.values.tps1 = (uint16_t)(1000U + xCounter);
        sensors_data.values.tps2 = (uint16_t)(2000U + xCounter);

        speed_data.values.wheel_speed_fl = (uint16_t)(100U + xCounter);
        speed_data.values.wheel_speed_fr = (uint16_t)(101U + xCounter);
        speed_data.values.steering_angle = 180U;
        speed_data.values.front_brake = 50U;

        dynamic_data.values.rpm = (uint16_t)(3000U + xCounter);
        dynamic_data.values.battey_voltage = 4000U;
        dynamic_data.values.battery_current = 500U;

        motor1_data.values.motor_voltage = 400U;
        motor1_data.values.motor_current = 20U;
        motor1_data.values.motor_temp = 40U;

        motor2_data.values.motor_voltage = 400U;
        motor2_data.values.motor_current = 20U;
        motor2_data.values.motor_temp = 40U;

        drivers_status_data.values.motor1_fault = 0U;
        drivers_status_data.values.motor2_fault = 0U;
        drivers_status_data.values.drive_state = 0U;

        xCounter++;

        for (volatile uint32_t i = 0; i < 2000000U; i++)
        {
        }
    }

    MPU_vTaskDelete(
# 1106 "app/App.cpp" 3 4
               __null
# 1106 "app/App.cpp"
                   );
}



static void vInitTimerCallback(TimerHandle_t xTimer)
{
    if( ( xTimer != 
# 1113 "app/App.cpp" 3 4
   __null 
# 1113 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };


    vInitModulesWithInterrupts();


    MPU_xTimerGenericCommand( ( xTimer ), ( ( BaseType_t ) 5 ), 0U, 
# 1119 "app/App.cpp" 3 4
   __null
# 1119 "app/App.cpp"
   , ( 0 ) );
}

static void vPeriodicTimerCallback(TimerHandle_t xTimer)
{
    if( ( xTimer != 
# 1124 "app/App.cpp" 3 4
   __null 
# 1124 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };





}

static void vTxTimeoutCallback(TimerHandle_t xTimer)
{
    if( ( xTimer != 
# 1134 "app/App.cpp" 3 4
   __null 
# 1134 "app/App.cpp"
   ) == ( ( BaseType_t ) 0 ) ) { vPortDisableInterrupts(); for( ;; ); };

    MPU_xEventGroupSetBits(xAppContext.xSync->xSysEvents, (1 << 14));
}
