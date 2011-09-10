
#include <stdbool.h>
#include <console.h>
#include <stdarg.h>
#include <string.h>
#include <io.h>

/* Format buffer space for printf */
static char format_buffer[256];

void puts(const char* s) {
    console_puts(s);
}

void putchar(char c) {
    console_putchar(c);
}

static void _format_d(unsigned int n, unsigned char base, bool is_signed) {
    static const char* char_table = "0123456789abcdef";
    int i = 0;
    int j = 0;
    int t;

    /* Zero is a special case */
    if(n == 0) {
        format_buffer[0] = '0';
        format_buffer[1] = '\0';
        return;
    }

    /* Negative number */
    if(n & (1 << 31) && is_signed) {
        format_buffer[0] = '-';
        n = ~(n - 1);
        j = 0;
        i = 1;
    }

    while(n > 0) {
        format_buffer[i] = char_table[n % base];
        n /= base;
        i++;
    }

    format_buffer[i] = '\0';
    i--;
    
    while(j < i) {
        t = format_buffer[i];
        format_buffer[i--] = format_buffer[j];
        format_buffer[j++] = t;
    }
}

static void _pad_format_buffer(int min_width, char pad_character, short pad_right) {
    int width, diff, i;

    width = strlen(format_buffer);
    diff = min_width - width;

    if(diff <= 0) {
        return;
    }

    if(pad_right) {
        for(i = width; i < min_width; i++) {
            format_buffer[i] = pad_character;
        }
        format_buffer[min_width] = '\0';
    } else {
        for(i = width; i >= 0; i--) {
            format_buffer[i + diff] = format_buffer[i];
        }
        for(i = 0; i < diff; i++) {
            format_buffer[i] = pad_character;
        }
    }
}

int printf(const char* fmt, ...) {
    int i, c;
    va_list ap;

    /* Temp args */
    unsigned int n;
    const char* s;

    /* Modifiers */
    bool is_signed;
    int min_width;
    char pad_character;
    char pad_right;

    va_start(ap, fmt);

    i = 0;
    while(true) {
        c = fmt[i];
        if(c == '\0') {
            return 0;
        } else if(c != '%') {
            putchar(c);
        } else {
            is_signed = true;
            min_width = 0;
            pad_character = ' ';
            pad_right = 0;
            while(true) {
                i++;
                c = fmt[i];
                if(c == '\0') {
                    return 1;
                } else if(c == '%') {
                    putchar('%');
                } else if(c == 'u') {
                    is_signed = false;
                } else if(c == 'd') {
                    n = va_arg(ap, unsigned int);
                    _format_d(n, 10, is_signed);
                    _pad_format_buffer(min_width, pad_character, pad_right);
                    puts(format_buffer);
                    break;
                } else if(c == 's') {
                    s = va_arg(ap, const char*);
                    strncpy(format_buffer, s, 256);
                    _pad_format_buffer(min_width, pad_character, pad_right);
                    puts(format_buffer);
                    break;
                } else if(c == 'b') {
                    n = va_arg(ap, unsigned int);
                    _format_d(n, 2, false);
                    _pad_format_buffer(min_width, pad_character, pad_right);
                    puts(format_buffer);
                    break;
                } else if(c == 'x') {
                    n = va_arg(ap, unsigned int);
                    _format_d(n, 16, false);
                    _pad_format_buffer(min_width, pad_character, pad_right);
                    puts(format_buffer);
                    break;
                } else if(c == '-') {
                    pad_right = 1;
                } else if(strchr("0123456789", c) != NULL) {
                    if(c == '0' && min_width == 0) {
                        pad_character = '0';
                    } else {
                        min_width *= 10;
                        min_width += c - '0';
                    }
                } else {
                    return 1;
                }
            }
        }
        i++;
    }
}
