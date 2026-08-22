/*----------------------------------------------------------------------------
 * sys_link.ld
 *
 * GNU ld linker script for TMS570LS1224 with FreeRTOS MPU port.
 * Adapted from TI sys_link.cmd (TI format → GNU ld format).
 *
 * Memory map:
 *   VECTORS   0x00000000   32 B    Interrupt vector table
 *   KERNEL    0x00000020   32 KB   FreeRTOS kernel code (MPU-protected)
 *   FLASH0    0x00008020  ~1.2 MB  Application flash
 *   STACKS    0x08000000    2 KB   CPU mode stacks (set up by sys_core.s)
 *   KRAM      0x08000800    2 KB   FreeRTOS kernel BSS/heap (MPU-protected)
 *   RAM       0x08001000  188 KB   Application RAM
 *----------------------------------------------------------------------------*/

ENTRY(_c_int00)

MEMORY
{
    VECTORS (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00000020
    KERNEL  (rx)  : ORIGIN = 0x00000020, LENGTH = 0x00008000
    FLASH0  (rx)  : ORIGIN = 0x00008020, LENGTH = 0x00137FE0
    STACKS  (rw)  : ORIGIN = 0x08000000, LENGTH = 0x00000800
    KRAM    (rw)  : ORIGIN = 0x08000800, LENGTH = 0x00000800
    RAM     (rw)  : ORIGIN = 0x08001000, LENGTH = 0x0002F000
}

SECTIONS
{
    /* Interrupt vector table — must be at address 0x0 */
    .intvecs :
    {
        KEEP(*(.intvecs))
    } >VECTORS

    /* FreeRTOS kernel code in MPU-protected KERNEL region */
    .kernelTEXT :
    {
        . = ALIGN(4);
        KEEP(*(.kernelTEXT))
        . = ALIGN(4);
    } >KERNEL

    /* Application code */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        KEEP (*(.init))
        KEEP (*(.fini))
        . = ALIGN(4);
        _etext = .;
    } >FLASH0

    /* Read-only data */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH0

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH0

    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH0

    /* C++ constructor/destructor tables — used by __libc_init_array() */
    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH0

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH0

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH0

    /* Load address of .data (in flash), copied to RAM at startup */
    _sidata = LOADADDR(.data);

    /* Initialized data: VMA in RAM, LMA copy in FLASH0 */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT> FLASH0

    /* Zero-initialized data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    PROVIDE (end  = _ebss);
    PROVIDE (_end = _ebss);

    /* FreeRTOS kernel BSS in MPU-protected KRAM */
    .kernelBSS :
    {
        . = ALIGN(4);
        *(.kernelBSS)
        . = ALIGN(4);
    } >KRAM

    /* FreeRTOS kernel heap in MPU-protected KRAM */
    .kernelHEAP :
    {
        . = ALIGN(4);
        *(.kernelHEAP)
        . = ALIGN(4);
    } >KRAM

    .ARM.attributes 0 : { *(.ARM.attributes) }
}
