/*
 Shell - einfache Shell um den SRCP Server mittels Serieller
 Schnittstelle zu bedienen.

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

#ifndef SRCP_SHELL_H_
#define SRPC_SHELL_H_

#include <../srcp/SRCPDeviceMaster.h>
#include <../srcp/SRCPSession.h>

namespace shell
{

class SRCPShell
{
private:
	char cmd[64];
	unsigned int pos;
	srcp::SRCPDeviceMaster *devices;
	srcp::SRCPSession *session;
	void dispatch( char *cmd );
public:
	void begin( srcp::SRCPDeviceMaster *devices, srcp::SRCPSession *session );
	void run();

};

}
extern shell::SRCPShell Shell;

#endif /* SRCP_SHELL_H_ */
