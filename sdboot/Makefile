# Makefile for zynq "fsbl" bare metal code
# Tom Trebisky  1-17-2025
# tom@mmto.org

BOARD = zzyzx
#CROSS_COMPILE = arm-linux-gnu-
CROSS_COMPILE = arm-none-eabi-

# -------------------------------------

OBJS = start.o main.o

TARGET = $(BOARD).bin

# CFLAGS		:=	-g -Wall -Wextra -ffreestanding -fno-builtin -mlittle-endian
#CFLAGS		:=	-g -Wall -ffreestanding -fno-builtin -mlittle-endian
CFLAGS		:=	-g -Wall -ffreestanding -fno-builtin
CFLAGS		+= -march=armv7-a
CFLAGS		+= -I.

LDFLAGS		:=	-T spl.lds \
			-nostdlib \
			-z noexecstack --no-warn-rwx-segments

CC			=	$(CROSS_COMPILE)gcc $(CFLAGS)
#LD 			=	$(CROSS_COMPILE)gcc $(LDFLAGS)
LD 			=	$(CROSS_COMPILE)ld $(LDFLAGS)
OBJCOPY			=	$(CROSS_COMPILE)objcopy
DUMP			=	$(CROSS_COMPILE)objdump

# This gives us dependencies in .d files.
# CFLAGS		+= -MMD
# This gives us a map file.
# CFLAGS		+= -Wl,-Map=$(BOARD).map,--cref \

.c.o:
	@echo " [CC]   $<"
	@$(CC) $< -c -o $@

.S.o:
	@echo " [CC]   $<"
	@$(CC) $< -c -o $@

# -------------------------------------

all: mkzboot $(TARGET)

mkzboot: mkzboot.c
	cc -o mkzboot mkzboot.c

help:
	$(CC) --help

$(BOARD).elf: $(OBJS)
	$(LD) $(OBJS) -o $(BOARD).elf

$(TARGET): $(BOARD).elf
	@#echo " [IMG]  $(TARGET)
	@$(OBJCOPY) -O binary $(BOARD).elf $(TARGET)

dis: $(BOARD).elf
	$(DUMP) -d $(BOARD).elf -z >$(BOARD).dis

fetch:
	cp /u1/Projects/Zynq/Git_zynq/secret/lots.bin .
	cp /u1/Projects/Zynq/Git_zynq/secret/loader.py  zloader.py

test:
	./loader.py zzyzx.bin

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.img
	rm -f *.elf
	rm -f *.bin
	rm -f *.map
	rm -f *.dis
#	rm -f *.d

# THE END
