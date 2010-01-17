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

#include "FBAnalogSensor.h"

namespace dev
{

FBAnalogSensor::FBAnalogSensor(uint8_t pin, uint8_t pos)
{
	this->pin = pin;
	this->pos = pos;

	pinMode(pin, INPUT);

	oldSensor = 0;
	sensor = 0;
}

void FBAnalogSensor::refresh()
{
	//Abfrage des gedrueckten Sensors - um Störungen zu vermeiden wurden
	//die Bereiche sehr eng gefasst, sollten bei Bedarf an jedes Board extra angepasst werden.

	int val = analogRead( pin );

	if ((val >= 337) && (val <= 343))
		bitSet( sensor, 0 );
	else if ((val >= 268) && (val <= 274))
		bitSet( sensor, 1 );
	else if ((val >= 200) && (val <= 206))
		bitSet( sensor, 2 );
	else if ((val >= 132) && (val <= 138))
		bitSet( sensor, 3 );
	else if ((val >= 64) && (val <= 70))
		bitSet( sensor, 4 );

}
srcp::feedback* FBAnalogSensor::info(int addr)
{
	refresh();

	memset( srcp::fb, 0, sizeof(srcp::fb));

	int pos = 0;
	for	( int i = 0; i < 8; i++ )
		if	( bitRead(oldSensor, i) != bitRead(sensor,i) )
		{
			srcp::fb[pos].pin = i + this->addr;
			srcp::fb[pos].value = bitRead(sensor, i);
			pos++;
		}

	oldSensor = sensor;
	sensor = 0;
	return	( srcp::fb );
}
}
