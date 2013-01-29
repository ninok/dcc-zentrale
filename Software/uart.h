#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>

#ifndef F_CPU
#warning F_CPU wurde nicht vom Compiler definiert
#define F_CPU 17200000 //Bei 4.0V haben wir einen Takt von 17,2 MHz
#endif /* F_CPU */

#define BAUDRATE 19200
#define BUFSIZE_IN  0x20
#define BUFSIZE_OUT 0x20

extern void uart_init();
extern int uart_putc (const uint8_t);
extern uint8_t uart_getc_wait();
extern int     uart_getc_nowait();

static inline void uart_flush()
{
	#ifdef __AVR_ATtiny2313__
	while (UCSRB & _BV(UDRIE));
	#elif __AVR_AT90S2313__
	while (UCR & _BV(UDRIE));
	#endif
}

#endif /* _UART_H_ */


