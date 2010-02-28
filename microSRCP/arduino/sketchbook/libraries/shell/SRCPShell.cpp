/*
	Shell - einfache Shell um den SRCP Server mittels Serieller Schnittstelle
	zu bedienen.

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

#include "SRCPShell.h"

#include <../srcp/SRCPCommand.h>
#include <../srcp/SRCPFeedback.h>
#include <../srcp/SRCPGenericAccessoire.h>
#include <../srcp/SRCPGenericLoco.h>
#include <../cp/EStorage.h>
#include <../i2c/I2CUtil.h>

// Preinstantiate Objects //////////////////////////////////////////////////////
shell::SRCPShell Shell = shell::SRCPShell();

namespace shell
{

void SRCPShell::begin( srcp::SRCPDeviceMaster *devices, srcp::SRCPSession *session )
{
	this->devices = devices;
	this->session = session;

	pos = 0;
}

void SRCPShell::run()
{
	while ( Serial.available() )
	{
		int c = Serial.read();

		// \r, \n oder ; loest Abarbeitung Befehl aus
		if	( c == '\r' || c == '\n' || c == ';' )
		{
			cmd[pos] = '\0';
			Serial << endl;
			dispatch( cmd );
			pos = 0;
			break;
		}
		if	( c == '\b' || c == 127 )
		{
			if	( pos > 0 )
			{
				pos--;
				Serial << (char) c;
			}
			continue;
		}
		if	( pos < sizeof(cmd) )
		{
			Serial << (char) c;
			cmd[pos] = c;
			pos++;
		}
	}
}

void SRCPShell::dispatch( char *cmd )
{
#if ( DEBUG_SCOPE == 0 )
	// Geraete am I2C Bus ausgeben
	if	( strncasecmp(cmd, "I2C", 3) == 0 || strncasecmp(cmd, "TWI", 3) == 0 )
	{
		union
		{
			uint8_t byte[12];
			int values[6];
		} buf;

		for	( int i = 1; i < 128; i++ )
		{
			int board = i2c::I2CUtil::getSM( i, 0, 0, srcp::CV, CV_BOARD );
			// kein I2C Board auf dieser Adresse vorhanden?
			if	( board == -1 )
				continue;
			int rc = i2c::I2CUtil::getDescription( i, 0, 0, srcp::LAN, buf.byte );
			if	( rc == -1 )
				continue;
				Serial << "I2C addr:id: " << i << ":" << board << ", fb: " << buf.values[0] << "-" << buf.values[1] <<
						", ga: " << buf.values[2] << "-" << buf.values[3] <<
						", gl: " << buf.values[4] << "-" << buf.values[5] << endl;
		}
	}
	// Devices ausgeben
	else if ( strncasecmp(cmd, "DEV", 3) == 0 )
	{
		for	( srcp::SRCPFeedback* n = devices->firstFeedbackElement(); n != 0; n = n->nextElement() )
			Serial << "FB " << n->getAddr() << " " << n->getStartAddr() << "-" << n->getEndAddr() << endl;
		for	( srcp::SRCPGenericAccessoire* n = devices->firstGAElement(); n != 0; n = n->nextElement() )
			Serial << "GA " << n->getAddr() << " " << n->getStartAddr() << "-" << n->getEndAddr() << endl;
		for	( srcp::SRCPGenericLoco* n = devices->firstGLElement(); n != 0; n = n->nextElement() )
			Serial << "GL " << n->getAddr() << " " << n->getStartAddr() << "-" << n->getEndAddr() << endl;
	}
	// EEPROM ausgeben
	else if ( strncasecmp(cmd, "ROM", 3) == 0 )
	{
		for ( int i = 1; ; i++ )
		{
			srcp::device_config_t device = Storage.getConfig( i );
			// EOF
			if	( device.start_addr == -1 )
				break;

			Serial << "EEPROM pos: " << i * (sizeof(srcp::device_config_t) + 2) << ", dev: " << (int) device.device << ":"
				   << (int) device.subDevice << ", addr " << device.start_addr << " - " << device.end_addr << ", args: ";
			for	( unsigned int i = 0; i < sizeof(device.args); i++ )
				Serial << (int) device.args[i] << " ";
			Serial << endl;
		}

	}
	// SRCP Befehl abarbeiten
	else
#endif
	{
		char *rc = session->dispatch( cmd );
		Serial << rc << '\r';
	}
}

}


