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

#ifndef GLAFMOTOR_H_
#define GLAFMOTOR_H_

#include "../srcp/SRCPGenericLoco.h"
#include <AFMotor.h>

namespace dev
{

class GLAFMotor : public srcp::SRCPGenericLoco
{
private:
	AF_DCMotor *motor;
public:
	GLAFMotor( int addr, uint8_t no, srcp::SRCPGenericLoco* next );
	int set( int addr, int drivemode, int v, int v_max, int fn[] );
	void setPower( int on );
};

}

#endif /* GLAFMOTOR_H_ */
