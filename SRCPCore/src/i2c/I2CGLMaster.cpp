/*
	I2CGLMaster - Hilfsklasse wandelt SRCP Befehle um nach I2C.
	Dient zur Ansteuerung von Motorentreiber.

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

#include "I2CGLMaster.h"
#include "I2CUtil.h"
#include "../srcp/SRCPCommand.h"

namespace i2c
{

I2CGLMaster::I2CGLMaster( int startAddr, int endAddr, int remoteAddr, srcp::SRCPGenericLoco* next )
{
	this->startAddr = startAddr;
	this->endAddr = endAddr;
	this->addr = remoteAddr;
	this->next = next;
}

int I2CGLMaster::set( int addr, int drivemode, int v, int v_max, int fn[] )
{
	uint8_t buf[6];

	buf[0] = srcp::GL;
	buf[1] = srcp::SET;
	buf[2] = addr;
	buf[3] = drivemode;
	buf[4] = v;
	buf[5] = v_max;

	// TODO fn abhandeln

	return	( I2CUtil::write( this->addr, buf, sizeof(buf) ) );
}
}
