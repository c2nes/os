
#ifndef __i386_MISC_H_
#define __i386_MISC_H_

#include <stdint.h>

extern void outb(uint16_t addr, uint8_t value);
extern uint8_t inb(uint16_t addr);

extern void halt(void);

#endif // #ifndef __i386_MISC_H_
