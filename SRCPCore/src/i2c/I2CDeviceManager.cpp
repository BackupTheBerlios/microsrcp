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

}
