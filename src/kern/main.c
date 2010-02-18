
#include <init.h>
#include <io.h>

#include <arch/misc.h>

void main(void) {
    /* Initialize interrupts and console */
    init_interrupts();
    init_console();

    printf("Booting...\n");

    halt();
}
