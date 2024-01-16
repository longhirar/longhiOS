#include <stdio.h>
#include <stdbool.h>


#include <kernel/tty.h>
#include <kernel/multiboot2.h>
#include <cpuid.h>

void panic(char* msg) {
	terminal_paniccolor();
	printf("\nKERNEL PANIC!\nkernel: %s\n", msg);
}

void kernel_main(unsigned long mb_tags_addr, unsigned long magic, unsigned long font_data_addr) {
	struct multiboot_tag *tag;
	unsigned size;

	// first loop just to find the framebuffer tag
	size = *(unsigned *) mb_tags_addr;
	for (tag = (struct multiboot_tag*) (mb_tags_addr + 8);
		tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (struct multiboot_tag* ) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))
	) {
		printf("kernel (fb pass): Tag 0x%x | Size 0x%x\n", tag->type, tag->size);
		switch(tag->type) {
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
				printf("kernel: found framebuffer\n");

				struct multiboot_tag_framebuffer* fb = (struct multiboot_tag_framebuffer*) tag;

				terminal_initialize(fb, font_data_addr);
				break;
			}
			default: {
			}
		}
	}

	// then second loop for everything else
	// yes this is inefficient
	// but i wanna see the logs 
	size = *(unsigned *) mb_tags_addr;
	for (tag = (struct multiboot_tag*) (mb_tags_addr + 8);
		tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (struct multiboot_tag* ) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))
	) {
		printf("kernel: Tag 0x%x | Size 0x%x\n", tag->type, tag->size);
		switch(tag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE: {
				printf("kernel: cmdline: %s\n", ((struct multiboot_tag_string*) tag)->string);
				break;
			}
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: {
				printf("kernel: bootloader_name: %s\n", ((struct multiboot_tag_string*) tag)->string);
				break;
			}
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
				// we already did this
				break;
			}
			default: {
			}
		}
	}

	terminal_warncolor();
	printf("longhiOS 0.1\n");
	terminal_infocolor();
	printf("kernel: kernel_main\n");
	printf("kernel: font data addr %x\n", font_data_addr);

	int i;
	for (int i = 640; i < 656; i++) {
        printf("%x ", ((unsigned char*)font_data_addr)[i]);
    }
	printf("\n");
	
	printf("kernel: test print A\n\n");

	

}

