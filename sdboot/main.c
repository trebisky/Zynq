/* First bare metal program for the EBAZ4205
 *
 * Tom Trebisky  1-9-2021
 */

typedef volatile unsigned int vu32;
typedef unsigned int u32;

#define BIT(x)	(1<<(x))

/* The TRM keeps the register details in Appendix B
 */
struct zynq_uart {
	vu32	control;		/* 00 */
	vu32	mode;
	vu32	ie;
	vu32	id;

	vu32	im;				/* 10 */
	vu32	istatus;
	vu32	baud_gen;
	vu32	rto;

	vu32	rfifo;			/* 20 */
	vu32	mcontrol;
	vu32	mstatus;
	vu32	cstatus;

	vu32	fifo;			/* 30 */
	vu32	baud_div;
	vu32	fc_delay;
	int	__pad1[2];

	vu32	tfifo;

};

#define UART0_BASE	((struct zynq_uart *) 0xe0000000)
#define UART1_BASE	((struct zynq_uart *) 0xe0001000)

/* The EBAZ uses UART1 */
#define UART_BASE	UART1_BASE

/* This is probably true, from a U-boot config file */
#define UART_CLOCK	100000000

/* Bits in the cstatus register */
#define CS_TXFULL	BIT(4)

/* ======================================================================= */
/* ======================================================================= */

#define SLCR_UNLOCK		0xf8000008
#define SLCR_LOCK		0xf8000004

/* We must unlock the slcr before we can write to
 * any registers in it.
 */
static void
slcr_unlock ( void )
{
		vu32 *rp = (vu32 *) SLCR_UNLOCK;

		*rp = 0xdf0d;
}

static void
slcr_lock ( void )
{
		vu32 *rp = (vu32 *) SLCR_LOCK;

		*rp = 0x767b;
}

#define NUM_MIO     54

struct mio_pins {
    vu32    ctrl[NUM_MIO];
};

#define MIO_PIN_BASE    (struct mio_pins *) 0xf8000700

/* Only the low 14 bits in the registers are used.
 */

static void
mio_pin ( int pin, u32 val )
{
	struct mio_pins *mp = MIO_PIN_BASE;

    mp->ctrl[pin] = val;
}

/*
; Set MIO pins for uart
; MIO pins are in the SLCR
; see page 52 in the TRM for a cute table
; page 1570 covers the SLCR,
; f800_07c0 is pin 48  Tx
; f800_07c4 is pin 49  Rx
; See page 1679, 1680 for these specific pins
; as you might expect this sets pin 48 to TxD and 49 to RxD
; (also some values get set in RAM here)
*/

#ifdef notdef
#define MIO_TX_PIN	0xf80007c0	/* pin 48 */
#define MIO_RX_PIN	0xf80007c4	/* pin 49 */
#endif

#define MIO_TX_PIN	48
#define MIO_RX_PIN	49

/* We get this from U-boot, so we dupicate that here
 * MIO pin 48; reg at f80007c0 = 000002e0
 * MIO pin 49; reg at f80007c4 = 000002e1
 * 0xe0 selects the uart1 function
 * 0x01 says "tri-enable" (makes sense for an input pin.
 * 0x200 is bit 9 in 11:9 for "io type" (LVCMOS18)
 *  -- it seems to me 010 for lvcmos25 would be more likely
 * the schematic shows a lsf0102 bidirectional level shifter
 * from 2.5 * to 3.3
 * The bit for pullup is 0, speed is 0.
 *
 * I have tried both the 1.8 and 2.5 volt setting, both work.
 * Based on the schematic, the 2.5 volt setting is corred.
 */

static void
uart_pin_init ( void )
{
		slcr_unlock ();
		/* Use 1.8 volt output driver */
		// mio_pin ( MIO_TX_PIN, 0x2e0 );
		// mio_pin ( MIO_RX_PIN, 0x2e1 );

		/* Use 2.5 volt output driver */
		mio_pin ( MIO_TX_PIN, 0x6e0 );
		mio_pin ( MIO_RX_PIN, 0x6e1 );
		slcr_lock ();
}

/*
0000a1f8:   e3a00d1f    mov r0, #0x7c0  ; ..
0000a1fc:   e582c000    str ip, [r2]    ; *0x704a0 = 0xe000_1000
0000a200:   e5835000    str r5, [r3]    ; *0x70498 = 0
0000a204:   e34f0800    movt    r0, #0xf800 ; f800_07c0
0000a208:   e3a010e0    mov r1, #0xe0
0000a20c:   ebffed64    bl  0x57a4      ; mio_pin ()
0000a210:   e30007c4    movw    r0, #0x7c4
0000a214:   e3a010e0    mov r1, #0xe0
0000a218:   e34f0800    movt    r0, #0xf800 ; f800_07c4
0000a21c:   ebffed60    bl  0x57a4      ; mio_pin()
*/

/* This yields 115200 baud for the bootrom "secret" code.
 * It also does that for me when loading this code via
 * the secret uart protocol.
 * XXX - will it still work when loaded from SD card?
 */
static void
baud_init ( void )
{
	struct zynq_uart *up = UART_BASE;

	up->baud_gen = 0x11;
	up->baud_div = 6;
}

void
uart_init ( void )
{
	struct zynq_uart *up = UART_BASE;

	uart_pin_init ();
	baud_init ();
	up->mode = 0x20;
	up->control = 0x114;
}

void
uart_putc ( char c )
{
	struct zynq_uart *up = UART_BASE;

	while ( up->cstatus & CS_TXFULL )
	    ;
	up->fifo = c;
}

void
puts ( char *s )
{
	while ( *s ) {
	    if (*s == '\n')
		uart_putc('\r');
	    uart_putc(*s++);
	}
}

void
main ( void )
{
	uart_init();

	for ( ;; ) {
	    puts ( "zzyzx " );
	}

	/* NOTREACHED */
}

/* THE END */
