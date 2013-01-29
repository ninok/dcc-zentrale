#ifndef _TIMER_H_
#define _TIMER_H_

#include <avr/io.h>
#include "dcc_rec.h"

extern void timer_init(uint8_t outputCompareValue);

static inline void _inline_timer_job () {	
	if (PORT_DCC_REC & _BV(PIN_DCC_REC)) {//Null empfangen
		nextBit = 0;
	} else {//Eins empfangen
		nextBit = 1;	
	}	
}

static inline void _inline_timer_start () {
	#ifdef __AVR_AT90S2313__
	//Timer Counter zurücksetzen
	TCNT1H = 0;	
	TCNT1L = 0;
	//Timer OutputCompareInt aktivieren
	TIMSK |= _BV (OCIE1A);
	#elif __AVR_ATtiny2313__
	//Timer Counter zurücksetzen
	TCNT0 = 0;
	//Timer OutputCompareInt aktivieren
	TIMSK |= _BV (OCIE0A);
	#endif
}

static inline void _inline_timer_stop () {
	#ifdef __AVR_AT90S2313__
	TIMSK &= ~_BV (OCIE1A);
	#elif __AVR_ATtiny2313__
	TIMSK &= ~_BV (OCIE0A);
	#endif
}

#endif /* _TIMER_H_ */


