/*
	I2CDeviceManager - Hilfsklasse erstellt die Klassen welche
	Befehle an/von Geraete via dem I2C an weitere Boards weitergeben

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

#include "I2CDeviceManager.h"

#include "I2CGAMaster.h"
#include "I2CGLMaster.h"
#include "I2CFBMaster.h"
#include "I2CUtil.h"

#include "../srcp/SRCPCommand.h"
#include "../srcp/SRCPDeviceMaster.h"

namespace i2c
{

srcp::SRCPGenericAccessoire* I2CDeviceManager::createGA( srcp::device_config_t config, srcp::SRCPGenericAccessoire* next )
{
	switch ( config.subDevice )
	{
		case srcp::I2CGAMaster:
			return	( new i2c::I2CGAMaster( config.start_addr, config.end_addr, config.args[0], next ) );
		default:
			break;
	}
	return	( 0 );
}

srcp::SRCPGenericLoco* I2CDeviceManager::createGL( srcp::device_config_t config, srcp::SRCPGenericLoco* next )
{
	switch ( config.subDevice )
	{
		case srcp::I2CGLMaster:
			return	( new i2c::I2CGLMaster( config.start_addr, config.end_addr, config.args[0], next ) );
		default:
			break;
	}
	return	( 0 );

}

srcp::SRCPFeedback* I2CDeviceManager::createFB( srcp::device_config_t config, srcp::SRCPFeedback* next )
{
	switch ( config.subDevice )
	{
		case srcp::I2CFBMaster:
			return	( new i2c::I2CFBMaster( config.start_addr, config.end_addr, config.args[0], next ) );
		default:
			break;
	}
	return	( 0 );
}

int I2CDeviceManager::createProxy( srcp::device_config_t config, srcp::SRCPGenericAccessoire **ga, srcp::SRCPGenericLoco **gl, srcp::SRCPFeedback **fb )
{
	// alle nicht I2C Board Proxies ignorieren
	if	( config.subDevice != srcp::I2CDESCRIPTION )
		return( 0 );

	int stepFB = config.args[0];
	int stepGA = config.args[1];
	int nextFB = 1;
	int nextGA = 1;
	union
	{
		uint8_t byte[12];
		int values[6];
	} buf;

#if	( DEBUG_SCOPE > 0 )
	Serial << "search I2C bus" << endl;
#endif

	for	( int i = 1; i < 128; i++ )
	{
		int board = I2CUtil::getSM( i, 0, 0, srcp::CV, CV_BOARD );
		// kein I2C Board auf dieser Adresse vorhanden?
		if	( board == -1 )
		{
			// Platzhalter 8 Adressen freihalten
			nextFB += stepFB;
			nextGA += stepGA;
			continue;
		}
		int rc = I2CUtil::getDescription( i, 0, 0, srcp::LAN, buf.byte );
		if	( rc == -1 )
			continue;
#if	( DEBUG_SCOPE > 0 )
			Serial << "I2C addr:id: " << i << ":" << board << ", fb: " << nextFB << " " << buf.values[0] << "-" << buf.values[1] <<
					", ga: " << nextGA << " " << buf.values[2] << "-" << buf.values[3] <<
					", gl: " << buf.values[4] << "-" << buf.values[5] << endl;
#endif
		switch	( board )
		{
			// DCC Boards haben fixe Adressen
			case srcp::BOARD_DCC:
				// GA Geraete vorhanden
				if	( buf.values[2] > 0 && buf.values[3] > 0 )
				{
					config.subDevice = srcp::I2CGAMaster;
					config.start_addr = buf.values[2];
					config.end_addr = buf.values[3];
					config.args[0] = i;
					*ga = createGA( config, *ga );
				}
				// GL Geraete vorhanden
				if	( buf.values[4] > 0 && buf.values[5] > 0 )
				{
					config.subDevice = srcp::I2CGLMaster;
					config.start_addr = buf.values[4];
					config.end_addr = buf.values[5];
					config.args[0] = i;
					*gl = createGL( config, *gl );
				}
				break;

			default:
				// FB Geraete vorhanden
				if	( buf.values[0] > 0 && buf.values[1] > 0 )
				{
					config.subDevice = srcp::I2CFBMaster;
					config.start_addr = nextFB;
					config.end_addr = nextFB + buf.values[1] - buf.values[0];
					config.args[0] = i;
					*fb = createFB( config, *fb );
					nextFB = config.end_addr + 1;
				}
				// GA Geraete vorhanden
				if	( buf.values[2] > 0 && buf.values[3] > 0 )
				{
					config.subDevice = srcp::I2CGAMaster;
					config.start_addr = nextGA;
					config.end_addr = nextGA +  + buf.values[3] - buf.values[2];
					config.args[0] = i;
					*ga = createGA( config, *ga );
					nextGA = config.end_addr + 1;
				}
				// GL (analoge) Geraete vorhanden
				if	( buf.values[4] > 0 && buf.values[5] > 0 )
				{
					config.subDevice = srcp::I2CGLMaster;
					config.start_addr = buf.values[4];
					config.end_addr = buf.values[5];
					config.args[0] = i;
					*gl = createGL( config, *gl );
				}
				break;
		}
	}

	return	( 1 );
}

}
