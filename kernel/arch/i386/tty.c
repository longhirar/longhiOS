#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/multiboot2.h>

static struct multiboot_tag_framebuffer* fb;
static struct multiboot_tag_framebuffer_common* fbc;

static size_t font_data_addr;
static size_t terminal_row;
static size_t terminal_column;
static size_t terminal_width;
static size_t terminal_height;
static struct pixel terminal_fgcolor;
static struct pixel terminal_bgcolor;

struct pixel* get_pixel(uint32_t x, uint32_t y) {
    // Assuming 32 bits per pixel (4 bytes)
    uint32_t pixels_per_row = fbc->framebuffer_width;
    return (struct pixel*)(fbc->framebuffer_addr + (y * pixels_per_row + x) * sizeof(struct pixel));
}

void putpixel(struct pixel* pxin, int x, int y) {
    struct pixel* px = get_pixel(x, y);
    px->red     = pxin->red;
    px->green   = pxin->green;
    px->blue    = pxin->blue;
    px->alpha   = pxin->alpha;
}

void terminal_initialize(struct multiboot_tag_framebuffer* fb, unsigned long _font_data_addr) {
    fbc = &fb->common;
    font_data_addr = _font_data_addr;

    terminal_bgcolor.red   = 0;
    terminal_bgcolor.green = 0;
    terminal_bgcolor.blue  = 0;
    terminal_bgcolor.alpha = 255;

    terminal_fgcolor.red   = 255;
    terminal_fgcolor.green = 255;
    terminal_fgcolor.blue  = 255;
    terminal_fgcolor.alpha = 255;
    
    if (fbc->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED) {
        // TODO: implement indexed framebuffer type
        // panic("Indexed Color Not Implemented");
    } else if (fbc->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {
        terminal_column = 0;
        terminal_row = 0;
        terminal_width = fbc->framebuffer_width / 8;    // font size
        terminal_height = fbc->framebuffer_height / 8;  // font size

        if (fbc->framebuffer_addr != 0) {
            // Fill the screen with a horizontal gradient
            uint32_t width = fbc->framebuffer_width;
            uint32_t height = fbc->framebuffer_height;

            for (uint32_t y = 0; y < height; ++y) {
                for (uint32_t x = 0; x < width; ++x) {

                    struct pixel newpixel;
                    newpixel.red    = (x * 255) / width;        // Red
                    newpixel.green  = (y * 255) / height;       // Green
                    newpixel.blue   = 255 - (x * 255) / width;  // Blue
                    newpixel.alpha  = 255;
                    putpixel(&newpixel, x, y);

                }
            }
        }
    } else if (fbc->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT) {
        // TODO: implement ega text framebuffer type
        // acctually no
        // fuck this
        // panic("Indexed Color Not Implemented");
    } else {
        // what the fuck is this ???
        //panic("Unknown framebuffer type??");
    }
}

void terminal_drawchar(unsigned long c, int x, int y, struct pixel* fgcolor, struct pixel* bgcolor) {
    c -= 32;
    // Each character is 16x32 with 8bpp, so each row of pixels is 16 bytes
    int charWidth = 8;
    int charHeight = 16;
    int bytesPerPixel = 1; // Assuming 8bpp

    unsigned long offset = 0x80 /* 8 * 16 in hex */ * c;
    // Iterate over each row of the character
    for (int row = 0; row < charHeight; row++) {
        // Iterate over each pixel in the row
        for (int col = 0; col < charWidth; col++) {
            // Calculate the offset for the current pixel in the font data
            unsigned long pixelOffset = offset + row * charWidth * bytesPerPixel + col * bytesPerPixel;

            // Extract the pixel values from font data
            uint8_t pixelValue = (uint8_t)((unsigned char*)font_data_addr)[pixelOffset];

            // Assuming a simple conversion from 8bpp to RGBA8888
            struct pixel pixel = { pixelValue, pixelValue, pixelValue, 255 };

            // Update the framebuffer using putpixel
            putpixel(&pixel, x + col, y + row);
        }
    }
}



void terminal_putchar(char c) {
	unsigned char uc = c;
	if (c == '\n') {
		terminal_column = 0;
		terminal_row++;
	} else {
		terminal_drawchar(uc, terminal_column*8, terminal_row*16, &terminal_fgcolor, &terminal_bgcolor);
		if (++terminal_column == terminal_width) {
			terminal_column = 0;
			if (++terminal_row == terminal_width)
				terminal_row = 0;
		}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_infocolor(void) {

}

void terminal_warncolor(void) {

}

void terminal_errcolor(void) {

}

void terminal_paniccolor(void) {

}