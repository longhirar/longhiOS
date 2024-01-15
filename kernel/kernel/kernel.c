#include <stdio.h>
#include <stdbool.h>


#include <kernel/tty.h>
#include <kernel/multiboot.h>

#include <cpuid.h>

void panic(char* msg) {
	terminal_paniccolor();
	printf("\nKERNEL PANIC!\nkernel: %s\n", msg);
}

void reverse(char* str, size_t length) {
    size_t start = 0;
    size_t end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa_hex(unsigned long value, char* buffer, size_t width) {
    if (value == 0) {
        for (size_t i = 0; i < width - 1; ++i) {
            buffer[i] = '0';
        }
        buffer[width - 1] = '0';
        buffer[width] = '\0';
        return buffer;
    }

    size_t index = 0;

    while (value != 0) {
        unsigned int remainder = value % 16;
        buffer[index++] = (remainder < 10) ? (char)(remainder + '0') : (char)(remainder - 10 + 'A');
        value = value / 16;
    }

    // Fill remaining width with zeros
    while (index < width) {
        buffer[index++] = '0';
    }

    buffer[index] = '\0';

    reverse(buffer, index);

    return buffer;
}
void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
	terminal_initialize();
	terminal_warncolor();
	printf("longhiOS 0.1\n");
	terminal_infocolor();
	printf("kernel: kernel_main\n");
	printf("kernel: cmdline: %s\n", mbd->cmdline);

	if(!mbd->flags >> 6 & 0x01)
		panic("invalid memory map given by bootloader.");

	int i;
	for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i);

        char addr_str[20];
        char len_str[20];
        char size_str[20];

        itoa_hex(mmmt->addr, addr_str, 8);
        itoa_hex(mmmt->len, len_str, 8);
        itoa_hex(mmmt->size, size_str, 8);

		char* type_str;
		if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
			type_str = "AVAILABLE";
		else if (mmmt->type == MULTIBOOT_MEMORY_RESERVED)
			type_str = "RESERVED";
		else if (mmmt->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE)
			type_str = "ACPI RECLAIMABLE";
		else if (mmmt->type == MULTIBOOT_MEMORY_NVS)
			type_str = "NVS";
		else if (mmmt->type == MULTIBOOT_MEMORY_BADRAM)
			type_str = "BAD RAM";
		else 
			type_str = "UNKNOWN";

        printf("kernel: addr %s / length %s / size %s / type %s\n",
               addr_str, len_str, size_str, type_str);
    }

	

	if(true)
		panic("reached the end of the kernel!");
}

