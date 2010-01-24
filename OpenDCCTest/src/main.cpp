/*
	main - kleines Testprogrammm um die OpenDCC Funktionen zu
	testen.

	Copyright (c) 2010 Marcel Bernet.  All right reserved.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
#include "WProgram.h"

t_opendcc_state opendcc_state; // this is the current running state

extern "C" void Serial_println( char* p )
{
	Serial.println( p );
}
extern "C" void Serial_print( char* p )
{
	Serial.print( p );
}

void setup()
{
	Serial.begin( 19200 );
	Serial.println( "setup" );
}

void run( int* loks, int* speed, int size, int stop, int dir )
{
	int rc;

	for ( int i = 0; i < size; i++ )
		if ( organizer_ready() )
		{
			if ( stop )
			{
				do_loco_func_grp0( loks[i], 255 );
				do_loco_func_grp1( loks[i], 255 );
				if	( dir )
					rc = do_loco_speed( loks[i], 0x80 | 0 );
				else
					rc = do_loco_speed( loks[i], 0 );
				Serial << "do " << loks[i] << ", speed " << 0 << ", dir " << stop << ",rc " << rc << endl;
			}
			else
			{
				do_loco_func_grp0( loks[i], 255 );
				do_loco_func_grp1( loks[i], 255 );
				if	( dir )
					rc = do_loco_speed( loks[i], 0x80 | speed[i] );
				else
					rc = do_loco_speed( loks[i], speed[i] );
				Serial << "do " << loks[i] << ", speed " << speed[i] << ", dir " << stop << ",rc " << rc << endl;
			}
		}
		else
			Serial << "queue full!!!!!!!!!!!!!!!!!!" << endl;

}

void setup3( int* servos, int size, int dir )
{
	for ( int i = 0; i < size; i++ )
	{
		if ( organizer_ready() )
		{
			if ( dir )
			{
				do_accessory( servos[i], 0, 0 );
				do_accessory( servos[i], 1, 0 );
				do_accessory( servos[i], 0, 1 );
			}
			else
			{
				do_accessory( servos[i], 0, 0 );
				do_accessory( servos[i], 1, 0 );
				do_accessory( servos[i], 1, 1 );
			}
			Serial << "servo " << servos[i] << " " << dir << endl;
		}
		else
			Serial << "queue full!!!!!!!!!!!!!!!!!!" << endl;
	}
}

void loopDCC()
{
	for ( int c = 0; c < 50; c++ )
		{
			run_organizer(); // run command organizer, depending on state,
			delay( 20 );
		}
}

void loop()
{
	init_main();
	init_dccout(); // timing engine for dcc

	init_organizer(); // engine for command repetition, memory of loco speeds and types
	opendcc_state = RUN_OKAY;

	Serial << "run" << endl;
	int loks[] =  { 162, 35, 43 };
	int speed[] = { 80, 60, 60 };
	int loks2[] =  { 630, 35, 43 };
	int speed2[] = { 70, 60, 60 };

	int servos1[] = { 80, 81 };
	int servos2[] = { 82, 83 };


	while ( 1 )
	{
		setup3( servos1, 2, 0 );
		Serial << "weichen start" << endl;
		loopDCC();

		Serial << "loks start" << endl;
		run( loks, speed, 3, 0, 0 );
		loopDCC();

		Serial << "weichen stop" << endl;
		setup3( servos1, 2, 1 );
		loopDCC();

		Serial << "loks stop" << endl;
		run( loks, speed, 3, 1, 0 );
		loopDCC();

		setup3( servos1, 2, 0 );
		Serial << "weichen start" << endl;
		loopDCC();

		Serial << "loks start r" << endl;
		run( loks, speed, 3, 0, 1 );
		loopDCC();

		Serial << "weichen stop" << endl;
		setup3( servos1, 2, 1 );
		loopDCC();

		Serial << "loks stop r" << endl;
		run( loks, speed, 3, 1, 1 );
		loopDCC();

		///////////////////////////////

		setup3( servos2, 2, 0 );
		Serial << "weichen start" << endl;
		loopDCC();

		Serial << "loks start" << endl;
		run( loks2, speed2, 3, 0, 0 );
		loopDCC();


		Serial << "weichen stop" << endl;
		setup3( servos2, 2, 1 );
		loopDCC();

		Serial << "loks stop" << endl;
		run( loks2, speed2, 3, 1, 0 );
		loopDCC();

		setup3( servos2, 2, 0 );
		Serial << "weichen start" << endl;
		loopDCC();

		Serial << "loks start r" << endl;
		run( loks2, speed2, 3, 0, 1 );
		loopDCC();

		Serial << "weichen stop" << endl;
		setup3( servos2, 2, 1 );
		loopDCC();

		Serial << "loks stop r" << endl;
		run( loks2, speed2, 3, 1, 1 );
		loopDCC();
	}
}

int main( void )
{
	init();

	setup();

	for ( ;; )
		loop();

	return 0;
}

extern "C" void __cxa_pure_virtual()
{
	cli();
	for ( ;; )
		;
}
