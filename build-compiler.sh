#!bash
export PROJECT_DIR="$PWD"
export SRC_DIR="$PROJECT_DIR/src"
export BUILD_DIR="$PROJECT_DIR/build"
export TEST_DIR="$PROJECT_DIR/test"
export SYSROOT="$PROJECT_DIR/sysroot"

export PATH="$PROJECT_DIR/cross/bin:$PATH" # add cross-compiler binaries to path
export CC="i686-elf-gcc"

export PREFIX="$PROJECT_DIR/cross"
export TARGET=i686-elf


DID_DOWNLOAD_NEW=false
if [ -d "$PROJECT_DIR/binutils" ]; then
    echo "binutils's source is already present. skipping download."
else
    git clone git://sourceware.org/git/binutils-gdb.git $PROJECT_DIR/binutils
    DID_DOWNLOAD_NEW=true
fi

if [ -d "$PROJECT_DIR/gcc" ]; then
    echo "gcc's source is already present. skipping download."
else
    git clone git://gcc.gnu.org/git/gcc.git $PROJECT_DIR/gcc
    DID_DOWNLOAD_NEW=true
fi

if DID_DOWNLOAD_NEW; then
    echo "Things have been downloaded that weren't present before. If your target is x86_64, then GCC configuration needs to be modified before you can run mk-compiler again. Follow the instructions at https://wiki.osdev.org/Libgcc_without_red_zone"
    echo "If your target is not x86_64, you can run mk-compiler again with no modification."
    exit
fi

rm -rf "$PROJECT_DIR/build-binutils"
mkdir "$PROJECT_DIR/build-binutils"
cd "$PROJECT_DIR/build-binutils"
$PROJECT_DIR/binutils/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j 12
make -j 12 install

cd $PROJECT_DIR
rm -rf "$PROJECT_DIR/build-gcc"
mkdir "$PROJECT_DIR/build-gcc"
cd "$PROJECT_DIR/build-gcc"
$PROJECT_DIR/gcc/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make -j 12 all-gcc
make -j 12 all-target-libgcc
make -j 12 install-gcc
make -j 12 install-target-libgcc