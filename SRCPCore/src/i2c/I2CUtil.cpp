/*
	I2CUtil - Hilfsklasse fuer den I2C Bus.
	Stellt LowLevel Funktionen wie write/read zur Verfuegung.
	Auf die Arduino Wire Library kann dadurch komplett verzichtet
	werden (spart Speicherplatz) und die Kommunikation ist
	sicherer durch Timeout und wiederholtes Senden bei Fehler.

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

extern "C" {
#include "types.h"
#include "twi.h"
}

#include <wiring.h>
#include "I2CUtil.h"

namespace i2c
{

void I2CUtil::begin( int addr = 0 )
{
	twi_init();
	if ( addr != 0 )
		twi_setAddress( addr );
}

int I2CUtil::write( int addr, uint8_t *buf, int size )
{
	// mehrmals versuchen ob Kommunikation klappt - evtl. muss auf den Bus gewartet werden.
	for ( int i = 0; i < 3; i++ )
	{
		int rc = twi_writeTo( addr, buf, size, 1 );
		if ( rc == 0 )
			return (200);

		delay( 5 );
	}
	// timeout!
	return (417);
}

int I2CUtil::read( int addr, uint8_t *buf, int size )
{
	// mehrmals versuchen ob Kommunikation klappt - evtl. muss auf den Bus gewartet werden.
	for ( int i = 0; i < 5; i++ )
	{
		int rc = twi_readFrom( addr, buf, size );
		if ( rc == size )
			return (200);

		delay( 10 );
	}
	// timeout!
	return (417);
}

}
