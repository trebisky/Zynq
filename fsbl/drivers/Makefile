# Makefile for the Zynq fsbl - fsbl app core
# Tom Trebisky  6-10-2024

CC := arm-none-eabi-gcc
AS=arm-none-eabi-gcc
LD=arm-none-eabi-gcc

#CFLAGS=-Wall -O0 -g3 -c -fmessage-length=0
CFLAGS=-Wall -O0 -g3
ECFLAGS = -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard

#INCLUDEPATH := -I$(BSP_DIR)/ps7_cortexa9_0/include -I. -I$(BSP_DIR)/$(BOARD)
INCLUDEPATH := -I../include -I.. -I.

# Removed by tjt 6-27-2024 to make room for NAND driver
#	xqspips.o  xqspips_g.o  xqspips_options.o  xqspips_sinit.o \

OBJS = inbyte.o outbyte.o sleep.o usleep.o \
	xdevcfg.o  xdevcfg_g.o  xdevcfg_intr.o  xdevcfg_sinit.o \
	xsdps.o  xsdps_card.o  xsdps_g.o  xsdps_host.o  xsdps_options.o  xsdps_sinit.o \
	xuartps_hw.o \
	xil_assert.o xil_cache.o xil_sleepcommon.o xil_util.o


%.o:%.c
	$(CC) $(CC_FLAGS) $(CFLAGS) $(ECFLAGS) -c $< -o $@ $(INCLUDEPATH)

%.o:%.S
	$(AS) $(CC_FLAGS) $(CFLAGS) $(ECFLAGS) -c $< -o $@ $(INCLUDEPATH)

# ---------------------------------------------------------

all: ../drivers.o

../drivers.o:		$(OBJS)
	$(LD) -r -o ../drivers.o $(OBJS)

clean:
	rm -rf *.o

# THE END
