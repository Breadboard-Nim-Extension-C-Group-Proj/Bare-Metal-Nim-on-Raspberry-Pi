ENTRY(_start)

SECTIONS
{
    . = 0x80000;

    .text : {
        *(.text.boot)
        *(.text*)
    }
    
    . = ALIGN(8);
    .rodata : { *(.rodata*) }
    
    . = ALIGN(8);
    .data : { *(.data*) }
    
    . = ALIGN(8);
    .bss : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8); 
        __bss_end = .;
    }
}