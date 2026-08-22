/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    GNU as (GAS) syntax version of os_portasm.asm for arm-none-eabi-gcc.
    Converted from TI CGT syntax (os_portasm.asm).
*/

        .section .kernelTEXT, "ax", %progbits
        .syntax unified
        .cpu cortex-r4
        .fpu vfpv3-d16
        .arm

        .global vTaskSwitchContext
        .global xTaskIncrementTick
        .global ulTaskHasFPUContext
        .global pxCurrentTCB
        .global ulCriticalNesting

@/*-----------------------------------------------------------*/
@
@ Save Task Context
@
.macro portSAVE_CONTEXT

        DSB

        @ Push R0 as we are going to use it
        STMDB   SP!, {R0}

        @ Set R0 to point to the task stack pointer.
        SUB SP, SP, #4
        STMDA   SP,{SP}^
        LDMIA   SP!,{R0}

        @ Push the return address onto the stack.
        STMDB   R0!, {LR}

        @ Now LR has been saved, it can be used instead of R0.
        MOV LR, R0

        @ Pop R0 so it can be saved onto the task stack.
        LDMIA   SP!, {R0}

        @ Push all the system mode registers onto the task stack.
        STMDB   LR,{R0-LR}^
        SUB LR, LR, #60

        @ Push the SPSR onto the task stack.
        MRS R0, SPSR
        STMDB   LR!, {R0}

        @ Determine if the task maintains an FPU context.
        LDR R0, ulFPUContextConst
        LDR R0, [R0]

        @ Test the flag
        CMP     R0, #0

        @ Skip FPU register save if the task has no FPU context.
        BEQ     .Lskip_save_fpu_\@

        VSTMDB  LR!, {D0-D15}
        VMRS    R1,  FPSCR
        STMDB   LR!, {R1}

.Lskip_save_fpu_\@:
        @ Save the flag
        STMDB   LR!, {R0}

        @ Store the new top of stack for the task.
        LDR R0, pxCurrentTCBConst
        LDR R0, [R0]
        STR LR, [R0]

.endm

@/*-----------------------------------------------------------*/
@
@ Restore Task Context
@
.macro portRESTORE_CONTEXT

        LDR     R0, pxCurrentTCBConst
        LDR     R0, [R0]

        @ Configure task stack MPU region (region 5)
        mov     r4,  #5
        add     r12, r0, #4
        ldmia   r12!, {r1-r3}
        mcr     p15, #0, r4, c6, c2, #0     @ Select region
        mcr     p15, #0, r1, c6, c1, #0     @ Base Address
        mcr     p15, #0, r3, c6, c1, #4     @ Access Attributes
        mcr     p15, #0, r2, c6, c1, #2     @ Size and Enable

        ldr     r5, portMax_MPU_Region
        mov     r4,  #6
        @ Load dynamic per-task MPU regions (6..portMax_MPU_Region)
.Lmpu_loop_\@:
        ldmia   r12!, {r1-r3}
        mcr     p15, #0, r4, c6, c2, #0     @ Select region
        mcr     p15, #0, r1, c6, c1, #0     @ Base Address
        mcr     p15, #0, r3, c6, c1, #4     @ Access Attributes
        mcr     p15, #0, r2, c6, c1, #2     @ Size and Enable
        add     r4, r4, #1
        cmp     r4, r5
        bne     .Lmpu_loop_\@

        LDR     LR, [R0]

        @ The FPU context flag is the first thing on the stack.
        LDR     R0, ulFPUContextConst
        LDMIA   LR!, {R1}
        STR     R1, [R0]

        @ Test the flag
        CMP     R1, #0

        @ Skip VFP register restore if no FPU context.
        BEQ     .Lskip_restore_fpu_\@

        @ Restore the floating point context.
        LDMIA   LR!, {R0}
        VLDMIA  LR!, {D0-D15}
        VMSR    FPSCR, R0

.Lskip_restore_fpu_\@:
        @ Get the SPSR from the stack.
        LDMIA   LR!, {R0}
        MSR     SPSR_cxsf, R0

        @ Restore all system mode registers for the task.
        LDMIA   LR, {R0-R14}^

        @ Restore the return address.
        LDR     LR, [LR, #+60]
        DSB

        @ Return, correcting the offset in LR.
        SUBS    PC, LR, #4

.endm

portMax_MPU_Region:     .word   12 - 1

@/*-----------------------------------------------------------*/
@ Start the first task by restoring its context.

        .global vPortStartFirstTask
        .type   vPortStartFirstTask, %function
vPortStartFirstTask:
        cps #0x13
        portRESTORE_CONTEXT
        .size vPortStartFirstTask, .-vPortStartFirstTask

@/*-----------------------------------------------------------*/
@ Yield to another task.

        .type   swiPortYield, %function
swiPortYield:
        @ Restore stack and LR before falling through to vPortYieldProcessor
        ldmfd   sp!, {r11,r12,lr}

        .global vPortYieldProcessor
        .type   vPortYieldProcessor, %function
vPortYieldProcessor:
        @ SWI doesn't offset LR the way IRQ does; add 4 manually.
        ADD     LR, LR, #4

        portSAVE_CONTEXT

        BL      vTaskSwitchContext

        portRESTORE_CONTEXT
        .size vPortYieldProcessor, .-swiPortYield

@/*-----------------------------------------------------------*/
@ Yield to another task from within the FreeRTOS API

        .global vPortYeildWithinAPI
        .type   vPortYeildWithinAPI, %function
vPortYeildWithinAPI:
        portSAVE_CONTEXT

        @ Clear SSI flag
        ldr     R0, =0xFFFFFFF4
        LDR     R0, [R0]

        BL      vTaskSwitchContext

        portRESTORE_CONTEXT
        .size vPortYeildWithinAPI, .-vPortYeildWithinAPI

@/*-----------------------------------------------------------*/
@ Preemptive Tick

        .global vPortPreemptiveTick
        .type   vPortPreemptiveTick, %function
vPortPreemptiveTick:
        portSAVE_CONTEXT

        @ Clear RTI compare interrupt flag
        ldr     R0, =0xFFFFFC88
        MOV     R1, #1
        STR     R1, [R0]

        BL      xTaskIncrementTick

        CMP R0, #0
        BLNE    vTaskSwitchContext

        portRESTORE_CONTEXT
        .size vPortPreemptiveTick, .-vPortPreemptiveTick

@-------------------------------------------------------------------------------

        .global vPortInitialiseFPSCR
        .type   vPortInitialiseFPSCR, %function
vPortInitialiseFPSCR:
        MOV     R0, #0
        VMSR    FPSCR, R0
        BX      LR
        .size vPortInitialiseFPSCR, .-vPortInitialiseFPSCR

@-------------------------------------------------------------------------------

        .global ulPortCountLeadingZeros
        .type   ulPortCountLeadingZeros, %function
ulPortCountLeadingZeros:
        CLZ     R0, R0
        BX      LR
        .size ulPortCountLeadingZeros, .-ulPortCountLeadingZeros

@-------------------------------------------------------------------------------
@ SWI handler — dispatches to protected-mode functions via jump table

        .global vPortSWI
        .type   vPortSWI, %function
vPortSWI:
        stmfd   sp!, {r11,r12,lr}
        ldrb    r12, [lr, #-1]
        ldr     r14, swiTable
        ldr     r12, [r14, r12, lsl #2]
        blx     r12
        ldmfd   sp!, {r11,r12,pc}^
swiTable:
        .word   jumpTable
jumpTable:
        .word   swiPortYield              @ 0 - vPortYieldProcessor
        .word   swiRaisePrivilege         @ 1 - Raise Privilege
        .word   swiPortEnterCritical      @ 2 - vPortEnterCritical
        .word   swiPortExitCritical       @ 3 - vPortExitCritical
        .word   swiPortTaskUsesFPU        @ 4 - vPortTaskUsesFPU
        .word   swiPortDisableInterrupts  @ 5 - vPortDisableInterrupts
        .word   swiPortEnableInterrupts   @ 6 - vPortEnableInterrupts
        .size vPortSWI, .-vPortSWI

@-------------------------------------------------------------------------------
        .type   swiPortDisableInterrupts, %function
swiPortDisableInterrupts:
        mrs     r11, SPSR
        orr     r11, r11, #0x80
        msr     SPSR_c, r11
        bx      r14
        .size swiPortDisableInterrupts, .-swiPortDisableInterrupts

@-------------------------------------------------------------------------------
        .type   swiPortEnableInterrupts, %function
swiPortEnableInterrupts:
        mrs     r11, SPSR
        bic     r11, r11, #0x80
        msr     SPSR_c, r11
        bx      r14
        .size swiPortEnableInterrupts, .-swiPortEnableInterrupts

@-------------------------------------------------------------------------------
        .type   swiPortTaskUsesFPU, %function
swiPortTaskUsesFPU:
        ldr     r12, ulTaskHasFPUContextConst
        mov     r11, #1
        str     r11, [r12]
        mov     r11, #0
        vmsr    FPSCR, r11
        bx      r14
        .size swiPortTaskUsesFPU, .-swiPortTaskUsesFPU

@-------------------------------------------------------------------------------
@ swiRaisePrivilege — returns zero in R0 if caller was in user mode
        .type   swiRaisePrivilege, %function
swiRaisePrivilege:
        mrs     r12, spsr
        ands    r0, r12, #0x0F
        orreq   r12, r12, #0x1F
        msreq   spsr_c, r12
        bx      r14
        .size swiRaisePrivilege, .-swiRaisePrivilege

@-------------------------------------------------------------------------------
        .type   swiPortEnterCritical, %function
swiPortEnterCritical:
        mrs     r11, SPSR
        orr     r11, r11, #0x80
        msr     SPSR_c, r11
        ldr     r11, ulCriticalNestingConst
        ldr     r12, [r11]
        add     r12, r12, #1
        str     r12, [r11]
        bx      r14
        .size swiPortEnterCritical, .-swiPortEnterCritical

@-------------------------------------------------------------------------------
        .type   swiPortExitCritical, %function
swiPortExitCritical:
        ldr     r11, ulCriticalNestingConst
        ldr     r12, [r11]
        cmp     r12, #0
        bxeq    r14
        subs    r12, r12, #1
        str     r12, [r11]
        bxne    r14
        mrs     r11, SPSR
        bic     r11, r11, #0x80
        msr     SPSR_c, r11
        bx      r14
        .size swiPortExitCritical, .-swiPortExitCritical

@-------------------------------------------------------------------------------
@ void prvMpuSetRegion(unsigned region, unsigned base, unsigned size, unsigned access)

        .global prvMpuSetRegion
        .type   prvMpuSetRegion, %function
prvMpuSetRegion:
        and   r0,  r0, #15
        mcr   p15, #0, r0, c6, c2, #0
        mcr   p15, #0, r1, c6, c1, #0       @ Base Address
        mcr   p15, #0, r3, c6, c1, #4       @ Access Attributes
        mcr   p15, #0, r2, c6, c1, #2       @ Size and Enable
        bx    lr
        .size prvMpuSetRegion, .-prvMpuSetRegion

@-------------------------------------------------------------------------------
        .global prvMpuEnable
        .type   prvMpuEnable, %function
prvMpuEnable:
        mrc   p15, #0, r0, c1, c0, #0
        orr   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        bx    lr
        .size prvMpuEnable, .-prvMpuEnable

@-------------------------------------------------------------------------------
        .global prvMpuDisable
        .type   prvMpuDisable, %function
prvMpuDisable:
        mrc   p15, #0, r0, c1, c0, #0
        bic   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        bx    lr
        .size prvMpuDisable, .-prvMpuDisable

@-------------------------------------------------------------------------------

pxCurrentTCBConst:        .word   pxCurrentTCB
ulFPUContextConst:        .word   ulTaskHasFPUContext
ulCriticalNestingConst:   .word   ulCriticalNesting
ulTaskHasFPUContextConst: .word   ulTaskHasFPUContext
