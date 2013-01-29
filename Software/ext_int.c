#include "dcc_rec.h"
#include "ext_int.h"
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void ext_int_init()
{
	// interrupt on INT1 pin rising edge (sensor triggered) 
	MCUCR = _BV(ISC11) | _BV(ISC10);

	// turn on interrupts!
	GIMSK  |= _BV(INT1);
}

/* External Interrupt Request 1 */
ISR (INT1_vect) {		
	if (bit_is_set(PORT_DCC_REC, PIN_DCC_REC)) {
		_inline_timer_start();
	}
}
