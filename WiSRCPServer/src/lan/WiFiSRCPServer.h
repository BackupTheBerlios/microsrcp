/*
	WiFiSRCPServer - Kompletter SRCP Server mit Abhandlung
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

#ifndef WiFiSRCPServer_H_
#define WiFiSRCPServer_H_


#include <WProgram.h>
#include "srcp/SRCPDeviceMaster.h"
#include "srcp/SRCPSession.h"
#include "WiFiPrint.h"

namespace lan
{

#define MAX_SRCP_SOCKETS 4

class WiFiSRCPServer
{
private:
	srcp::SRCPDeviceMaster *devices;
	srcp::SRCPSession *srcpSession[MAX_SRCP_SOCKETS];
	int id[MAX_SRCP_SOCKETS];
	WiFiPrint out;
protected:
	void run( srcp::SRCPSession *server );
public:
	WiFiSRCPServer();
	void addDeviceManager( srcp::SRCPDeviceManager* manager ) { devices->addDeviceManager( manager ); }
	void begin( srcp::device_config_t deviceConfig[] );
	void connected( int id );
	void newData( int id, char* buf );
	void closed( int id );
	void run();

	virtual ~WiFiSRCPServer();
};

}

extern lan::WiFiSRCPServer WiFiServer;

#endif /* WiFiSRCPServer_H_ */
