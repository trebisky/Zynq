/* mapper.c
 *
 * Tom Trebisky  6-22-2-24
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

char * im_file = "nand.bin";

/* On a 64 bit machine, with gcc * an int is 4 bytes,
 *  so 256M becomes 64M words
 *  and 32M becomes 8M words
 */
// #define IMSIZE	64*1024*1024
#define IMSIZE	8*1024*1024

unsigned int image[IMSIZE];

void scan_file ( unsigned int * );

int
main ( int argc, char **argv )
{
	int fd;
	int n;

	// printf ( "%d\n", sizeof(int) );
	// printf ( "%d\n", sizeof(image) );

	fd = open ( im_file, O_RDONLY );
	n = read ( fd, image, IMSIZE );
	close ( fd );
	if ( n != IMSIZE ) {
	    fprintf ( stderr, "Bad read\n" );
	    exit ( 1 );
	}

	scan_file ( image );
}

char *mname[] = { "unk", "Zer", "One" };
enum em { UNK, ZER, ONE };

void
scan_file ( unsigned int *im )
{
	int i;
	enum em mode;
	enum em new;
	int count;
	int start;
	int addr1, addr2;

	mode = UNK;
	count = 0;
	start = 0;
	for ( i=0; i < IMSIZE; i++ ) {
	    if ( im[i] == 0 )
		new = ZER;
	    else if ( im[i] == 0xffffffff )
		new = ONE;
	    else
		new = UNK;

	    if ( new == mode ) {
		++count;
		continue;
	    }

	    // if ( count != 0 ) {
	    if ( count > 256 ) {
		addr1 = start*4;
		addr2 = (start+count-1)*4;
		printf ( "%s: %08x - %08x (%d)\n", mname[mode], addr1, addr2, count );
	    }

	    mode = new;
	    count = 1;
	    start = i;
	}
}

/* the END */
