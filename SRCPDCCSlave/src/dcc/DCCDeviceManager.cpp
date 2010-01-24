/*
 * DCCDeviceManager.cpp
 *
 *  Created on: 24.01.2010
 *      Author: mbern
 */

#include "DCCDeviceManager.h"
#include "GLOpenDCC.h"
#include "GAOpenDCC.h"

namespace dcc
{

srcp::SRCPGenericAccessoire* DCCDeviceManager::createGA( srcp::device_config_t config, srcp::SRCPGenericAccessoire* next )
{
	switch ( config.subDevice )
	{
		case srcp::GADCC:
			return	( new GAOpenDCC( config.start_addr, config.end_addr, next ) );

		default:
			break;
	}
	return	( 0 );
}

srcp::SRCPGenericLoco* DCCDeviceManager::createGL( srcp::device_config_t config, srcp::SRCPGenericLoco* next )
{
	switch ( config.subDevice )
	{
		case srcp::GLDCC:
			return	( new GLOpenDCC( config.start_addr, config.end_addr, next ) );

		default:
			break;
	}
	return	( 0 );

}

}
