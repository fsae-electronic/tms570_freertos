@/** @file os_portswi_gcc.s
@*   @brief GCC equivalents of TI CGT's '#pragma SWI_ALIAS'
@*
@*   The TMS570 FreeRTOS port reaches its protected-mode entry points through
@*   the SWI (SVC) handler in os_portasm.s, which reads the SVC immediate and
@*   dispatches through jumpTable:
@*
@*       0 - vPortYield              4 - vPortTaskUsesFPU
@*       1 - prvRaisePrivilege
@*
@*   With the TI compiler, '#pragma SWI_ALIAS(fn, n)' in os_portmacro.h makes
@*   a call to fn() compile directly into 'svc #n', so no function body is
@*   ever needed. GCC has no equivalent pragma and ignores it, leaving the
@*   declarations in os_portmacro.h as ordinary undefined externs.
@*
@*   These thin wrappers supply the missing bodies: issue the SVC, then
@*   return. One extra bl/bx pair per call versus the TI build.
@*
@*   vPortEnterCritical/vPortExitCritical/vPortDisableInterrupts/
@*   vPortEnableInterrupts are deliberately NOT routed through SVC (unlike
@*   the TI original, which used SWI #2/#3/#5/#6 for these -- see jumpTable
@*   in os_portasm.s, still present but now unreachable dead code for those
@*   four slots). SVC exception entry on ARM does not itself mask IRQ (only
@*   IRQ/FIQ/Reset entry do); swiPortEnterCritical only set the I-bit in
@*   SPSR, to take effect once the SVC returned. That left a real window,
@*   spanning vPortSWI's dispatch plus the handler body, during which a
@*   genuine hardware IRQ (the periodic RTI tick) could preempt while the
@*   CPU was already inside SVC-mode exception handling -- a nested
@*   exception this port's portSAVE_CONTEXT/portRESTORE_CONTEXT (which use
@*   the IRQ-mode stack as transient scratch space) cannot survive. Since
@*   every task in this project runs in System mode (privileged) -- plain
@*   xTaskCreateStatic() is used throughout, never xTaskCreateRestricted()
@*   -- CPSID/CPSIE can be executed directly wherever these are called
@*   from, with no SVC indirection and therefore no window at all: CPSID
@*   masks IRQ atomically as part of instruction execution.
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
        PORT_SWI vPortTaskUsesFPU,       4

        .global vPortEnterCritical
        .type   vPortEnterCritical, %function
vPortEnterCritical:
        cpsid   i
        ldr     r0, =ulCriticalNesting
        ldr     r1, [r0]
        add     r1, r1, #1
        str     r1, [r0]
        bx      lr
        .size   vPortEnterCritical, .-vPortEnterCritical

        .global vPortExitCritical
        .type   vPortExitCritical, %function
vPortExitCritical:
        ldr     r0, =ulCriticalNesting
        ldr     r1, [r0]
        cmp     r1, #0
        bxeq    lr
        subs    r1, r1, #1
        str     r1, [r0]
        bxne    lr
        cpsie   i
        bx      lr
        .size   vPortExitCritical, .-vPortExitCritical

        .global vPortDisableInterrupts
        .type   vPortDisableInterrupts, %function
vPortDisableInterrupts:
        cpsid   i
        bx      lr
        .size   vPortDisableInterrupts, .-vPortDisableInterrupts

        .global vPortEnableInterrupts
        .type   vPortEnableInterrupts, %function
vPortEnableInterrupts:
        cpsie   i
        bx      lr
        .size   vPortEnableInterrupts, .-vPortEnableInterrupts
