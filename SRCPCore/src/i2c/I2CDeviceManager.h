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

#ifndef I2CDEVICEMANAGER_H_
#define I2CDEVICEMANAGER_H_

#include "../srcp/SRCPDeviceManager.h"

namespace i2c
{

class I2CDeviceManager : public srcp::SRCPDeviceManager
{
public:
	I2CDeviceManager() { next = 0; }
	srcp::SRCPGenericAccessoire* createGA( srcp::device_config_t config, srcp::SRCPGenericAccessoire* next );
	srcp::SRCPGenericLoco* createGL( srcp::device_config_t config, srcp::SRCPGenericLoco* next );
	srcp::SRCPFeedback* createFB( srcp::device_config_t config, srcp::SRCPFeedback* next );
};

}

#endif /* I2CDEVICEMANAGER_H_ */
