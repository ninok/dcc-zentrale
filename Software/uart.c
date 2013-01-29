#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "fifo.h"

// FIFO-Objekte und Puffer für die Ein- und Ausgabe
uint8_t inbuf[BUFSIZE_IN];
fifo_t infifo;

uint8_t outbuf[BUFSIZE_OUT];
fifo_t outfifo;

void uart_init()
{
	// Global Interrupt-Flag sichern
	uint8_t sreg = SREG;

	uint16_t ubrr = (uint16_t) ((uint32_t) F_CPU/(16*BAUDRATE) - 1);

	#ifdef __AVR_ATtiny2313__
	UBRRH = (uint8_t) (ubrr>>8);
	UBRRL = (uint8_t) (ubrr);
	#elif __AVR_AT90S2313__
	UBRR = (uint8_t) (ubrr);
	#endif

	// Interrupts kurz deaktivieren
	cli();

	// UART Receiver und Transmitter anschalten, Receive-Interrupt aktivieren
	// Data mode 8N1, synchron
	#ifdef __AVR_ATtiny2313__
	UCSRB = _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);
	UCSRC = _BV(UMSEL) | _BV(UCSZ1) |  _BV(UCSZ0);
	#elif __AVR_AT90S2313__
	UCR =  _BV(RXEN) |  _BV(TXEN) |  _BV(RXCIE);
	#endif


	// Flush Receive-Buffer (entfernen evtl. vorhandener ungültiger Werte)
	do
	{
		// UDR auslesen (Wert wird nicht verwendet)
		UDR;
	}
	#ifdef __AVR_ATtiny2313__
	while (UCSRA & _BV( RXC));
	#elif __AVR_AT90S2313__
	while ( USR & _BV(RXC));
	#endif

	// Rücksetzen von Receive und Transmit Complete-Flags
	#ifdef __AVR_ATtiny2313__
	UCSRA = _BV(RXC) | _BV(TXC);
	#elif __AVR_AT90S2313__
	USR = _BV( RXC) | _BV(TXC);
	#endif


	// Global Interrupt-Flag wieder herstellen
	SREG = sreg;

	// FIFOs für Ein- und Ausgabe initialisieren
	fifo_init (&infifo,   inbuf, BUFSIZE_IN);
	fifo_init (&outfifo, outbuf, BUFSIZE_OUT);
}

int uart_putc (const uint8_t c)
{
	int ret = fifo_put (&outfifo, c);
	
	#ifdef __AVR_ATtiny2313__
	UCSRB |= _BV(UDRIE);
	#elif __AVR_AT90S2313__
	UCR |= _BV(UDRIE);
	#endif
	
	return ret;
}

int uart_getc_nowait ()
{
	return fifo_get_nowait (&infifo);
}

uint8_t uart_getc_wait ()
{
	return fifo_get_wait (&infifo);
}


//Zeichen  fertig empfangen
SIGNAL (UART_RX_vect)
{
	_inline_fifo_put (&infifo, UDR);
}

//UART hat kein Zeichen zum senden
SIGNAL (UART_UDRE_vect)
{
	//Wenns was zu senden gibt, dann senden
	if (outfifo.count > 0) {
		UDR = _inline_fifo_get (&outfifo);
	} else {
		//Wenn nicht, empty-buffer-irq ausschalten 
		//(wird von uart_putc wieder angeschaltet)
		#ifdef __AVR_ATtiny2313__
		UCSRB &= ~_BV(UDRIE);
		#elif __AVR_AT90S2313__
		UCR &= ~_BV(UDRIE);     	
		#endif
	}
}
