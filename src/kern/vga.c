
#include <arch/misc.h>
#include <vga.h>

static struct VGA_State state;

void vga_init(void) {
    uint8_t temp;

    /* Initialize internal VGA state */
    state.position = 0;
    vga_set_attribute(VGA_ATTRIBUTE_DEFAULT);

    /* Map video memory to 0xb8000-0xbffff, enable odd/even page map access, and
       use text mode */
    outb(VGA_REGISTER_GRAPHICS_ADDR, 0x06);
    outb(VGA_REGISTER_GRAPHICS_DATA, (3 << 2) | (1 << 1) | 0);
    
    /* Retrive and update attribute control register to disable blink */
    inb(VGA_REGISTER_INPUT_STATUS);
    outb(VGA_REGISTER_ATTRIBUTE_ADDRDATA, (0x20 | 0x10));
    temp = inb(VGA_REGISTER_ATTRIBUTE_DATAREAD);
    temp &= ~(1 << 3); /* Clear blink-enable bit */
    outb(VGA_REGISTER_ATTRIBUTE_ADDRDATA, temp);

    /* Enable cursor */
    outb(VGA_REGISTER_CRT_ADDR, 0x0a);
    temp = inb(VGA_REGISTER_CRT_DATA);
    temp &= ~(1 << 5); /* Clear cursor disable bit */
    outb(VGA_REGISTER_CRT_DATA, temp);

    /* Clear screen */
    vga_clear();
}

/* Reset display -- empty screen and place cursor at 0, 0 */
void vga_clear(void) {
    for(int i = 0; i < VGA_COLUMN_COUNT * VGA_ROW_COUNT; i++) {
        VGA_ADDRESS_SPACE[i] = 0x0000;
    }

    state.position = 0;
}

/* Set the attribute to use when writing characters */
void vga_set_attribute(uint8_t attribute) {
    state.attribute = attribute << 8;
}

/* Scroll the screen by one line */
void vga_scroll_up(void) {
    for(int i = 0; i < ((VGA_ROW_COUNT - 1) * VGA_COLUMN_COUNT); i++) {
        VGA_ADDRESS_SPACE[i] = VGA_ADDRESS_SPACE[i + VGA_COLUMN_COUNT];
    }
    
    for(int i = (VGA_ROW_COUNT - 1) * VGA_COLUMN_COUNT; i < VGA_ROW_COUNT * VGA_COLUMN_COUNT; i++) {
        VGA_ADDRESS_SPACE[i] = state.attribute | ' ';
    }

    state.position -= VGA_COLUMN_COUNT;
}

/* Write a character to the screen */
void vga_putc(char c) {
    if(c == '\n') {
        state.position += VGA_COLUMN_COUNT - (state.position % VGA_COLUMN_COUNT);
    } else if(c == '\t') {
        state.position = (state.position & (~7)) + 8;
    } else if(c != '\r') {
        VGA_ADDRESS_SPACE[state.position] = state.attribute | c;
        state.position++;
    }
    
    if(state.position >= VGA_COLUMN_COUNT * VGA_ROW_COUNT) {
        vga_scroll_up();
    }
    
    vga_update_cursor();
}

void vga_update_cursor(void) {
    outb(VGA_REGISTER_CRT_ADDR, 0x0e);
    outb(VGA_REGISTER_CRT_DATA, (uint8_t) (state.position >> 8));
    outb(VGA_REGISTER_CRT_ADDR, 0x0f);
    outb(VGA_REGISTER_CRT_DATA, (uint8_t) (state.position & 255));
}

void vga_puts(const char* s) {
    while(*s != '\0') {
        vga_putc(*s);
        s++;
    }
}
