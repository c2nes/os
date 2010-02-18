
#ifndef __i386_PARAMS_H_
#define __i386_PARAMS_H_

/* Size of interrupt table to be stored in memory. This must statisfy
   INTR_VECTOR_COUNT <= INTR_MAX_VECTOR_COUNT */
#define INTR_VECTOR_COUNT 64

/* The interrupt descriptor table can contain a maximum of 256 elements */
#define INTR_MAX_VECTOR_COUNT 256

/* The first 32 interrupt vectors are reserved by Intel for handling
   predetermined interrupts as well as standard exceptions and traps */
#define INTR_CRITICAL_VECTORS 32 

#endif // #ifndef __i386_PARAMS_H_
