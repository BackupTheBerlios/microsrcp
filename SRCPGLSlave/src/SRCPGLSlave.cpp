/*
	SRCPGLSlave - I2C Board um 2 analoge Stromkreise zu regeln.

	Bestehend aus:
	- Arduino Board mit min. ATmega168p - http://www.arduino.cc
	- Motor Shield - http://arduino.cc/en/Main/ArduinoMotorShield oder
	http://www.nkcelectronics.com/freeduino-arduino-motor-control-shield-kit.html

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

#include "srcp/SRCPDeviceMaster.h"
#include "dev/CoreDeviceManager.h"
#include "i2c/I2CServer.h"

#define VERSION 10	// Version 1.0

// Definition der lokalen Geraete
srcp::device_config_t deviceConfig[] =
	{
		// SRCP Adresse 3 - Arduino http://arduino.cc/en/Main/ArduinoMotorShield - Pin 10 Geschwindigkeit, Pin 13 Laufrichtung
		{ 3, 0, srcp::GL, srcp::GLArduinoMotor , { 10, 13 } },
		// SRCP Adresse 3 - Arduino http://arduino.cc/en/Main/ArduinoMotorShield - Pin  9 Geschwindigkeit, Pin 12 Laufrichtung
		{ 4, 0, srcp::GL, srcp::GLArduinoMotor , { 9, 12 } },
		// EOF Geraete - nicht vergessen!
		{ -1 },
	};

void setup()
{
	//Serial.begin(19200);

	WireServer.addDeviceManager( new dev::CoreDeviceManager() );
	WireServer.begin( deviceConfig, srcp::BOARD_GL, VERSION );

	//Serial << "GL listen " <<  WireServer.getMyAddr() << endl;
}

void loop()
{
	WireServer.run();
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
