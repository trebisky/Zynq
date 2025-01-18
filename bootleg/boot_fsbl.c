/* boot_fsb.c
 *
 * This program has one purpose, to extract the fsbl
 *  from a boot image
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

char *boot_image = "nand.bin";

typedef unsigned int u32;



/* The following is from bootgen/bootheader-zynq.h
 */
#define MAX_BOOT_VECTORS                        8
#define UDF_BH_SIZE_ZYNQ    19

struct zynq_bootheader {
    u32 bootVectors [MAX_BOOT_VECTORS];    //  (0x00)
    u32 widthDetectionWord;                //  (0x20)
    u32 identificationWord;                //  (0x24)
    u32 encryptionKeySource;               //  (0x28)
    u32 headerVersion;                     //  (0x2C)
    u32 sourceOffset;                      //  (0x30)
    u32 fsblLength;                        //  (0x34)
    u32 fsblLoadAddress;                   //  (0x38)
    u32 fsblExecAddress;                   //  (0x3C)
    u32 totalFsblLength;                   //  (0x40)
    u32 qspiConfigWord;                    //  (0x44)
    u32 headerChecksum;                    //  (0x48)
    u32 udf [UDF_BH_SIZE_ZYNQ];            //  (0x4C)
    u32 imageHeaderByteOffset;             //  (0x98)
    u32 partitionHeaderByteOffset;         //  (0x9C)
};

/* On a 64 bit machine, with gcc an int is 4 bytes */

#define IMSIZE	32*1024*1024

unsigned char image[IMSIZE + 4];
int im_size;

void load_image ( char * );
void save_fsbl ( char *, unsigned char *, int );

int
main ( int argc, char **argv )
{
	struct zynq_bootheader *zbp;

	// printf ( "argc: %d\n", argc );
	if ( argc > 1 )
	    boot_image = argv[1];

	printf ( "Boot image: %s\n", boot_image );
	load_image ( boot_image );
	printf ( "Image read, %d bytes\n", im_size );

	zbp = (struct zynq_bootheader *) image;
	printf ( "FSBL offset: 0x%08x\n", zbp->sourceOffset );
	printf ( "FSBL size: 0x%08x %d\n", zbp->fsblLength, zbp->fsblLength );
	printf ( "FSBL load address: 0x%08x\n", zbp->fsblLoadAddress );
	printf ( "FSBL exec address: 0x%08x\n", zbp->fsblExecAddress );
	// printf ( "FSBL size2: %08x\n", zbp->totalFsblLength );

	save_fsbl ( "fsbl.out", &image[zbp->sourceOffset], zbp->fsblLength );
}

void
load_image ( char *im_path )
{
	int fd;
	int n;

	fd = open ( im_path, O_RDONLY );
	if ( fd < 0 ) {
	    fprintf ( stderr, "Cannot open image\n" );
	    exit ( 1 );
	}

	n = read ( fd, image, IMSIZE + 4 );
	close ( fd );

	if ( n > IMSIZE ) {
	    fprintf ( stderr, "Image is too big\n" );
	    exit ( 1 );
	}
	if ( n <= 0 ) {
	    fprintf ( stderr, "Read error\n" );
	    exit ( 1 );
	}
	im_size = n;
}

void
save_fsbl ( char *path, unsigned char *im, int size )
{
	int fd;
	int n;

	fd = open ( path, O_CREAT | O_TRUNC | O_WRONLY, 0644 );
	if ( fd < 0 ) {
	    fprintf ( stderr, "Cannot create fsbl\n" );
	    exit ( 1 );
	}
	n = write ( fd, im, size );
	if ( n != size ) {
	    fprintf ( stderr, "Cannot write fsbl\n" );
	    exit ( 1 );
	}
	close ( fd );
}

/* THE END */
