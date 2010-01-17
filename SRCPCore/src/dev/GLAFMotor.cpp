/*
	GLAFMotor - Motortreiber basiert auf dem Shield von
	LadyAda. Welche Pins verwendet werden bitte entsprechende
	Dokumentation beachten. Die Ansteuerung der Motoren
	erfolgt, soweit moeglich mit 64 KHz.

	Link zum Shield:
	http://www.ladyada.net/make/mshield/index.html

	ACHTUNG: das die Arduino alle PWM Register neu initialisiert
	kann dieser Treiber nicht mit Servos zusammen verwendet werden.

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

#include "WProgram.h"
#include "GLAFMotor.h"

namespace dev
{

GLAFMotor::GLAFMotor( int addr, uint8_t no, srcp::SRCPGenericLoco* next )
{
	this->addr = addr;
	this->next = next;

	if	( no == 1 || no == 2 )
		motor = new AF_DCMotor( no, MOTOR12_64KHZ );
	else if ( no == 3 || no == 4 )
		motor = new AF_DCMotor( no, MOTOR34_64KHZ );

}

int GLAFMotor::set( int addr, int drivemode, int v, int v_max, int fn[] )
{
	// rueckwarts?
	if	( drivemode == 0 )
		motor->run( BACKWARD );
	// vorwarets
	else
		motor->run( FORWARD );

	if	( v > v_max )
		v = v_max;
	if	( v < 0 )
		v = 0;

	v = map(v, 0, v_max, 0, 255);
	motor->setSpeed( v );
	return	( 200 );

}

}
