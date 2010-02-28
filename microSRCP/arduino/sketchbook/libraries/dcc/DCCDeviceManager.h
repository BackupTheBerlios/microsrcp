/*
	DCCDeviceManager - Hilfsklasse zur Erzeugung von DCC Geraeten

	Copyright (c) 2010 Marcel Bernet.  All right reserved.

	Der Code wurde ermoeglicht durch das OpenDCC Projekt und
	die Zurverfuegungstellung der Sourcen von Wolfgang Kufer.
	Besten Dank!

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

#ifndef DCCDEVICEMANAGER_H_
#define DCCDEVICEMANAGER_H_

#include <../srcp/SRCPCommand.h>
#include <../srcp/SRCPDeviceManager.h>
#include <../srcp/SRCPGenericLoco.h>
#include <../srcp/SRCPGenericAccessoire.h>

namespace dcc
{

class DCCDeviceManager : public srcp::SRCPDeviceManager
{
public:
	DCCDeviceManager() { next = 0; }
	srcp::SRCPGenericAccessoire* createGA( srcp::device_config_t config, srcp::SRCPGenericAccessoire* next );
	srcp::SRCPGenericLoco* createGL( srcp::device_config_t config, srcp::SRCPGenericLoco* next );

};

}

#endif /* DCCDEVICEMANAGER_H_ */
