/*
	SRCPGASlave - I2C Board um 6 Servos, 2 Lichtsignale und 7 Rueckmelder
	zu verwalten. Pinbelegungen siehe unten.

	Bestehend aus:
	- Arduino Board mit min. ATmega168p - http://www.arduino.cc
	- Proto Shield - http://www.arduino.cc/playground/Tutorials/Protoshield

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

#include "dev/CoreDeviceManager.h"
#include "i2c/I2CServer.h"

// Meine I2C Adresse, muss fuer weitere Boards angepasst werden.
#define MY_ADDR	1

// Definition der lokalen Geraete
srcp::device_config_t deviceConfig[] =
	{
		// lokale Adresse 1, Servo an Pin 0, Servoradius von 60 - 120 verwenden.
		{ 1, 0, srcp::GA, srcp::GAServo , {0, 60, 120} },	// Servo Pin 0 - 5
		{ 2, 0, srcp::GA, srcp::GAServo , {1, 60, 120} },
		{ 3, 0, srcp::GA, srcp::GAServo , {2, 60, 120} },
		{ 4, 0, srcp::GA, srcp::GAServo , {3, 60, 120} },
		{ 5, 0, srcp::GA, srcp::GAServo , {4, 60, 120} },
		{ 6, 0, srcp::GA, srcp::GAServo , {5, 60, 120} },
		// lokale Adresse 1 bis 8, Rueckmelder an Pins 6 - 12
		{ 1, 8, srcp::FB, srcp::FBSwitchSensor, { 6, 12 } },	// Sensoren Pin 6 - 12, 13 = LED
		// lokale Adresse 7, Lichtsignal, Rot an Pin 14, Gruen an Pin 15.
		{ 7, 0, srcp::GA, srcp::GASignal, {14, 15} },		// Signale Pin ADC 0 - 4, Pin 14 - 17
		{ 8, 0, srcp::GA, srcp::GASignal, {16, 17} },
		// EOF Geraete - nicht vergessen!
		{ -1 },
};

void setup()
{
	//Serial.begin(19200);

	WireServer.addDeviceManager( new dev::CoreDeviceManager() );
	WireServer.begin( MY_ADDR, deviceConfig );
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
