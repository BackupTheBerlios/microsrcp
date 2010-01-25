/*
 * GAOpenDCC.cpp
 *
 *  Created on: 24.01.2010
 *      Author: mbern
 */

extern "C"
{
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>

#include "config.h"                // general structures and definitions - make your changes here
#include "parser.h"
#include "dccout.h"                // make dcc
#include "organizer.h"             // manage commands
}
#include "GAOpenDCC.h"
#include <WProgram.h>

namespace dcc
{

GAOpenDCC::GAOpenDCC( int startAddr, int endAddr, srcp::SRCPGenericAccessoire* next)
{
	this->startAddr = startAddr;
	this->endAddr = endAddr;
	this->next = next;
}

int GAOpenDCC::set( int addr, int port, int value, int delay )
{
	// TODO wie ist es mit an und Abschalten des Powers?
	do_accessory( addr, 0, 0 );
	do_accessory( addr, 1, 0 );
	do_accessory( addr, port, 1 );

	return	( 200 );
}

}
