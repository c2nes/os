
#ifndef __OS_CONSOLE_H_
#define __OS_CONSOLE_H_

struct console_t {
    void (*puts)(const char*);
    void (*putchar)(char);
    void (*set_cursor_position)(unsigned short, unsigned short);
    unsigned long (*get_cursor_position)(void);

    /* some input commands go here... */
};

#ifndef __CONSOLE_DECLARED__
extern struct console_t console;
#endif

void console_preinit(void);
void console_puts(const char* s);
void console_putchar(char c);
void console_set_cursor(unsigned short r, unsigned short c);
unsigned long console_get_cursor(void);

#endif // #ifndef __OS_CONSOLE_H_
