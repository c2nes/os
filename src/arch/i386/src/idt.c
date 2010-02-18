
#define __IDT_DECLARED__

#include <arch/descriptor.h>
#include <arch/idt.h>
#include <arch/params.h>

#include <string.h>

struct idt_t idt;
static struct gate_descriptor interrupt_descriptors[INTR_VECTOR_COUNT];

void init_idt(void) {
    idt.limit = sizeof(interrupt_descriptors);
    idt.vectors = interrupt_descriptors;
    memset(interrupt_descriptors, 0, sizeof(interrupt_descriptors));
}
