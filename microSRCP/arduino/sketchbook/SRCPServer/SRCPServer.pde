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

#include <Servo.h>
#include <Ethernet.h>
#include <I2CUtil.h>
#include <I2CDeviceManager.h>
#include <SRCPDeviceMaster.h>
#include <EthernetSRCPServer.h>
#include <ServerSocket.h>
#include <CoreDeviceManager.h>

// network configuration.  gateway and subnet are optional.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 241 };

// Definition der I2C Boards - muss fuer weitere Boards erweitert werden.
srcp::device_config_t deviceConfig[] =
	{
		////////////////////// Board 1 //////////////////////////////////////////
		// Die SRCP Adressen 32 - 63 werden an I2C Board 1 weitergeleitet
		{ 32, 63, srcp::GA, srcp::I2CGAMaster, { 1 } },
		// Die SRCP Adressen 64 - 96 werden an I2C Board 1 weitergeleitet
		{ 64, 96, srcp::GA, srcp::I2CGAMaster, { 1 } },
		// Die SRCP Rueckmelder 1 - 8 befinden sich auf I2C Board 1
		{ 1, 8, srcp::FB, srcp::I2CFBMaster, { 1 } },
		// Loks mit der SRCP Adresse 3 und 4 werden an I2C Board 100 weitergeleitet

		////////////////////// Board 100 //////////////////////////////////////////
		{ 3, 4, srcp::GL, srcp::I2CGLMaster, { 100 } },
		// EOF Geraete - nicht vergessen!
		{ -1 },
	};

void setup()
{
  EthernetServer.addDeviceManager( new i2c::I2CDeviceManager() );
  EthernetServer.begin( mac, ip, 4303, deviceConfig );

  // initialize I2C
  i2c::I2CUtil::begin( 0 );
}

void loop()
{
  // Empfaengt SRCP Befehle via TCP/IP, leitet diese via I2C weiter, fragt Rueckmelder via I2C ab.
  EthernetServer.run();
  // Hier gibt es noch Raum fuer Erweiterungen
  delay( 15 );
}


