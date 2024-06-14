# Makefile for the Zynq fsbl
# Tom Trebisky  6-10-2024

CC := arm-none-eabi-gcc
AS=arm-none-eabi-gcc

# This message length switch is for diagnostic messages
# You can tell it to try to format messages to a certain length.
# zero says "no wrapping", let them be as long as need be.
#CFLAGS=-Wall -O0 -g3 -c -fmessage-length=0
CFLAGS=-Wall -O0 -g3 -c
ECFLAGS = -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard

GCCVERSION = `$(CC) -dumpversion`
#ABI = -isystem /usr/lib/gcc/arm-none-eabi/$(GCCVERSION)/include
LIBS = -L /usr/lib/gcc/arm-none-eabi/$(GCCVERSION) -lgcc

#INCLUDEPATH := -I$(BSP_DIR)/ps7_cortexa9_0/include -I. -I$(BSP_DIR)/$(BOARD)
INCLUDEPATH := -I./include -I.
LIBPATH := ./lib

ASM_OBJS = asm_vectors.o boot.o xil-crt0.o translation_table.o cpu_init.o
CPU_OBJS = vectors.o xil_exception.o xtime_l.o ps7_init.o
X_OBJS = fsbl.o ffs.o drivers.o
U_OBJS = utils.o xil_printf.o
CORE_OBJS =

OBJS = $(ASM_OBJS) $(CPU_OBJS) $(X_OBJS) $(CORE_OBJS) $(U_OBJS)

%.o:%.c
	$(CC) $(CC_FLAGS) $(CFLAGS) $(ECFLAGS) -c $< -o $@ $(INCLUDEPATH)

%.o:%.S
	$(AS) $(CC_FLAGS) $(CFLAGS) $(ECFLAGS) -c $< -o $@ $(INCLUDEPATH)

# --- link stuff ----
# I tried using -nostdlib, but their code was looking for __libc_init_array
# without it, the code looks for -sbrk and uses the standard crt0 in the library.
# I fix the issue with _sbrk and such (and the standard crt0) by just copying
# their libxil.a into ./lib, which I don't fully understand yet.
#
# The business of __libc_init_array and friends smells like C++.
# It remains to be seem if there is any C++ source in libxil.a
# If not, this is totally superfluous and the crt0.o should be fixed.
# If so, I am less than happy to have C++ entering the game.
# I have searched and don't see any C++ code anywhere in "embeddedsw"
#  so I think this is a generic framework that is entirely unnecessary.
# We will do away with it (and rework xil-crt0.S to do so).

# Without the specfile we get: "multiple definition of `_start'"
# Once we got rid of -lc in the link, we could get rid of the specfile.
#L1FLAGS = $(ECFLAGS) -Wl,-build-id=none -Tlscript.ld -L$(LIBPATH) -specs=Xilinx.spec -Wl,--no-warn-rwx-segments
#L1FLAGS = $(ECFLAGS) -Wl,-build-id=none -Tlscript.ld -L$(LIBPATH) -Wl,--no-warn-rwx-segments
# I just get rid of build-id also

LD=arm-none-eabi-gcc
#LD=arm-none-eabi-ld

# The -v switch has gcc tell you all that it is doing
#L1FLAGS = -v $(ECFLAGS) -Tlscript.ld -L$(LIBPATH) -Wl,--no-warn-rwx-segments
# Using gcc
L1FLAGS = $(ECFLAGS) -T lscript.ld -L$(LIBPATH) -Wl,--no-warn-rwx-segments
# We switch from using gcc to using ld for the link
#L1FLAGS = -T lscript.ld -L$(LIBPATH) --no-warn-rwx-segments

# -lc supplies: _exit(), _close(), _lseek(), _read(), _write(), _sbrk()
# These are referenced as follows:
# /usr/lib/gcc/arm-none-eabi/13.2.0/../../../../arm-none-eabi/bin/ld:
#     /usr/lib/gcc/arm-none-eabi/13.2.0/../../../../arm-none-eabi/lib/thumb/v7+fp/hard/libc.a(libc_a-exit.o): in function `exit':
#     /builddir/build/BUILD/newlib-4.4.0.20231231/build-newlib/arm-none-eabi/thumb/v7+fp/hard/newlib/
#          ../../../../../../newlib/libc/stdlib/exit.c:65:(.text.exit+0x1a): undefined reference to `_exit'
# !!! This hardly seems right for embedded software that doesn't run under linux

# We depend on libc for strlen, memset, and such.
#XIL_GROUP = -Wl,--start-group,-lxil,-lgcc,-lc,--end-group
#XIL_GROUP = -Wl,--start-group,-lxil,-lgcc,--end-group

#FFS_GROUP = -Wl,--start-group,-lxilffs,-lxil,-lgcc,-lc,--end-group
#RSA_GROUP = -Wl,--start-group,-lrsa,-lxil,-lgcc,-lc,--end-group

# We don't need librsa (since RSA is not selected).
# We get libffs via the "ffs" module
#L2FLAGS = $(XIL_GROUP) $(FFS_GROUP) $(RSA_GROUP)
#L2FLAGS = $(XIL_GROUP) $(FFS_GROUP)
#L2FLAGS = $(XIL_GROUP) -nostdlib

# Using gcc - we do need to explicitly link against libgcc
#L2FLAGS = -lxil -lgcc -nostdlib
# after we switch to ld rather than gcc
#L2FLAGS = -lxil -nostdlib $(LIBS)

# All done, No need for library
L2FLAGS = -lgcc -nostdlib

# ---------------------------------------------------------

all: fsbl.elf

fsbl.o:	bogus
	cd fsbl ; make
	cd ffs ; make
	cd drivers ; make

bogus:

fsbl.elf:	$(OBJS)
	$(LD) $(L1FLAGS)  -o $@ $(OBJS) $(L2FLAGS)

clean:
	rm -f fsbl.elf
	rm -f *.o
	cd fsbl ; make clean
	cd ffs ; make clean
	cd drivers ; make clean

showver:
	echo $(GCCVERSION)
	echo $(LIBS)

# THE END
