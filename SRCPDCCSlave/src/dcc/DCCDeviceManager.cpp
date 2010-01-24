/*
 * DCCDeviceManager.cpp
 *
 *  Created on: 24.01.2010
 *      Author: mbern
 */

#include "DCCDeviceManager.h"
#include "GLOpenDCC.h"

namespace dcc
{
srcp::SRCPGenericLoco* DCCDeviceManager::createGL( srcp::device_config_t config, srcp::SRCPGenericLoco* next )
{
	switch ( config.subDevice )
	{
		case srcp::GLArduinoMotor:
			return	( new GLOpenDCC( config.start_addr, config.end_addr, next ) );

		default:
			break;
	}
	return	( 0 );

}


}
