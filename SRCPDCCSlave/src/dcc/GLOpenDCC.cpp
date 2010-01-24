/*
 * GLOpenDCC.cpp
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
#include "GLOpenDCC.h"
#include <WProgram.h>

namespace dcc
{

GLOpenDCC::GLOpenDCC( int startAddr, uint8_t endAddr, srcp::SRCPGenericLoco* next )
{
	this->startAddr = startAddr;
	this->endAddr = endAddr;
	this->next = next;
}

int GLOpenDCC::set( int addr, int drivemode, int v, int v_max, int fn[] )
{
	v = map( v, 0, v_max, 0, 127 );

	if	( drivemode )
		v |= 0x80;

	// FIXME fn abhandeln und map ueberpruefen
	do_loco_func_grp0( addr, 255 );	// Licht
	do_loco_func_grp1( addr, 255 ); // F1 - F4
	do_loco_speed( addr, v );

	return	( 200 );
}

}
