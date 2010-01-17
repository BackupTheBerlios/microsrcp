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

#ifndef ETHERNETSRCPSERVER_H_
#define ETHERNETSRCPSERVER_H_

#include <WProgram.h>
#include "ServerSocket.h"
#include "../srcp/SRCPDeviceMaster.h"
#include "../srcp/SRCPSession.h"

namespace lan
{

class EthernetSRCPServer
{
private:
	ServerSocket* commandSocket;
	ServerSocket* infoSocket;
	srcp::SRCPDeviceMaster *devices;
	srcp::SRCPSession *srcpSession;
	srcp::SRCPSession *infoSRCPSession;
protected:
	void run( srcp::SRCPSession *server, lan::ServerSocket* socket );
public:
	EthernetSRCPServer();
	void addDeviceManager( srcp::SRCPDeviceManager* manager ) { devices->addDeviceManager( manager ); }
	void begin( byte mac[], byte ip[], int port, srcp::device_config_t deviceConfig[] );
	void run();

	virtual ~EthernetSRCPServer();
};

}

extern lan::EthernetSRCPServer EthernetServer;

#endif /* ETHERNETSRCPSERVER_H_ */
