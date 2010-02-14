/*
	EthernetSRCPServer - Kompletter SRCP Server mit Abhandlung
	der COMMAND und INFO Sessions.

	Dazu werden zwei TCP/IP Ports und zwei SRCPSession benoetigt.

	Fuer die TCP/IP Kommunikation wird ein spezieller Shield
	benoetigt (nur der ist getestet!).

	Das Protokoll entspricht mindestens 0.8.2
	Siehe auch: http://srcpd.sourceforge.net/srcp/

	Im Sketch als EthernetServer ansprechbar.

	Beispiele fuer eine Implementierung siehe SRCPSession Sketch/Projekt.

	Getestet mit:
	- RocRail - http://www.rocrail.net
	- jsrcpc - SRCP Java Client Implementierung

	Shield:
	http://arduino.cc/en/Main/ArduinoEthernetShield

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

#include "EthernetSRCPServer.h"

namespace lan
{
char command[64];
int pos = 0;
long last = millis();

EthernetSRCPServer::EthernetSRCPServer()
{
	devices = new srcp::SRCPDeviceMaster();

	srcpSession = new srcp::SRCPSession( devices );
	infoSRCPSession = new srcp::SRCPSession( devices );
}

void EthernetSRCPServer::begin( byte mac[], byte ip[], int port, srcp::device_config_t deviceConfig[], int id, int version )
{
	commandSocket = new ServerSocket( port );
	infoSocket = new ServerSocket( port );

	// initialize the ethernet device
	Ethernet.begin( mac, ip );
	commandSocket->begin();
	infoSocket->begin();

	// initialize devices
	devices->init( deviceConfig, id, version );
}

void EthernetSRCPServer::run()
{
	run( srcpSession, commandSocket );
	run( infoSRCPSession, infoSocket );
}

void EthernetSRCPServer::run( srcp::SRCPSession *server, lan::ServerSocket* socket )
{
	if ( socket->connected() )
	{
		// noch nicht verbunden - zuerst Versioninfo senden
		if	( server->getStatus() == srcp::UNDEFINED )
		{
#if	( DEBUG_SCOPE > 0 )
			Serial << "conn : " << server->getStatus() << endl;
#endif
			socket->print( server->version() );
		}

		// verbunden - Daten empfangen
		while ( socket->available() != 0 )
		{
			int byte = socket->read();
			if ( byte != '\n' )
			{
				command[pos] = byte;
				pos++;
				continue;
			}
			if ( byte == '\r' )
				continue;
			command[pos] = '\0';

#if	( DEBUG_SCOPE > 0 )
			Serial << "data : " << server->getStatus() << ", " << command << endl;
#endif
			char* rc = server->dispatch( command );

#if	( DEBUG_SCOPE > 0 )
			Serial << "rc   : " << server->getStatus() << ", " << rc << '\r';
#endif
			socket->print( rc );
			pos = 0;
			break;
		}

		// Info Server
		if	( server->getStatus() == srcp::INFO && server->isPowerOn() )
		{
			// evtl. FB Module refreshen - auch wenn noch nicht gesendet wird.
			server->refresh();

			if	( last+250 < millis() )
			{
				server->infoFeedback( socket );
				last = millis();
			}
		}
	}
	else
	{
		if	( server->getStatus() != srcp::UNDEFINED )
		{
#if	( DEBUG_SCOPE > 0 )
			Serial << "disconnect" << endl;
#endif
			server->disconnect();
			socket->stop();
			// auf ein neues!
			socket->begin();
		}
	}
}

EthernetSRCPServer::~EthernetSRCPServer()
{
	delete commandSocket;
	delete infoSocket;
	delete srcpSession;
	delete infoSRCPSession;
}

}

lan::EthernetSRCPServer EthernetServer = lan::EthernetSRCPServer();
