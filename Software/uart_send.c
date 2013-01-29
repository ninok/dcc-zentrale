#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main (void)
{
	uint8_t next_adress=0;
	uint8_t next_command=128;	
	uint8_t i;
	uart_init ();	
	sei ();

	for (i=0; i < 10; i++) {
		_delay_loop_2 (0);
	}
	for (;;) {
		next_adress++;
		next_command++;		
		uart_putc (next_adress);
		uart_putc (next_command);
		for (i=0; i < 6; i++) {
			_delay_loop_2 (0);
		}
	}//for (;;)
}


