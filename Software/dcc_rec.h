#ifndef _DCC_REC_H_
#define _DCC_REC_H_

#include <avr/io.h>

#define DDR_DCC_REC DDRD
#define PORT_DCC_REC PIND
#define PIN_DCC_REC 3

volatile register uint8_t nextBit asm("r15");

#endif /* _DCC_REC_H_ */



