Zynq projects - fsbl

This "project" is entirely source code from Xilinx

I obtained their "embeddedsw" package, and spent nearly a week
pulling the fsbl files out of it and rearranging them.

The purpose of all that effort is to be able to study their
code without tripping over all kinds of dead wood and files
that are not actually being used.

I may also trim the comments contained in the source files,
many of which record version history that is only of interest
to Xilinx.  I will take care to retain their copyrights.

They offer this code for anyone to use and modify.
I have little if any interest in actually running it,
but I certainly could if I wished to.

---------------------------------

Top level files are either the first files where the code
starts up -- or files that I have modified or added for
some reason.

The fsbl specific main.c is in the "fsbl" directory.

The "drivers" directory holds most of the code.
