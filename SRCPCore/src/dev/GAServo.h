/*
	GAServo - Einfache Servo Ansteuerung mittels der Arduino
	Servo Library.

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

#ifndef GASERVO_H_
#define GASERVO_H_

#include "../srcp/SRCPGenericAccessoire.h"
#include <Servo.h>

namespace dev
{

class GAServo: public srcp::SRCPGenericAccessoire
{
private:
	Servo *servo;
	uint8_t min;
	uint8_t max;
public:
	GAServo( int addr, uint8_t pin, uint8_t min, uint8_t max, srcp::SRCPGenericAccessoire* next );
	int get( int addr, int port ) { return ( 200 ); }
	int set( int addr, int port, int value, int delay );
};

}

#endif /* GASERVO_H_ */
