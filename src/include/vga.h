
#ifndef __OS_VGA_H_
#define __OS_VGA_H_

#include <stdint.h>

#define VGA_REGISTER_GRAPHICS_ADDR 0x3ce
#define VGA_REGISTER_GRAPHICS_DATA 0x3cf
#define VGA_REGISTER_ATTRIBUTE_ADDRDATA 0x3c0
#define VGA_REGISTER_ATTRIBUTE_DATAREAD 0x3c1
#define VGA_REGISTER_CRT_ADDR 0x3d4
#define VGA_REGISTER_CRT_DATA 0x3d5
#define VGA_REGISTER_INPUT_STATUS 0x3da

#define VGA_COLOR_BLACK   0x0
#define VGA_COLOR_BLUE    0x1
#define VGA_COLOR_GREEN   0x2
#define VGA_COLOR_CYAN    0x3
#define VGA_COLOR_RED     0x4
#define VGA_COLOR_MAGENTA 0x5
#define VGA_COLOR_BROWN   0x6
#define VGA_COLOR_WHITE   0x7
#define VGA_COLOR_INTENSE(c) (0x8 | (c))

#define VGA_ATTRIBUTE_COLOR_BG(c) ((c) << 4)
#define VGA_ATTRIBUTE_COLOR_FG(c) (c)
#define VGA_ATTRIBUTE_COLOR(bg, fg) (VGA_ATTRIBUTE_COLOR_BG(bg) | VGA_ATTRIBUTE_COLOR_FG(fg))
#define VGA_ATTRIBUTE_DEFAULT VGA_ATTRIBUTE_COLOR(VGA_COLOR_BLACK, VGA_COLOR_WHITE)

#define VGA_ADDRESS_SPACE ((unsigned short*)0xb8000)

#define VGA_COLUMN_COUNT 80
#define VGA_ROW_COUNT 25

struct VGA_State {
    unsigned short position;
    uint16_t attribute;
};

void vga_init(void);
void vga_clear(void);
void vga_set_attribute(uint8_t attribute);
void vga_scroll_up(void);
void vga_update_cursor(void);
void vga_putc(char c);
void vga_puts(const char* s);

#endif // #ifndef __OS_VGA_H_
