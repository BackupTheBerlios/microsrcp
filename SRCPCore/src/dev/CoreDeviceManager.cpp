/*
	CoreDeviceManager - Hilfsklasse um lokale Geraete wie Servos,
	Lichtsignale, Motorentreiber zu erstellen.
	Verwaltet werden diese vom SRCPDeviceMaster.

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

#include <WProgram.h>
#include "CoreDeviceManager.h"

#include "GASignal.h"
#include "GAServo.h"

//#include "GLAnalog293.h"
//#include "GLAFMotor.h"
#include "GLArduinoMotor.h"

#include "FBSwitchSensor.h"

namespace dev
{

srcp::SRCPGenericAccessoire* CoreDeviceManager::createGA( srcp::device_config_t config, srcp::SRCPGenericAccessoire* next )
{
	switch ( config.subDevice )
	{
		case srcp::GASignal:
			return	( new dev::GASignal( config.start_addr, config.args[0], config.args[1], next ) );
		case srcp::GAServo:
			return	( new dev::GAServo( config.start_addr, config.args[0], config.args[1], config.args[2], next ));

		default:
			break;
	}
	return	( 0 );
}

srcp::SRCPGenericLoco* CoreDeviceManager::createGL( srcp::device_config_t config, srcp::SRCPGenericLoco* next )
{
	switch ( config.subDevice )
	{
/*		case srcp::GLAnalog293:
			return	( new dev::GLAnalog293( config.start_addr, config.args[0], config.args[1], config.args[2], next ) );
		case srcp::GLAFMotor:
			return	( new dev::GLAFMotor( config.start_addr, config.args[0], next ));*/
		case srcp::GLArduinoMotor:
			return	( new dev::GLArduinoMotor( config.start_addr, config.args[0], config.args[1], next ) );

		default:
			break;
	}
	return	( 0 );

}

srcp::SRCPPower* CoreDeviceManager::createPower( srcp::device_config_t config, srcp::SRCPPower* next )
{
	return	( 0 );

}

srcp::SRCPFeedback* CoreDeviceManager::createFB( srcp::device_config_t config, srcp::SRCPFeedback* next )
{
	switch ( config.subDevice )
	{
		case srcp::FBSwitchSensor:
			return	( new dev::FBSwitchSensor( config.start_addr, config.args[0], config.args[1], next ) );

		default:
			break;
	}
	return	( 0 );

}

}

dev::CoreDeviceManager CoreDevices = dev::CoreDeviceManager();
