/* Baremetal linker script. Derivative of the one for F4OS */

/* Specify the memory areas */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00400000, LENGTH = 0x00040000 /* flash, 256K */
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x0000c000 /* sram, 48K */
}

/* Highest address of the kernel stack */
_estack = 0x20009ffc;
/* Heap */
_sheap = 0x2000a000;
_eheap = 0x2000c000;

SECTIONS {
    flash :
    {
        KEEP(*(.vectors))
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
        _srodata = .;
        *(.rodata*)
        . = ALIGN(4);
        _erodata = .;
    } > FLASH

    ram :
    {
        . = ALIGN(4);
        _sdata = .;        /* create a global symbol at data start */
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end */
        . = ALIGN(4);
        _sbss = .;         /* define a global symbol at bss start */
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;         /* define a global symbol at bss end */
    } > RAM
}
