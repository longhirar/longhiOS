#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/font8x8.h>

const multiboot_info_t* mbd;

static size_t terminal_row;
static size_t terminal_column;
static size_t terminal_width;
static size_t terminal_height;
static struct pixel terminal_fgcolor;
static struct pixel terminal_bgcolor;

struct pixel* get_pixel(uint32_t x, uint32_t y) {
    // Assuming 32 bits per pixel (4 bytes)
    uint32_t pixels_per_row = mbd->framebuffer_width;
    return (struct pixel*)(mbd->framebuffer_addr + (y * pixels_per_row + x) * sizeof(struct pixel));
}

void putpixel(struct pixel* pxin, int x, int y) {
    struct pixel* px = get_pixel(x, y);
    px->red     = pxin->red;
    px->green   = pxin->green;
    px->blue    = pxin->blue;
    px->alpha   = pxin->alpha;
}

void terminal_initialize(multiboot_info_t* _mbd) {
    mbd = _mbd;

    terminal_bgcolor.red   = 0;
    terminal_bgcolor.green = 0;
    terminal_bgcolor.blue  = 0;
    terminal_bgcolor.alpha = 255;

    terminal_fgcolor.red   = 255;
    terminal_fgcolor.green = 255;
    terminal_fgcolor.blue  = 255;
    terminal_fgcolor.alpha = 255;

    if ((mbd->flags >> 12 & 0x01) != 0x01) {
        //panic("No framebuffer available!");
    }
    
    if (mbd->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED) {
        // TODO: implement indexed framebuffer type
        // panic("Indexed Color Not Implemented");
    } else if (mbd->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {
        terminal_column = 0;
        terminal_row = 0;
        terminal_width = mbd->framebuffer_width / 8;    // font size
        terminal_height = mbd->framebuffer_height / 8;  // font size

        if (mbd->framebuffer_addr != 0) {
            // Fill the screen with a horizontal gradient
            uint32_t width = mbd->framebuffer_width;
            uint32_t height = mbd->framebuffer_height;

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
    } else if (mbd->framebuffer_type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT) {
        // TODO: implement ega text framebuffer type
        // acctually no
        // fuck this
        // panic("Indexed Color Not Implemented");
    } else {
        // what the fuck is this ???
        //panic("Unknown framebuffer type??");
    }
}

void terminal_drawchar(unsigned char c, int x, int y, struct pixel* fgcolor, struct pixel* bgcolor)
{
    x += 10;
    y += 20;
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};
	unsigned char *glyph=font8x8_basic[c];
 
	for(cy=0;cy<8;cy++){
		for(cx=0;cx<8;cx++){
			putpixel(glyph[cy]&mask[cx]?fgcolor:bgcolor,x+cx,y+cy-12);
		}
	}
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	if (c == '\n') {
		terminal_column = 0;
		terminal_row++;
	} else {
		terminal_drawchar(uc, terminal_column*8, terminal_row*8, &terminal_fgcolor, &terminal_bgcolor);
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