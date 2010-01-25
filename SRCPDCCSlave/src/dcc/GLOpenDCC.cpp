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

GLOpenDCC::GLOpenDCC( int startAddr, int endAddr, srcp::SRCPGenericLoco* next )
{
	this->startAddr = startAddr;
	this->endAddr = endAddr;
	this->next = next;
}

int GLOpenDCC::set( int addr, int drivemode, int v, int v_max, int fn[] )
{
	// es sind nur Fahrstufen 0 - 127 moeglich, groessere Geschwindkeiten werden halbiert.
	if	( v_max > 127 )
	{
		v /= 2;
		v_max /= 2;
	}
	v = map( v, 0, v_max, 0, 127 );

	if	( drivemode )
		v |= 0x80;

	//Serial << "setGL addr=" << addr << ", v=" << v << endl;

	int f = bitRead( fn[3], 0 );
	do_loco_func_grp0( addr, f );	// Licht
	f = 0;
	for	( int i = 1; i < 5; i++ )
		bitWrite( f, i-1, bitRead(fn[3], i));
	do_loco_func_grp1( addr, f ); // F1 - F4

	// TODO F5 - F12 abhandeln
	do_loco_speed( addr, v );

	return	( 200 );
}

}
