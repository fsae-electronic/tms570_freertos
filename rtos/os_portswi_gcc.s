@/** @file os_portswi_gcc.s
@*   @brief GCC equivalents of TI CGT's '#pragma SWI_ALIAS'
@*
@*   The TMS570 FreeRTOS port reaches its protected-mode entry points through
@*   the SWI (SVC) handler in os_portasm.s, which reads the SVC immediate and
@*   dispatches through jumpTable:
@*
@*       0 - vPortYield              4 - vPortTaskUsesFPU
@*       1 - prvRaisePrivilege       
@*       2 - vPortEnterCritical      5 - vPortDisableInterrupts
@*       3 - vPortExitCritical       6 - vPortEnableInterrupts
@*
@*   With the TI compiler, '#pragma SWI_ALIAS(fn, n)' in os_portmacro.h makes
@*   a call to fn() compile directly into 'svc #n', so no function body is
@*   ever needed. GCC has no equivalent pragma and ignores it, leaving the
@*   declarations in os_portmacro.h as ordinary undefined externs.
@*
@*   These thin wrappers supply the missing bodies: issue the SVC, then
@*   return. One extra bl/bx pair per call versus the TI build.
@*/

        .text
        .arm
        .syntax unified

        .macro  PORT_SWI name, num
        .global \name
        .type   \name, %function
\name:
        svc     #\num
        bx      lr
        .size   \name, .-\name
        .endm

        PORT_SWI vPortYield,             0
        PORT_SWI prvRaisePrivilege,      1
        PORT_SWI vPortEnterCritical,     2
        PORT_SWI vPortExitCritical,      3
        PORT_SWI vPortTaskUsesFPU,       4
        PORT_SWI vPortDisableInterrupts, 5
        PORT_SWI vPortEnableInterrupts,  6
