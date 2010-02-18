
#ifndef __i386_EXCEPTIONS_H_
#define __i386_EXCEPTIONS_H_

/* Intel standard interrupt vectors */
#define DIVIDE_ERROR 0
#define NMI 2
#define BREAKPOINT 3
#define OVERFLOW 4
#define BOUND_EXCEEDED 5
#define INVALID_OPCODE 6
#define NO_MATH_COPROCESSOR 7
#define DOUBLE_FAULT 8
#define COPROCESSOR_SEGMENT_OVERUN 9
#define INVALID_TSS 10
#define SEGMENT_NOT_PRESENT 11
#define STACK_SEGMENT_FAULT 12
#define GENERAL_PROTECTION 13
#define PAGE_FAULT 14
#define FLOATING_POINT_ERROR 16
#define ALIGNMENT_CHECK 17
#define MACHINE_CHECK 18
#define SIMD_FP_EXCEPTION 19

void divide_error(void);
void double_fault(void);
void exception_trap(unsigned short vector);

#endif // #ifndef __i386_EXCEPTIONS_H_
