/*
	GASignal - Einfache 2 begriffige Lichtsignal Ansteuerung.

	Ich verwende Signale von BEMO mit 2 LED die haben eine
	gemeinsame Leitung nach 5+V und je eine Leitung nach GND.

	Dadurch leuchtet die LED wenn der Pin auf GND geschaltet wird.

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

#ifndef GASIGNAL_H_
#define GASIGNAL_H_

#include <inttypes.h>
#include <wiring.h>
#include "../srcp/SRCPGenericAccessoire.h"

namespace dev
{

class GASignal: public srcp::SRCPGenericAccessoire
{
private:
	uint8_t redPin;
	uint8_t greenPin;
public:
	GASignal( int addr, uint8_t red, uint8_t green, srcp::SRCPGenericAccessoire* next );
	int get( int addr, int port ) { return ( 200 ); }
	int set( int addr, int port, int value, int delay );
};

}

#endif /* GASIGNAL_H_ */
