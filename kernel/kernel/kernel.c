#include <stdio.h>
#include <stdbool.h>


#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <cpuid.h>

void panic(char* msg) {
	terminal_paniccolor();
	printf("\nKERNEL PANIC!\nkernel: %s\n", msg);
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
	terminal_initialize(mbd);
	terminal_warncolor();
	printf("longhiOS 0.1\n");
	terminal_infocolor();
	printf("kernel: kernel_main\n");
	printf("kernel: cmdline: %s\n", mbd->cmdline);

	if(!mbd->flags >> 6 & 0x01)
		panic("invalid memory map given by bootloader.");

    printf("kernel: multiboot framebuffer: %x %xx%x \n", mbd->framebuffer_addr, mbd->framebuffer_width, mbd->framebuffer_height);


    printf("kernel mmap_length: %d", mbd->mmap_length);
	int i;
	for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i);

		char types[5][16] = {
            "AVAILABLE",
            "RESERVED",
            "ACPI RECLAIMABLE",
            "NVS",
            "BAD RAM"
        };
		
        
        printf("kernel memmap: addr %x | size %x | len %x | type %d \n", mmmt->addr, mmmt->size, mmmt->len, mmmt->type);

    }

	putchar(0xFF);

	if(true)
		panic("reached the end of the kernel!");
}

