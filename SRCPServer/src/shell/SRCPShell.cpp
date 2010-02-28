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
			char *rc = session->dispatch( cmd );
			Serial << rc << '\r';
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
}


