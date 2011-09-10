
/* Kernel linker script */

OUTPUT_FORMAT(binary);

/* Input files */
STARTUP(obj/kern/entry/entry.o)

SECTIONS
{
    . = 0x8000;
    .text : {
            *(.text)
            }

    .data : {
            *(.data)
            }

    .bss  : {
            *(.bss)
            }
}
