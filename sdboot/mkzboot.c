/* mkzboot.c
 * "z" for Zynq
 * This program makes a boot.bin file given
 * an image we would like to boot.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

char *infile = "zzyzx.bin";
char *outfile = "BOOT.BIN";

typedef unsigned int u32;

struct bootheader {
		u32 itable[8];

		u32 width;
		u32 id;
		u32 encrypt;
		u32 user;				/* fsbl/user defined */

		u32 src_offset;			/* source offset */
		u32 length;
		u32 load;
		u32 start;

		u32 tlength;			/* total length */
		u32 qspi;
		u32 cksum;

		u32 user2[19];			/* 76 bytes */
		// u32 _pad1[2];
		u32 im_header_ptr;
		u32 part_header_ptr;
		u32 reg_init[512];		/* 2048 bytes */
		u32 im_header[8];		/* at 0x8a0 */
#ifdef PART_HEADER
		u32 part_header[16];
#endif
		// u32 _end;
};

/* With the 8 and 16 sizes for im_header
 * and part_header, the "_end" is at 0x900.
 * Omit the part_header and the image will
 * go to 0x8c0.
 * The image must start on a 64 byte boundary.
 * so either 8c0 or 900 or further along.
 */

struct bootheader bh;

int im_size = 0;

/* Max image is 3*64 = 192K */
#define IMSIZE	3*64*1024
char image[IMSIZE];

/* ============================================================== */

static void
error ( char *msg )
{
		fprintf ( stderr, "%s\n", msg );
		_exit ( 1 );
}

static void
read_image ( void )
{
		int fd;

		fd = open ( infile, O_RDONLY );
		if ( fd < 0 )
			error ( "Cannot open image" );
		im_size = read ( fd, image, IMSIZE );
		close ( fd );
		if ( im_size <= 0 )
			error ( "Cannot read image" );

		printf ( "Image read: %d bytes\n", im_size );
}

void
check_header ( void )
{
		char *p1, *p2;

		p1 = (char *) &bh.itable;
		p2 = (char *) &bh.reg_init;
		printf ( "p1, p2 = %08x, %08x\n", p1, p2 );
		printf ( "off = %08x\n", p2-p1 );
		p2 = (char *) &bh.im_header;
		printf ( "p1, p2 = %08x, %08x\n", p1, p2 );
		printf ( "off = %08x\n", p2-p1 );
#ifdef PART_HEADER
		p2 = (char *) &bh.part_header;
		printf ( "p1, p2 = %08x, %08x\n", p1, p2 );
		printf ( "off = %08x\n", p2-p1 );
#endif
		//p2 = (char *) &bh._end;
		//printf ( "p1, p2 = %08x, %08x\n", p1, p2 );
		//printf ( "off = %08x\n", p2-p1 );
}

/* sum from 0x20 to 0x44,
 * then invert the result
 */
u32
sum_header ( struct bootheader *bp )
{
		u32 sum = 0;
		u32 *up;

		for ( up = &bp->width; up < &bp->cksum; up++ )
			sum += *up;

		return 0xffffffff - sum;
}

#define NDATA 10
u32 data[] = {
	0xaa995566, 0x584c4e58, 0x00000000, 0x01010000,
	0x00001700, 0x0001c014, 0x00000000, 0x00000000,
	0x0001c014, 0x00000001
};
// We expect sum = 0xfc15c518

void
sum_tester ( void )
{
		u32 sum = 0;
		u32 *up, *end;
		u32 rv;

		end = &data[NDATA];
		for ( up = &data[0]; up < end; up++ )
			sum += *up;

		rv = 0xffffffff - sum;
		printf ( "SUM = %08x\n", rv );
		// We get: SUM = fc15c518
}

void
init_header ( void )
{
		struct bootheader *bp = &bh;
		int i;

		for ( i=0; i < 8; i++ )
			bp->itable[i] = 0xeafffffe;

		bp->width = 0xaa995566;
		bp->id =    0x584c4e58;		// 'XLNX'
		bp->encrypt = 0;
		bp->user = 0x01010000;		// bootrom version

		/* We can put the image here (and omit the
		 * part_header from the structure).
		 */
		bp->src_offset = 0x8c0;

		bp->length = im_size;
		bp->load = 0;				// load here
		bp->start = 0;				// start here
		bp->tlength = im_size;
		bp->qspi = 1;				// always
		bp->cksum = sum_header ( bp );

		for ( i=0; i < 19; i++ )
			bp->user2[i] = 0;

		/* not in the TRM */
		bp->im_header_ptr = 0x8c0;
		bp->part_header_ptr = 0x8c0;

		/* pairs of register initialization info
		 * address, then data
		 * address of 0xffff_ffff ends the list
		 */
		for ( i=0; i < 512; i += 2 ) {
			bp->reg_init[i] = 0xffffffff;
			bp->reg_init[i+1] = 0;
		}

		/* data can be anything apparently */
		for ( i=0; i < 8; i++ )
			bp->im_header[i] = 0xaaaaaaaa;

#ifdef PART_HEADER
		for ( i=0; i < 16; i++ )
			bp->part_header[i] = 0xaaaaaaaa;
#endif
}

int
main ( int argc, char **argv )
{
		int fd;

		/* read image first to get length */
		read_image ();

		// sum_tester ();
		// check_header ();
		init_header ();

		fd = open ( outfile, O_WRONLY | O_CREAT, 0644 );
		if ( fd < 0 )
			error ( "Cannot open Output" );

		write ( fd, (char *) &bh, sizeof(struct bootheader) );
		write ( fd, image, im_size );

		close ( fd );
}

/* THE END */
