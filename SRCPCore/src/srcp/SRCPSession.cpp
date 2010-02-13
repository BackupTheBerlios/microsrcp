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

#include <stdio.h>
#include "SRCPSession.h"
#include "SRCPMessages.h"

namespace srcp
{

/** Globaler Counter wird bei jedem SRCP GO um ein aufgezaehlt */
int SRCPSession::counter = 1;

// Globaler IO-Buffer fuer Sensoren - Platz fuer 8 Sensoren und 9 als Endmerker \0
feedback fb[9];

SRCPSession::SRCPSession( SRCPDeviceMaster *manager )
{
	status = UNDEFINED;
	power = OFF;
	this->manager = manager;
}

char* SRCPSession::dispatch(char* args, int length )
{
	parse(args, length );

	switch (cmd.cmd)
	{
		case GO:
			power = ON;
			return	( Messages.go( counter++ ));
		case GET:
			return (Messages.ok());
		case SET:
			switch (cmd.device)
			{
				case POWER:
					power = (power_enum) cmd.values[0];
					manager->setPower( power );
					return (Messages.ok());

				case FB:
					return (Messages.ok());

				case GA:
					cmd.values[0] = manager->setGA( cmd.addr, cmd.values[0], cmd.values[1], cmd.values[2] );
					return (Messages.ok());

				case GL:
					cmd.values[0] = manager->setGL( cmd.addr, cmd.values[0], cmd.values[1], cmd.values[2], cmd.values );
					return (Messages.ok());

				default:
					return (Messages.error(421));
			}
			break;
		case CONNECTIONMODE:
			status = cmd.values[0];
			return	( Messages.ok202() );

		case PROTOCOL:
			return	( Messages.ok201() );

		default:
			return (Messages.ok());
	}
	//return (Messages.error(500));
	return (Messages.ok());
}

const char* SRCPSession::version()
{
	status = HANDSHAKE;
	return	( Messages.version() );
}

void SRCPSession::infoFeedback( Print* out )
{
	for	( SRCPFeedback* e = manager->firstFeedbackElement(); e != 0; e = e->nextElement() )
	{
		e->info( 0, &fb[0] );
		for	( int i = 0; fb[i].pin != 0; i++ )
		{
			char* m = Messages.info( 0, "FB", fb[i].pin, fb[i].value );
			//Serial << "rc " << m << endl;
			out->println( m );
		}
	}
}

void SRCPSession::refresh()
{
	manager->refresh();
}

void SRCPSession::parse( char* args, int length )
{
	char d[10];
	memset( cmd.values, 0, sizeof(cmd.values) );

	cmd.cmd = UNKNOWN;
	cmd.device = NA;
	cmd.args[0] = '\0';

	if (strncasecmp(args, "SET CONNECTIONMODE SRCP", 23) == 0  )
	{
		cmd.cmd = CONNECTIONMODE;
		cmd.values[0] = COMMAND;
		strcpy( d, args+24 );
		if	( strncasecmp(d, "INFO", 4) == 0 )
			cmd.values[0] = INFO;
	}
	else if ( strncasecmp( args, "SET PROTOCOL SRCP", 17) == 0 )
	{
		cmd.cmd = PROTOCOL;
		strcpy( cmd.args, args+18 );
	}
	else if (strncasecmp(args, "SET", 3) == 0)
	{
		// GL hat max. 12 (15 - 3) Funktionen, siehe SRCP_MAX_ARGS
		cmd.cmd = SET;
		sscanf( args, "%*s %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &cmd.bus, d, &cmd.addr, &cmd.values[0], &cmd.values[1], &cmd.values[2],
				&cmd.values[3], &cmd.values[4], &cmd.values[5], &cmd.values[6], &cmd.values[7], &cmd.values[8], &cmd.values[9],
				&cmd.values[10], &cmd.values[11], &cmd.values[12], &cmd.values[13], &cmd.values[14] );
		cmd.device = getDevice( d );

		if	( cmd.device == POWER )
		{
			sscanf( args,  "%*s %*d %*s %s", cmd.args );
			if	( strncasecmp( cmd.args, "ON", 2) == 0 )
				cmd.values[0] = ON;
			else
				cmd.values[0] = OFF;
		}
	}
	else if (strncasecmp(args, "GET", 3) == 0)
	{
		cmd.cmd = GET;
	}
	else if (strncasecmp(args, "WAIT", 4) == 0)
		cmd.cmd = WAIT;
	else if (strncasecmp(args, "INIT", 4) == 0)
		cmd.cmd = INIT;
	else if (strncasecmp(args, "TERM", 4) == 0)
		cmd.cmd = TERM;
	else if (strncasecmp(args, "GO", 2) == 0)
		cmd.cmd = GO;
	else
		cmd.cmd = UNKNOWN;
}

devices SRCPSession::getDevice( char* device )
{
	if	( strncasecmp(device, "POWER", 5) == 0 )
		return	( POWER );
	else if	( strncasecmp(device, "FB", 2) == 0 )
		return	( FB );
	else if	( strncasecmp(device, "GA", 2) == 0 )
		return	( GA );
	else if	( strncasecmp(device, "GL", 2) == 0 )
		return	( GL );

	return	( NA );
}

}
