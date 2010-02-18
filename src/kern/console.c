
#define __CONSOLE_DECLARED__
#include <console.h>
#include <stddef.h>

struct console_t console;

void console_preinit(void) {
    console.puts = NULL;
    console.putchar = NULL;
    console.set_cursor_position = NULL;
    console.get_cursor_position = NULL;
}

void console_puts(const char* s) {
    if(console.puts != NULL) {
        console.puts(s);
    }
}

void console_putchar(char c) {
    if(console.putchar != NULL) {
        console.putchar(c);
    }
}

void console_set_cursor(unsigned short r, unsigned short c) {
    if(console.set_cursor_position != NULL) {
        console.set_cursor_position(r, c);
    }
}

unsigned long console_get_cursor(void) {
    if(console.get_cursor_position != NULL) {
        return console.get_cursor_position();
    } else {
        return ((unsigned long)(-1));
    }
}
