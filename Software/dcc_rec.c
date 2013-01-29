#include "ext_int.h"
#include "dcc_rec.h"
#include "dcc_rec.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//register uint8_t nextBit ("r30");

#define ERROR(num)\
	PORTB = num;\
	nextBit = 2;\
	bitOffset = 0x00;\
	for(;;);

int main (void)
{	
	volatile uint8_t bitOffset;
	volatile uint8_t address;
	volatile uint8_t command;
	volatile uint8_t checksum;
	volatile uint8_t praeambel;

	cli();

	DDR_DCC_REC &= ~_BV(PIN_DCC_REC);
	PORT_DCC_REC &= ~_BV(PIN_DCC_REC);

	//DDRD |= _BV(0);
	//PORTD &= ~_BV(0);

	DDRB = 0xff;
	//PORTB = 0xff;

	//Der Timer läuft mit 8-tel Tackt, also entspricht folgende Formel 88 us warten.
	timer_init( (uint8_t)( (uint32_t)(11*F_CPU)/(uint32_t)(1000000) ) );
	ext_int_init ();	

	bitOffset = 0;
	nextBit = 2;
	
	sei();
	

	for (;;) {
		PORTB = bitOffset;
		if ( nextBit == 0) {				
			if ( bitOffset == 0 ) {
				if (praeambel < 10) {
					ERROR(0xE1);
				}
				bitOffset++;
				address = 0;
			} else if ( bitOffset == 9) {
				bitOffset ++;
				command = 0;
			} else if ( bitOffset == 18) {
				bitOffset ++;
				checksum = 0;
			} else if ( bitOffset == 27) {				
				ERROR(0xE2);
			} else if ( bitOffset < 9) {
				bitOffset ++;
				address = (address << 1) & 0xFE;
			} else if ( bitOffset < 18) {
				bitOffset ++;
				command = (command << 1) & 0xFE;				
			} else if ( bitOffset < 27) {
				bitOffset ++;
				checksum = (checksum << 1) & 0xFE;
			}
			nextBit = 2;
		}else if( nextBit == 1 ) {
			if ( bitOffset == 0) {
				//PRAEAMBEL
				praeambel++;				
			} else if ( bitOffset == 9) {
				ERROR(address);
			} else if ( bitOffset == 18) {
				// ERROR
				ERROR(0xE4);
			}else if ( bitOffset == 27) {
				bitOffset = 0;				
				PORTB = command;				
				//checksum ^= command;
				//if (checksum == address) {
				//}				
			} else if ( bitOffset < 9) {
				bitOffset ++;
				address = (address << 1) | 0x01;
			} else if ( bitOffset < 18) {
				bitOffset ++;
				command = (command << 1) | 0x01;
			} else if ( bitOffset < 27) {
				bitOffset ++;
				checksum = (checksum << 1) | 0x01;
			}
			nextBit = 2;
		}		
	}//for (;;)
}

