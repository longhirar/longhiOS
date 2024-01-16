#define ASM_FILE 1
#include <kernel/multiboot2.h>

# Declare a header as in the Multiboot Standard.
.section .multiboot2
.align 8
multiboot2_header:
	.long MULTIBOOT2_HEADER_MAGIC 																		# magic
	.long MULTIBOOT_ARCHITECTURE_I386 																# architecture
	.long multiboot2_header_end - multiboot2_header																  		# header_length
	.long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot2_header_end - multiboot2_header))  	# checksum
framebuffer_tag:  
        .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
        .short 0 # not optional :)
        .long framebuffer_tag_end - framebuffer_tag
        .long 1600
        .long 900
        .long 32
framebuffer_tag_end:
        .short MULTIBOOT_HEADER_TAG_END
        .short 0
        .long 0
multiboot2_header_end:

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp

	# load memory map provided by bootloader into stack
	push $font_data
	push %eax
	push %ebx

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	pushl $halt_message
	call printf

	cli
1:	hlt
	jmp 1b
.size _start, . - _start

halt_message:
	.asciz "Halt!!"

.section .rodata
font_data:
.incbin "8x16.font"