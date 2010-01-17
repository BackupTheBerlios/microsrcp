/*
	SRCPSession - Handelt eine SRCP Session ab, z.B.
	zusammen mit EthernetSRCPServer verwenden.

	Das Protokoll entspricht mindestens 0.8.2
	Siehe auch: http://srcpd.sourceforge.net/srcp/

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

#ifndef SRCPSESSION_H_
#define SRCPSESSION_H_

#include "WProgram.h"
#include "SRCPPower.h"
#include "SRCPFeedback.h"
#include "SRCPGenericAccessoire.h"
#include "SRCPGenericLoco.h"
#include "SRCPCommand.h"
#include "SRCPDeviceMaster.h"

namespace srcp
{

enum { UNDEFINED, HANDSHAKE, COMMAND, INFO };
enum power_enum { ON, OFF };

class SRCPSession
{
private:
	int status;
	static int counter;
	power_enum power;
	SRCPDeviceMaster* manager;
	command_t cmd;
	void parse( char* args, int length = 0 );
	devices getDevice( char* device );
public:
	SRCPSession( SRCPDeviceMaster *manager );
	void setProtocol( char *version );
	void setConnectionMode( char* mode );
	int Go();
	int connect();
	char* dispatch( char* args, int length = 0 );
	const char* version();
	int getStatus() { return (status); }
	void disconnect() { power = OFF; status = UNDEFINED; };
	char* sendFeedback( char *buf );
	int isPowerOn() { return( power == ON ); }
	void infoFeedback( Print* out  );
	void refresh();
	command_t getCommand() { return ( cmd ); }
};

}

#endif /* SRCPSESSION_H_ */
