Zynq projects - sdboot

This is a prelude to figuring out how to build
U-boot and get it onto an SD card.

The idea is to build a trivial executable
(some kind of "hello world" thing that writes
to the serial port).  Then figure out how to
get it onto an SD card and boot it.

This sort of thing is typically done to boot the
FSBL (first stage boot loader).  The bootrom
locates the image and copies it to OCM (on chip
memory), then branches to it.

OCM on the Zynq is at address 0.  There is 256K
of OCM, but we only get to use 192K of it.  The
details as to why can be found in my bootrom notes,
but don't really matter as 192K will be more than
enough for this experiment.

SD card booting is different from booting from other
media on the Zynq.  For an SD card, there must be a
FAT filesystem in the first partition and it must
contain a file BOOT.BIN.  This file contains the
image to be booted, wrapped in a bootrom header.

Xilinx provides a utility "bootgen", but I have not
yet had luck using it.

One last note.  I will have to fully initialize the
serial port as the bootrom will not do that for me when
booting from SD card.

The original code came from the zzyzx project in my Antminer repo.
