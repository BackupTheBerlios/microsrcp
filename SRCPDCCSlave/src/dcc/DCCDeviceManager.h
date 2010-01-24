/*
 * DCCDeviceManager.h
 *
 *  Created on: 24.01.2010
 *      Author: mbern
 */

#ifndef DCCDEVICEMANAGER_H_
#define DCCDEVICEMANAGER_H_

#include <srcp/SRCPCommand.h>
#include <srcp/SRCPDeviceManager.h>
#include <srcp/SRCPGenericLoco.h>

namespace dcc
{

class DCCDeviceManager : public srcp::SRCPDeviceManager
{
public:
	DCCDeviceManager() { next = 0; }
	srcp::SRCPGenericLoco* createGL( srcp::device_config_t config, srcp::SRCPGenericLoco* next );

};

}

#endif /* DCCDEVICEMANAGER_H_ */
