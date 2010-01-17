/*
	FBAnalogSensor - Beispiel wie durch eine Widerstandskette von
	2K Ohm an einem der Analogen Eingaenge, mehrere Sensoren
	angebracht werden koennen.

	Nicht weiterverfolgt, wegen:
	- Sensoren koennen sich ueberlappen

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

#ifndef FBANALOGSENSOR_H_
#define FBANALOGSENSOR_H_

#include <WProgram.h>
#include "../srcp/SRCPFeedback.h"

/**
 * AnalogSensor mit 5 Widerstaenden wie bei rn-control verwendet.
 *
 * Sehr unzuverlaessig, nur fuer Testboards geeignet
 */
namespace dev
{

class FBAnalogSensor : public srcp::SRCPFeedback
{
private:
	uint8_t pin;
	uint8_t pos;
	void refresh();
public:
	FBAnalogSensor( uint8_t pin, uint8_t pos );
	srcp::feedback* info( int addr );
};
}

#endif /* FBANALOGSENSOR_H_ */
