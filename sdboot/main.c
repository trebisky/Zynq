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
	vu32	control;
	vu32	mode;
	vu32	ie;
	vu32	id;

	vu32	im;
	vu32	istatus;
	vu32	baud;
	vu32	rto;

	vu32	rfifo;
	vu32	mcontrol;
	vu32	mstatus;
	vu32	cstatus;

	vu32	fifo;
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

/*
; Set MIO pins for uart
; MIO pins are in the SLCR
; see page 52 in the TRM for a cute table
; page 1570 covers the SLCR,
; f800_07c0 is pin 48
; f800_07c4 is pin 49
; See page 1679, 1680 for these specific pins
; as you might expect this sets pin 48 to TxD and 49 to RxD
; (also some values get set in RAM here)
*/

#define MIO_TX	0xf80007c0	/* pin 48 */
#define MIO_RX	0xf80007c4	/* pin 49 */

static void
mio_pin_init ( void )
{
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

static void
baud_init ( void )
{
}

void
uart_init ( void )
{
	mio_pin_init ();
	baud_init ();
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
