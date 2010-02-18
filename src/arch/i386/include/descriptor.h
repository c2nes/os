
#ifndef __i386_DESCRIPTOR_H_
#define __i386_DESCRIPTOR_H_

#include <stdint.h>

/* Describes a segment descriptor */
struct segment_descriptor {
    uint32_t limit_low:16;
    uint32_t base_low:24;
    uint32_t accessed:1;
    uint32_t type:3;
    uint32_t _h0:1;
    uint32_t dpl:3;
    uint32_t present:1;
    uint32_t limit_high:4;
    uint32_t available:1;
    uint32_t _seg64:1; /* unused in 32 bit mode (always 0) */
    uint32_t opsize:1;
    uint32_t granularity:1;
    uint32_t base_high:8;
} __attribute__ ((__packed__));

/* Describes a segment descriptor in either the GDT or a LDT */
struct tss_descriptor {
    uint32_t limit_low:16;
    uint32_t base_low:24;
    uint32_t _h0:1;
    uint32_t busy:1;
    uint32_t _z0:1;
    uint32_t _h1:1;
    uint32_t _z1:1;
    uint32_t dpl:3;
    uint32_t present:1;
    uint32_t limit_high:4;
    uint32_t available:1;
    uint32_t _z2:2;
    uint32_t granularity:1;
    uint32_t base_high:8;
} __attribute__ ((__packed__));

/* Describes a gate descriptor. Describes either a call gate, task gate,
   interrupt gate, or trap gate */
struct gate_descriptor {
    uint32_t offset_low:16;
    uint32_t selector:16;
    uint32_t param_count:5;
    uint32_t _z0:3;
    uint32_t type:4;
    uint32_t _z1:1;
    uint32_t dpl:2;
    uint32_t present:1;
    uint32_t offset_high:16;
} __attribute__ ((__packed__));

/* Unified descriptor type */
union descriptor {
    struct segment_descriptor segment;
    struct tss_descriptor tss;
    struct gate_descriptor gate;
    uint32_t words[2];
};

/* Descriptor types */
enum desc_type_t {
    DESC_CALL_GATE16 = 0x4,
    DESC_TASK_GATE = 0x5,
    DESC_INTERRUPT_GATE16 = 0x6,
    DESC_TRAP_GATE16 = 0x7,
    DESC_TASK_STATE_SEGMENT = 0x9,
    DESC_CALL_GATE32 = 0xc,
    DESC_INTERRUPT_GATE32 = 0xe,
    DESC_TRAP_GATE32 = 0xf,
    DESC_SEGMENT = 0xff /* Normal data/code segments do not have a corresponding
                           descriptor type */
};

/* opsize flags */
#define DESC_16BIT 0x0
#define DESC_32BIT 0x1

/* Code and data segment type flags */
#define DESC_DATA_SEGMENT 0x0
#define DESC_DATA_EXPAND_UP 0x0
#define DESC_DATA_EXPAND_DOWN 0x4
#define DESC_DATA_RO   0x0
#define DESC_DATA_RDWR 0x2
#define DESC_CODE_SEGMENT 0x8
#define DESC_CODE_NONCONFORMING 0x0
#define DESC_CODE_CONFORMING 0x4
#define DESC_CODE_EXEC_ONLY 0x0
#define DESC_CODE_READ 0x2

/* Offset, base, limit splitting */
#define DESC_GATE_OFFSET_HIGH(o) (((unsigned int)(o)) >> 16)
#define DESC_GATE_OFFSET_LOW(o) (((unsigned int)(o)) & 0xffff)
#define DESC_SEGMENT_LIMIT_HIGH(l) ((((unsigned int)(l)) >> 16) & 0xf)
#define DESC_SEGMENT_LIMIT_LOW(l) (((unsigned int)(l)) & 0xffff)
#define DESC_SEGMENT_BASE_HIGH(b) (((unsigned int)(b)) >> 24)
#define DESC_SEGMENT_BASE_LOW(b) (((unsigned int)(b)) & 0xffffff)

/* Set offset, limit, base */
#define DESC_GATE_SET_OFFSET(d, o) (d).offset_low = DESC_GATE_OFFSET_LOW(o); \
                                   (d).offset_high = DESC_GATE_OFFSET_HIGH(o)

/* Build a segment selector */
#define SEGMENT_SELECTOR(i, ti, rpl) ((((i) & 0x1fff) << 3) | (((ti) & 1) << 2) | ((rpl) & 3))

/* Standard segment selectors */
#define DATA_SEGMENT SEGMENT_SELECTOR(0x1, 0, 0)
#define CODE_SEGMENT SEGMENT_SELECTOR(0x2, 0, 0)

void init_descriptor(union descriptor* desc, enum desc_type_t type, unsigned char v);

#endif // #ifndef __i386_DESCRIPTOR_H_

