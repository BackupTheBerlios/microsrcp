/*
	ServerSocket - Ersatz fuer die Klassen Server und Client
	aus der Arduino Library.

	Diese konnten nicht verwendet werden, weil:
	- keine Abfrage auf connected wenn keine Daten vorhanden sind
	- keine zwei listen Ports auf dem gleichen Port.

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

#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include "Ethernet.h"

namespace lan
{

class ServerSocket : public Print
{
private:
	int port;
	int sock;
public:
	ServerSocket( int port );
	void begin();
	int available();
	int read();
	uint8_t status();
	int connected();
	void stop();
	virtual void write( uint8_t );
	virtual void write( const char *str );
	virtual void write( const uint8_t *buf, size_t size );

};

}

#endif /* SERVERSOCKET_H_ */
