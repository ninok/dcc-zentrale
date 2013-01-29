#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "dcc_send.h"
#include "uart.h"

uint16_t oneLength;
uint16_t zeroLength;

void send_one (void) {
	PORT_DCC_SEND |= _BV (PIN_DCC_SEND);
	_delay_loop_2 (oneLength);
	PORT_DCC_SEND &= ~_BV (PIN_DCC_SEND);	
	_delay_loop_2 (oneLength);	
}	

void send_zero (void) {
	PORT_DCC_SEND |= _BV (PIN_DCC_SEND);
	_delay_loop_2 (zeroLength);
	PORT_DCC_SEND &= ~_BV (PIN_DCC_SEND);	
	_delay_loop_2 (zeroLength);	
}	

void send_byte (uint8_t data) {
	uint8_t next_bit;
	uint8_t i;
	for (i=0; i<8; i++) {
		next_bit = (data >> (7-i)) & 0x01;
		if (next_bit)
		send_one();
		else	
		send_zero();
	}
}	

void send_praeambel (void) {
	uint8_t i;
	for (i=0; i<15; i++) {
		send_one();
	}
}

void send_command (uint8_t address, uint8_t command) {
	send_praeambel();
	send_zero();
	send_byte(address);
	send_zero();
	send_byte(command);
	send_zero();
	address ^= command;
	send_byte(address);
	send_one();
	//PORT_DCC_SEND &= ~_BV (PIN_DCC_SEND);
}

int main (void)
{
	uint8_t next_address = 0x12;
	uint8_t next_command = 0x34;	

	//Eine Phase der "Eins" dauert 58 us, aber wir warten
	//in send_one mit _delay_loop_2, welches 4 Takte pro
	//Zyklus wartet
	oneLength = (uint16_t)( (uint32_t)(14*F_CPU)/(uint32_t)(1000000) );
	zeroLength = (uint16_t)( (uint32_t)(29*F_CPU)/(uint32_t)(1000000) );

	DDR_DCC_SEND |= _BV (PIN_DCC_SEND);
	PORT_DCC_SEND &= ~_BV (PIN_DCC_SEND);

	//uart_init ();
	
	sei ();

	
	//DDR_DCC_SEND = 0xff;
	//PORT_DCC_SEND = 0xff;
	for (;;) {
		_delay_loop_2(0);
		//next_address = uart_getc_wait ();
		//next_command = uart_getc_wait ();
		send_command (next_address, next_command);
		//uart_putc (next_address);
		//PORT_DCC_SEND = next_address;
	}//for (;;)
}




