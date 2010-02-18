
#ifndef __i386_STDARG_H_
#define __i386_STDARG_H_

#include <stddef.h>

typedef unsigned char* va_list;
#define va_start(ap, argn) (ap = (va_list) (((unsigned char*)&(argn)) + sizeof(argn)));
#define va_arg(ap, type) (*((type*)_va_arg(&(ap), sizeof(type))))

void* _va_arg(va_list* ap, size_t s);

#endif // #ifndef __i386_STDARG_H_
