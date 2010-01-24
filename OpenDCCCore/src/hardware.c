/*
 * hardware.c
 *
 *  Created on: 20.01.2010
 *      Author: mbern
 */

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>         // put var to program memory
#include <avr/eeprom.h>
#include <string.h>

#include "config.h"                // general structures and definitions
#include "hardware.h"
#include "parser.h"
#include "dccout.h"

#include "organizer.h"

void init_main( void )
{
#if (__AVR_ATmega32__)
	// Input/Output Ports initialization
	// Port D
	PORTD = (0 << NDCC) // out
			| (1 << DCC) // out
			| (0 << ENA_PROG) // out, disabled
			| (0 << ENA_MAIN); // out, disabled

	DDRD = (1 << NDCC) // out
			| (1 << DCC) // out
			| (1 << ENA_PROG) // out
			| (1 << ENA_MAIN); // out
#elif (__AVR_ATmega328P__)
	PORTB = (0<<NDCC) // out
	| (1<<DCC) // out
	| (0<<ENA_PROG) // out, disabled
	| (0<<ENA_MAIN); // out, disabled

	DDRB = (1<<NDCC) // out
	| (1<<DCC) // out
	| (1<<ENA_PROG) // out
	| (1<<ENA_MAIN); // out

#endif

}

