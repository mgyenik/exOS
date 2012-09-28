/* Baremetal linker script. Derivative of the one for F4OS */

/* Specify the memory areas */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 112K
  ETHRAM (xrw)    : ORIGIN = 0x2001C000, LENGTH = 16K
  CCMRAM (xrw)    : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Highest address of the kernel stack */
_estack = 0x10010000;    /* end of 64K CCM RAM */
/* Heap */
_sheap = 0x20000000;    /* beginning of 112K SRAM */
_eheap = 0x2001BFFC;    /* end of 112K SRAM */

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
    } > CCMRAM
}
