#ifndef _DCC_SEND_H_
#define _DCC_SEND_H_

#include <avr/io.h>

#define DDR_DCC_SEND DDRB
#define PORT_DCC_SEND PORTB
#define PIN_DCC_SEND 2

extern void send_one (void);
extern void send_zero (void);
extern void send_byte (uint8_t data);
extern void send_praeambel (void);
extern void send_command (uint8_t address, uint8_t command);

#endif /* _DCC_SEND_H_ */



