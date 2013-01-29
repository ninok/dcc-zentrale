#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

/* Test von F_CPU und INTERRUPTS_PER_SECOND */
/* auf Gültigkeitsbereich                   */
/*
#if (F_CPU / INTERRUPTS_PER_SECOND -1 < 0) \
	|| (F_CPU / INTERRUPTS_PER_SECOND -1 >= 0x10000)
#error Werte für F_CPU bzw. INTERRUPTS_PER_SECOND ungeeignet
#error evtl. muss der Prescaler verwendet werden
#endif
*/

#ifdef __AVR_AT90S2313__
void timer_init (uint8_t outputCompareValue)
{
	//Timer1 Standard Wavegenearationmode
	TCCR1A = 0;

	//Timer1 mit 1/8 Prescale
	TCCR1B = _BV (CS11);

	//Output Compare Register setzen
	OCR1AH = 0;
	OCR1AL = outputCompareValue;

	//Timer Counter zurücksetzen
	TCNT1H = 0;	
	TCNT1L = 0;
}


/* Timer/Counter1 Compare Match */
SIGNAL(TIMER1_COMP1_vect)
{
	_inline_timer_job();
	_inline_timer_stop();
}

#elif __AVR_ATtiny2313__

void timer_init (uint8_t outputCompareValue)
{
	//Timer0 Standard Wavegenearationmode
	TCCR0A |= _BV(WGM01);

	//Timer0 ohne Prescale
	TCCR0B = _BV (CS11);

	//Output Compare Register setzen
	OCR0A = (uint8_t) outputCompareValue;

	TIMSK = _BV (OCIE0A);

	TCNT0 = 0;
}

SIGNAL(TIMER0_COMPA_vect)
{
	_inline_timer_job();
	_inline_timer_stop();
}

#endif
