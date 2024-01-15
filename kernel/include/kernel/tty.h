#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>
#include <stddef.h>
#include <kernel/multiboot.h>

struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

void terminal_initialize(multiboot_info_t* mbd);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_infocolor(void);
void terminal_warncolor(void);
void terminal_errcolor(void);
void terminal_paniccolor(void);
#endif
