/*
	WiFiSRCPServer - Kompletter SRCP Server mit Abhandlung
	der COMMAND und INFO Sessions.

	Dazu werden zwei TCP/IP Ports und zwei SRCPSession benoetigt.

	Fuer die TCP/IP Kommunikation wird ein der WiShield
	benoetigt (nur der ist getestet!).

	Das Protokoll entspricht mindestens 0.8.2
	Siehe auch: http://srcpd.sourceforge.net/srcp/

	Im Sketch als WiFiServer ansprechbar.

	Beispiele fuer eine Implementierung siehe SRCPSession Sketch/Projekt.

	Getestet mit:
	- RocRail - http://www.rocrail.net
	- jsrcpc - SRCP Java Client Implementierung

	Shield:
	http://asynclabs.com/wiki/index.php?title=AsyncLabsWiki

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

#include "WiFiPrint.h"
#include "WiFiSRCPServer.h"

//////////// Aufrufe von socketapp.c //////////////////////////////////////////
extern "C" void WiFi_connected(  int id ) { WiFiServer.connected( id ); }
extern "C" void WiFi_newData( int id, char* buf ) { WiFiServer.newData( id, buf ); }
extern "C" void WiFi_closed( int id ) { WiFiServer.closed( id ); }

namespace lan
{
long last = millis();

WiFiSRCPServer::WiFiSRCPServer()
{
	devices = new srcp::SRCPDeviceMaster();

	for	( int i = 0; i < MAX_SRCP_SOCKETS; i++ )
	{
		srcpSession[i] = 0;
		id[i] = 0;
	}
}

void WiFiSRCPServer::begin( srcp::device_config_t deviceConfig[] )
{
	devices->init( deviceConfig );
}

void WiFiSRCPServer::connected( int id )
{
	int i = 0;
	for	( ; i < MAX_SRCP_SOCKETS; i++ )
		if	( this->id[i] == 0 )
		{
			this->id[i] = id;
			srcpSession[i] = new srcp::SRCPSession( devices );
			break;
		}
	// to many sockets
	if	( i == MAX_SRCP_SOCKETS )
		return;

	out.write( srcpSession[i]->version() );
	Serial << "conn : " << srcpSession[i]->getStatus() << ", " << id << endl;
}

void WiFiSRCPServer::newData( int id, char* buf )
{
	int i = 0;
	for	( ; i < MAX_SRCP_SOCKETS; i++ )
		if	( this->id[i] == id )
			break;
	// to many sockets
	if	( i == MAX_SRCP_SOCKETS )
		return;

	Serial << "data : " << srcpSession[i]->getStatus() << ", " << id << ", " << buf << endl;
	char* rc = srcpSession[i]->dispatch( buf );

	Serial << "rc   : " << srcpSession[i]->getStatus() << ", " << id << ", " << rc << "\r";
	out.write( rc );
}

void WiFiSRCPServer::run()
{
	for ( int i = 0; i < MAX_SRCP_SOCKETS; i++ )
		if ( id[i] != 0 )
			run( srcpSession[i] );
}

void WiFiSRCPServer::run( srcp::SRCPSession *server )
{
	// Info Server
	if	( server->getStatus() == srcp::INFO && server->isPowerOn() )
	{
		// evtl. FB Module refreshen - auch wenn noch nicht gesendet wird.
		server->refresh();

		if	( last+250 < millis() )
		{
			server->infoFeedback( &out );
			last = millis();
		}
	}
}

void WiFiSRCPServer::closed( int id )
{
	int i = 0;
	for	( ; i < MAX_SRCP_SOCKETS; i++ )
		if	( this->id[i] == id )
			break;
	// to many sockets
	if	( i == MAX_SRCP_SOCKETS )
		return;

	Serial << "close: " << srcpSession[i]->getStatus() << ", " << id << endl;
	srcpSession[i]->disconnect();
	delete srcpSession[i];
	srcpSession[i] = 0;
	this->id[i] = 0;
}

WiFiSRCPServer::~WiFiSRCPServer()
{
	for	( int i = 0; i < MAX_SRCP_SOCKETS; i++ )
		if	( srcpSession[i] != 0 )
			delete srcpSession[i];
}

}

lan::WiFiSRCPServer WiFiServer = lan::WiFiSRCPServer();
