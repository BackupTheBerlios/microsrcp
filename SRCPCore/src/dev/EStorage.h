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

#ifndef ESTORAGE_H_
#define ESTORAGE_H_

#include <EEPROM.h>
#include "../srcp/SRCPCommand.h"

namespace dev
{

#define CV_ID 			1
#define CV_BOARD 		2
#define CV_VERSION 		7
#define CV_PRODUCER 	8
#define PRODUCER_ID		123

class EStorage
{
private:
	int readInt( int cv ) { return( EEPROM.read( cv ) * 256 + EEPROM.read( cv+1 ) ); }
	void writeInt( int cv, int value ) { EEPROM.write( cv, value/256); EEPROM.write( cv+1, value%256); }
	void writeConfig(int addr, srcp::device_config_t device );
public:
	uint8_t read( int cv ) { return( EEPROM.read( cv ) ); }
	void write( int cv, uint8_t value ) { EEPROM.write( cv, value ); }
	void init( srcp::device_config_t deviceConfig[], int id, int version );
	srcp::device_config_t getConfig( int addr );
};

}

extern dev::EStorage Storage;

#endif /* ESTORAGE_H_ */
