/*

	SRCPServer - WLan SRCP Zentrale - empfaengt die Daten via TCP/IP und
	leitet sie mittels I2C Bus an die anderen Boards weiter.

	Bestehend aus:
	- Arduino Board mit min. ATmega328p - http://www.arduino.cc
	- WiShield - http://asynclabs.com/wiki/index.php?title=AsyncLabsWiki

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
#include <WiShield.h>

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1,242};	// IP address of WiShield
unsigned char gateway_ip[] = {192,168,1,1};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"xxx"};		// max 32 bytes

unsigned char security_type = 2;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"xxx"};	// max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = {	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00,	// Key 1
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00,	// Key 2
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00	// Key 3
								};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
//---------------------------------------------------------------------------

#include "lan/WiFiSRCPServer.h"
//#include "dev/CoreDeviceManager.h"
#include "i2c/I2CDeviceManager.h"
#include "i2c/I2CUtil.h"

// Definition der I2C Boards - muss fuer weitere Boards erweitert werden.
srcp::device_config_t deviceConfig[] =
	{
		////////////////////// Board 1 - Universal (Servos, Lichtsignale, Rueckmelder) /////////////
		// Die SRCP Adressen 32 - 63 werden an I2C Board 1 weitergeleitet
		{ 32, 63, srcp::GA, srcp::I2CGAMaster, { 1 } },
		// Die SRCP Adressen 64 - 96 werden an I2C Board 1 weitergeleitet
		{ 64, 96, srcp::GA, srcp::I2CGAMaster, { 1 } },
		// Die SRCP Rueckmelder 1 - 8 befinden sich auf I2C Board 1
		{ 1, 8, srcp::FB, srcp::I2CFBMaster, { 1 } },

		////////////////////// Board 90 - 92 - Abspielen von Wave Dateien /////////////
		// Die SRCP Adressen 100 - 199 werden an I2C Board 90 weitergeleitet
		{ 100, 199, srcp::GA, srcp::I2CGAMaster, { 90 } },

		////////////////////// Board 99 - OpenDCC //////////////////////////////////////////////////
		// Loks mit der SRCP Adresse 10 bis 9999 werden an I2C Board 99 weitergeleitet
		{ 10, 9999, srcp::GL, srcp::I2CGLMaster, { 99 } },
		// Zubehoer mit der SRCP Adresse 1 und 999 werden an I2C Board 99 weitergeleitet
		{  1,  999, srcp::GA, srcp::I2CGAMaster, { 99 } },

		////////////////////// Board 100 - Motorentreiber //////////////////////////////////////////
		// Loks mit der SRCP Adresse 3 und 4 werden an I2C Board 100 weitergeleitet
		{ 3, 4, srcp::GL, srcp::I2CGLMaster, { 100 } },

		// EOF Geraete - nicht vergessen!
		{ -1 },
	};

void setup()
{
	Serial.begin( 19200 );
	Serial << "start" << endl;

	//WiFiServer.addDeviceManager( &CoreDevices );
	WiFiServer.addDeviceManager( new i2c::I2CDeviceManager() );
	WiFiServer.begin( deviceConfig );

	// initialize I2C - Master braucht keine Adresse
	i2c::I2CUtil::begin( 0 );

	// WLAN initialisieren
	WiFi.init();

	Serial << "setup i.o. " << endl;

}

void loop()
{
	WiFi.run();
	WiFiServer.run();
	delay( 100 );
}

int main(void)
{
	init();

	setup();

	for (;;)
		loop();

	return 0;
}

extern "C" void __cxa_pure_virtual()
{
	cli();
	for (;;)
		;
}


