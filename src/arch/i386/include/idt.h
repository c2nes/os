
#ifndef __i386_IDT_H_
#define __i386_IDT_H_

#include <arch/descriptor.h>
#include <arch/params.h>

struct idt_t {
    unsigned short limit;
    struct gate_descriptor* vectors;
} __attribute__ ((__packed__));

#ifndef __IDT_DECLARED__
extern struct idt_t idt;
#endif

void init_idt(void);
void load_idt(struct idt_t* idt);
void enable_interrupts(void);
void disable_interrupts(void);

#endif // #ifndef __i386_IDT_H_
