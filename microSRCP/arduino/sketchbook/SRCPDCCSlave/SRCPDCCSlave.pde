/*
	SRCPDCCSlave - I2C Board welches DCC Signale erzeugt.

	Bestehend aus:
	- Arduino Board mit min. ATmega168p - http://www.arduino.cc
	- Motor Shield - im Moment das Pollin Schrittmotor Board

	Copyright (c) 2010 Marcel Bernet.  All right reserved.

	Der Code wurde ermoeglicht durch das OpenDCC Projekt und
	die Zurverfuegungstellung der Sourcen von Wolfgang Kufer.
	Besten Dank!

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

#include <EEPROM.h>
#include <EStorage.h>
#include "SRCPDeviceMaster.h"
#include "DCCDeviceManager.h"
#include "I2CServer.h"

#define VERSION 11	// Version 1.0


// Definition der lokalen Geraete
srcp::device_config_t deviceConfig[] =
	{
		// SRCP Lokomotive von 1 - 9999 via DCC Protokoll haendeln
		{ 1, 9999, srcp::GL, srcp::GLDCC },

		// SRCP Zubehoer 1 - 9999 via DCC Protokoll haendeln,
		// ACHTUNG: Versatzadresse vom Server beruecksichtigen!
		{ 1, 9999, srcp::GA, srcp::GADCC },

		// EOF Geraete - nicht vergessen!
		{ -1 },
	};

t_opendcc_state opendcc_state; // this is the current running state

void setup()
{
#if	( DEBUG_SCOPE > 0 )
	Serial.begin(19200);
#endif

	WireServer.addDeviceManager( new dcc::DCCDeviceManager() );
	WireServer.begin( deviceConfig, srcp::BOARD_DCC, VERSION );

	init_main();
	init_dccout(); // timing engine for dcc

	init_organizer(); // engine for command repetition, memory of loco speeds and types
	opendcc_state = RUN_OKAY;

#if	( DEBUG_SCOPE > 0 )
	Serial << "listen " <<  WireServer.getMyAddr() << endl;
#endif
}

void loop()
{
	WireServer.run();
	run_organizer(); // run command organizer, depending on state,
	delay( 10 );
}


