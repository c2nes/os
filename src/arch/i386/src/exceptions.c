
/* Handlers for standard exceptions */

#include <io.h>
#include <arch/misc.h>
#include <arch/exceptions.h>

void divide_error(void) {
    puts("Divide by zero\nHalting");
    halt();
}

void double_fault(void) {
    puts("Caught double fault\nHalting");
    halt();
}

void exception_trap(unsigned short vector) {
    switch(vector) {
    case DIVIDE_ERROR:
        divide_error();
        break;

    case DOUBLE_FAULT:
        double_fault();
        break;

    default:
        /* Unknown exception, give up for debugging */
        printf("Caught unknown exception: %d\nHalting", vector);
        halt();
        break;
    }
}
