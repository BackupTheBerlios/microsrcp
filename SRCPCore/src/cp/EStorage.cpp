/*
	EStorage - Hilfsklasse um die Board/Geraete Konfiguration
	im EEPROM zu lesen und schreiben.

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

#include "EStorage.h"
#include <WProgram.h>

// Preinstantiate Objects //////////////////////////////////////////////////////

cp::EStorage Storage = cp::EStorage();

namespace cp
{

void EStorage::init(srcp::device_config_t deviceConfig[], int id, int version)
{
	// Board oder Version-Nr. gewechselt = Storage frisch initialisieren
	if	( read(CV_BOARD) != id || read(CV_VERSION) != version || read(CV_PRODUCER) == CV_PRODUCER )
	{
		int i = 0;
		for ( ; deviceConfig[i].start_addr != -1; i++ )
			writeConfig( i+1, deviceConfig[i] );

		// EOF explizit schreiben!
		int cv = (i + 1) * (sizeof(srcp::device_config_t) + 2);
		for	( unsigned int ii = 0; ii < (sizeof(srcp::device_config_t) + 2); ii++ )
			write( cv+ii, 255 );

		// am Schluss allgemeine Werte zuruecksetzen um Fehler durch Reset zu verhindern
		write( CV_ID, id );
		write( CV_BOARD, id );
		write( CV_VERSION, version );
		write( CV_PRODUCER, PRODUCER_ID );
	}
}

srcp::device_config_t EStorage::getConfig(int addr)
{
	srcp::device_config_t device;

	// Startadresse Parameter
	int cv = addr * (sizeof(srcp::device_config_t) + 2);

	device.device     = (srcp::devices) read( cv ); 		cv++;
	device.subDevice  = read( cv ); 						cv++;
	device.start_addr = readInt( cv ); 						cv += 2;
	device.end_addr   = readInt( cv );						cv += 2;

	for	( unsigned int i = 0; i < sizeof(device.args); i++ )
		device.args[i] = read( cv + i );

	return	( device );
}

void EStorage::writeConfig(int addr, srcp::device_config_t device )
{
	// Startadresse Parameter
	int cv = addr * (sizeof(srcp::device_config_t) + 2);

	write( cv, device.device );			cv++;
	write( cv, device.subDevice );		cv++;
	writeInt( cv, device.start_addr );	cv += 2;
	writeInt( cv, device.end_addr );	cv += 2;

	for	( unsigned int i = 0; i < sizeof(device.args); i++ )
		write( cv + i, device.args[i] );
}

}




