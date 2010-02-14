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


#include "WProgram.h"
#include "lan/EthernetSRCPServer.h"

#include "srcp/SRCPDeviceMaster.h"
#include "dev/CoreDeviceManager.h"
#include "i2c/I2CDeviceManager.h"
#include "i2c/I2CUtil.h"

// network configuration.  gateway and subnet are optional.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 241 };

#define VERSION 11		// Version x.y

// Definition der I2C Boards - muss fuer weitere Boards erweitert werden.
srcp::device_config_t deviceConfig[] =
	{
		////////////////////// Board 1 - Universal (Servos, Lichtsignale, Rueckmelder) /////////////
		// 6 Servo und 2 Lichtsignale
		{ 1, 8, srcp::GA, srcp::I2CGAMaster, { 1 } },
		// 8 Rueckmelder
		{ 1, 8, srcp::FB, srcp::I2CFBMaster, { 1 } },

		////////////////////// Board 2 - Universal (Servos, Lichtsignale, Rueckmelder) /////////////
		// 6 Servo und 2 Lichtsignale
		{ 9, 16, srcp::GA, srcp::I2CGAMaster, { 2 } },
		// 8 Rueckmelder
		{ 9, 16, srcp::FB, srcp::I2CFBMaster, { 2 } },

		////////////////////// Board 3 - Universal (Servos, Lichtsignale, Rueckmelder) /////////////
		// 6 Servo und 2 Lichtsignale
		{ 17, 24, srcp::GA, srcp::I2CGAMaster, { 3 } },
		// 8 Rueckmelder
		{ 17, 24, srcp::FB, srcp::I2CFBMaster, { 3 } },

		////////////////////// Board 4 - etc. /////////////
#if	( DEBUG_SCOPE == 0 ) // Serial braucht viel Speicher, ansonsten Memoryoverflow!
		{ 25, 32, srcp::GA, srcp::I2CGAMaster, { 4 } },
		{ 25, 32, srcp::FB, srcp::I2CFBMaster, { 4 } },
		{ 33, 40, srcp::GA, srcp::I2CGAMaster, { 5 } },
		{ 33, 40, srcp::FB, srcp::I2CFBMaster, { 5 } },
		{ 41, 48, srcp::GA, srcp::I2CGAMaster, { 6 } },
		{ 41, 48, srcp::FB, srcp::I2CFBMaster, { 6 } },
		{ 49, 56, srcp::GA, srcp::I2CGAMaster, { 7 } },
		{ 49, 56, srcp::FB, srcp::I2CFBMaster, { 7 } },
		{ 57, 64, srcp::GA, srcp::I2CGAMaster, { 8 } },
		{ 57, 64, srcp::FB, srcp::I2CFBMaster, { 8 } },
		{ 65, 72, srcp::GA, srcp::I2CGAMaster, { 9 } },
		{ 65, 72, srcp::FB, srcp::I2CFBMaster, { 9 } },
		{ 73, 80, srcp::GA, srcp::I2CGAMaster, { 10 } },
		{ 73, 80, srcp::FB, srcp::I2CFBMaster, { 10 } },
#endif
		////////////////////// Board 90 - 92 - Abspielen von Wave Dateien /////////////
		// Die SRCP Adressen 100 - 199 werden an I2C Board 90 weitergeleitet
		{ 100, 199, srcp::GA, srcp::I2CGAMaster, { 90 } },
#if	( DEBUG_SCOPE == 0 )
		{ 200, 299, srcp::GA, srcp::I2CGAMaster, { 91 } },
		{ 300, 399, srcp::GA, srcp::I2CGAMaster, { 92 } },
#endif

		////////////////////// Board 99 - OpenDCC //////////////////////////////////////////////////
		// Loks mit der SRCP Adresse 10 bis 9999 werden an I2C Board 99 weitergeleitet
		{ 10, 9999, srcp::GL, srcp::I2CGLMaster, { 99 } },
		// Zubehoer mit der SRCP Adresse 1 und 999 werden an I2C Board 99 weitergeleitet
		{  1,  999, srcp::GA, srcp::I2CGAMaster, { 99 } },

		////////////////////// Board 100 - Motorentreiber //////////////////////////////////////////
		// Loks mit der SRCP Adresse 3 und 4 werden an I2C Board 100 weitergeleitet
		//{ 3, 4, srcp::GL, srcp::I2CGLMaster, { 100 } },

		// EOF Geraete - nicht vergessen!
		{ -1 },
	};

void setup()
{
#if	( DEBUG_SCOPE > 0 )
	Serial.begin( 19200 );
#endif

	//EthernetServer.addDeviceManager( &CoreDevices );
	EthernetServer.addDeviceManager( new i2c::I2CDeviceManager() );
	EthernetServer.begin( mac, ip, 4303, deviceConfig, srcp::BOARD_CPU, VERSION );

	// initialize I2C - Master braucht keine Adresse
	i2c::I2CUtil::begin( 0 );

#if	( DEBUG_SCOPE > 0 )
	Serial << "Server setup i.o. " << endl;
#endif
}

void loop()
{
	EthernetServer.run();
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
