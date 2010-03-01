/*
	SRCPServer - SRCP Zentrale - empfaengt die Daten via TCP/IP und
	leitet sie mittels I2C Bus an die anderen Boards weiter.

	Bestehend aus:
	- Arduino Board mit min. ATmega328p - http://www.arduino.cc
	- Ethernet Shield - http://www.arduino.cc/en/Main/ArduinoEthernetShield

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

#include "lan/EthernetSRCPServer.h"

#include "srcp/SRCPCommand.h"
#include "srcp/SRCPDeviceMaster.h"
#include "i2c/I2CDeviceManager.h"
#include "i2c/I2CUtil.h"
#include "cp/EStorage.h"

#include "shell/SRCPShell.h"

#define VERSION 12		// Version x.y

// Definition der I2C Boards - muss fuer weitere Boards erweitert werden.
srcp::device_config_t deviceConfig[] =
	{
		// Platzhalter damit IP, Port mittels EEPROM verwaltet werden koennen
		{ 0, 0, srcp::LAN, srcp::IP,      { 192, 168, 1, 241 } },
		{ 0, 0, srcp::LAN, srcp::SRCP_PORT, { 16, 207 } },	// 4303/256 + 4303%256 = 4303

		// alle Geraete werden im I2C Netzwerk gesucht und anhand der gefunden
		// I2C Boards eingetragen. Pro I2C Adresse ohne Board (FB, GA) werden 8 Adressen
		// reserviert.
		{ 1, 9999, srcp::LAN, srcp::I2CDESCRIPTION, { 8, 8 } },

		// EOF Geraete - nicht vergessen!
		{ -1 },
	};

void setup()
{
	Serial.begin( 19200 );

	// initialize I2C - Master braucht keine Adresse, muss als erstes erfolgen
	// sonst kann der I2C Bus nicht durchsucht werden.
	i2c::I2CUtil::begin( 0 );

	//EthernetServer.addDeviceManager( &CoreDevices );
	EthernetServer.addDeviceManager( new i2c::I2CDeviceManager() );
	srcp::device_config_t config = Storage.getConfig( 2 );
	int port = config.args[0] * 256 + config.args[1];
	config = Storage.getConfig( 1 );
	EthernetServer.begin( config.args, port, deviceConfig, srcp::BOARD_CPU, VERSION );

	// Shell braucht Zugriff auf Geraete und eine SRCP Session
	Shell.begin( EthernetServer.getDevices(), EthernetServer.getSession() );

	Serial << "Server listen " << (int) config.args[0] << "." << (int) config.args[1] << "."
		   << (int) config.args[2] << "." << (int) config.args[3] << ":" << port << endl;
}

void loop()
{
	EthernetServer.run();
	Shell.run();
	delay( 15 );
}

int main( void )
{
	init();

	setup();

	for ( ;; )
		loop();

	return 0;
}

extern "C" void __cxa_pure_virtual()
{
	cli();
	for ( ;; )
		;
}
