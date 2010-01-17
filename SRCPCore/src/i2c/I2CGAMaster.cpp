/*
	I2CGAMaster - Hilfsklasse wandelt SRCP Befehle um nach I2C.
	Dient zum Schalten von Geraeten wie Servos, Lichtsignale ...

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

#include "I2CGAMaster.h"
#include "I2CUtil.h"
#include "../srcp/SRCPCommand.h"

namespace i2c
{

I2CGAMaster::I2CGAMaster( int startAddr, int endAddr, int remoteAddr, srcp::SRCPGenericAccessoire* next )
{
	this->startAddr = startAddr;
	this->endAddr = endAddr;
	this->addr = remoteAddr;
	this->next = next;
}

int I2CGAMaster::set( int addr, int port, int value, int delay )
{
	uint8_t buf[6];
	buf[0] = srcp::GA;
	buf[1] = srcp::SET;
	buf[2] = addr - startAddr;
	buf[3] = port;
	buf[4] = value;
	buf[5] = delay;

	return	( I2CUtil::write( this->addr, buf, sizeof(buf) ) );
}

}
