/* Extremely simple (and almost unnecessary)
 * assembly language startup file.
 * Tom Trebisky  1-18-2025
 *
 * We cannot put the stack at 0x3_0000
 * We do have 256K of OCM, but the upper 64K
 * is mapped elsewhere.  But we can just
 * restrict ourselves to 192K and put the
 * stack at 0x28000 and all is well,
 * especially for this little demo.
 */

	.global start
start:
	ldr	sp, =0x28000
	bl		main

spin:	b		spin

# Idiotic junk to avoid complaints from linker
.section        .note.GNU-stack,"",%progbits

// THE END
