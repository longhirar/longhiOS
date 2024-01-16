#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>
#include <stddef.h>
#include <kernel/multiboot2.h>

struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

void terminal_initialize(struct multiboot_tag_framebuffer* fb, unsigned long font_data_addr);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_infocolor(void);
void terminal_warncolor(void);
void terminal_errcolor(void);
void terminal_paniccolor(void);

void __terminal_drawchar(unsigned char c, int x, int y, struct pixel* fgcolor, struct pixel* bgcolor);
#endif
