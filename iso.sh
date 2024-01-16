#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/system.bin isodir/boot/system.bin
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "longhiOS" {
	multiboot2 /boot/system.bin
}
EOF
grub-mkrescue -o longhiOS.iso isodir
