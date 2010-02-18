
#include <stddef.h>
#include <stdarg.h>

void* _va_arg(va_list* ap, size_t s) {
    void* v = *ap;
    *ap += s;
    return v;
}
