/*
	SRCPDeviceManager - Interface wo alle Funktionen definiert die
	ein DeviceManager implementieren muss.

	Siehe auch SRCPDeviceMaster, http://srcpd.sourceforge.net/srcp/

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

#ifndef SRCPDEVICEMANAGER_H_
#define SRCPDEVICEMANAGER_H_

#include "SRCPCommand.h"
#include "SRCPFeedback.h"
#include "SRCPGenericAccessoire.h"
#include "SRCPGenericLoco.h"

namespace srcp
{

class SRCPDeviceManager
{
protected:
	SRCPDeviceManager *next;
public:
	virtual SRCPGenericAccessoire* createGA( device_config_t config, SRCPGenericAccessoire* next ) { return(0); }
	virtual SRCPGenericLoco* createGL( device_config_t config, SRCPGenericLoco* next ) { return(0); }
	virtual SRCPFeedback* createFB( device_config_t config, SRCPFeedback* next ) { return(0); }
	virtual int createProxy( device_config_t config, SRCPGenericAccessoire **ga, SRCPGenericLoco **gl, SRCPFeedback **fb ) { return(0); }
	void setNextManager( SRCPDeviceManager *next ) { this->next = next; }
	SRCPDeviceManager* getNextManager() { return ( next ); }
};

}

#endif /* SRCPDEVICEMANAGER_H_ */
