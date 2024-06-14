# Makefile for the Zynq fsbl - fsbl app core
# Tom Trebisky  6-10-2024

CC := arm-none-eabi-gcc
AS=arm-none-eabi-gcc
LD=arm-none-eabi-gcc

#CFLAGS=-Wall -O0 -g3 -c -fmessage-length=0
CFLAGS=-Wall -O0 -g3 -c
ECFLAGS = -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard

#INCLUDEPATH := -I$(BSP_DIR)/ps7_cortexa9_0/include -I. -I$(BSP_DIR)/$(BOARD)
INCLUDEPATH := -I../include -I.. -I.

ASM_OBJS = fsbl_handoff.o

# We can do without rsa.c, but we need md5.c
#CORE_OBJS = fsbl_hooks.o  image_mover.o  main.o  md5.o  nand.o  nor.o  pcap.o  qspi.o  rsa.o  sd.o
CORE_OBJS = fsbl_hooks.o  image_mover.o  main.o  md5.o  nand.o  nor.o  pcap.o  qspi.o  sd.o

OBJS = $(ASM_OBJS) $(CORE_OBJS)

%.o:%.c
	$(CC) $(CC_FLAGS) $(CFLAGS) $(ECFLAGS) -c $< -o $@ $(INCLUDEPATH)

%.o:%.S
	$(AS) $(CC_FLAGS) $(CFLAGS) $(ECFLAGS) -c $< -o $@ $(INCLUDEPATH)

# ---------------------------------------------------------

all: ../fsbl.o

../fsbl.o:		$(OBJS)
	$(LD) -r -o ../fsbl.o $(OBJS)

clean:
	rm -rf *.o

# THE END
