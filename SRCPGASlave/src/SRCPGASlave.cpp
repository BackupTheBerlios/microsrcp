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

#include "srcp/SRCPDeviceMaster.h"
#include "dev/CoreDeviceManager.h"
#include "i2c/I2CServer.h"

#define VERSION 11	// Version x.y der EEPROM Daten

/*// Definition der lokalen Geraete - 1. Version, als Arduino Shield aufgebaut.
srcp::device_config_t deviceConfig[] =
	{
		// lokale Adresse 1, Servo an Pin 0, Servoradius von 87 - 93 (Mittelstellung) verwenden.
		{ 1, 0, srcp::GA, srcp::GAServo , {0, 87, 93} },	// Servo Pin 0 - 5
		{ 2, 0, srcp::GA, srcp::GAServo , {1, 87, 93} },
		{ 3, 0, srcp::GA, srcp::GAServo , {2, 87, 93} },
		{ 4, 0, srcp::GA, srcp::GAServo , {3, 87, 93} },
		{ 5, 0, srcp::GA, srcp::GAServo , {4, 87, 93} },
		{ 6, 0, srcp::GA, srcp::GAServo , {5, 87, 93} },
		// lokale Adresse 1 bis 8, Rueckmelder an Pins 6 - 12
		{ 1, 8, srcp::FB, srcp::FBSwitchSensor, { 6, 12 } },	// Sensoren Pin 6 - 12, 13 = LED
		// lokale Adresse 7, Lichtsignal, Rot an Pin 14, Gruen an Pin 15.
		{ 7, 0, srcp::GA, srcp::GASignal, {14, 15} },		// Signale Pin ADC 0 - 4, Pin 14 - 17
		{ 8, 0, srcp::GA, srcp::GASignal, {16, 17} },
		// EOF Geraete - nicht vergessen!
		{ -1 },
};*/

// Definition der lokalen Geraete - 2. Version, minimale Bausteine (Resonator, 10K, Atmega328P)
// aufgebaut auf Laborkarte RE315 von Roth Elektronik. Kein Arduino Standard Board mehr!
/*srcp::device_config_t deviceConfig[] =
	{
		// lokale Adresse 1, Servo an Pin 0, Servoradius von 87 - 93 (Mittelstellung) verwenden.
		{ 1, 0, srcp::GA, srcp::GAServo , {9, 87, 93} },	// Servo Pin D9 und D0 - D4
		{ 2, 0, srcp::GA, srcp::GAServo , {0, 87, 93} },
		{ 3, 0, srcp::GA, srcp::GAServo , {1, 87, 93} },
		{ 4, 0, srcp::GA, srcp::GAServo , {2, 87, 93} },
		{ 5, 0, srcp::GA, srcp::GAServo , {3, 87, 93} },
		{ 6, 0, srcp::GA, srcp::GAServo , {4, 87, 93} },
		// lokale Adresse 7, Lichtsignal
		{ 7, 0, srcp::GA, srcp::GASignal, {5, 6} },		// Signale Pin D5 + D6 und D7 + D8
		{ 8, 0, srcp::GA, srcp::GASignal, {7, 8} },
		// lokale Adresse 1 bis 8, Rueckmelder an Pins 6 - 12
		{ 1, 8, srcp::FB, srcp::FBSwitchSensor, { 10, 17 } },	// Sensoren Pin D10 - D13 und C0 - C3
		// EOF Geraete - nicht vergessen!
		{ -1 },
};*/

// Definition der lokalen Geraete - 3. Version, mit DFRobot IO Expansion Shield For Arduino(V5)
srcp::device_config_t deviceConfig[] =
	{
		// lokale Adresse 7, Lichtsignal
		{ 7, 0, srcp::GA, srcp::GASignal, {4, 5} },		// Signale Pin D5 + D6 und D7 + D8
		{ 8, 0, srcp::GA, srcp::GASignal, {6, 7} },
		// lokale Adresse 1, Servo an Pin 0, Servoradius von 87 - 93 (Mittelstellung) verwenden.
		{ 1, 0, srcp::GA, srcp::GAServo , {8, 87, 93} },	// Servo Pin 0 - 5
		{ 2, 0, srcp::GA, srcp::GAServo , {9, 87, 93} },
		{ 3, 0, srcp::GA, srcp::GAServo , {10, 87, 93} },
		{ 4, 0, srcp::GA, srcp::GAServo , {11, 87, 93} },
		{ 5, 0, srcp::GA, srcp::GAServo , {12, 87, 93} },
		{ 6, 0, srcp::GA, srcp::GAServo , {13, 87, 93} },

		// lokale Adresse 1 bis 8, Rueckmelder an Pins 14 - 17
		//{ 1, 8, srcp::FB, srcp::FBSwitchSensor, { 0, 3 } },		// Digital Pin 0 - 3
		{ 1, 8, srcp::FB, srcp::FBSwitchSensor, { 14, 17 } },	// Sensoren Pin ADC 0 - 4
		// EOF Geraete - nicht vergessen!
		{ -1 },
};
// Definition der lokalen Geraete - 4. Version, mit DFRobot IO Expansion Shield For Arduino Mega!
/*srcp::device_config_t deviceConfig[] =
	{
		// Pin 0 + 1 Serielle Schnittstelle 1 - frei
		// lokale Adresse 1, Servos an Pin 2 - 37 Servoradius von 87 - 93 (Mittelstellung) verwenden.
		{  1, 0, srcp::GA, srcp::GAServo , {2, 87, 93} },
		{  2, 0, srcp::GA, srcp::GAServo , {3, 87, 93} },
		{  3, 0, srcp::GA, srcp::GAServo , {4, 87, 93} },
		{  4, 0, srcp::GA, srcp::GAServo , {5, 87, 93} },
		{  5, 0, srcp::GA, srcp::GAServo , {6, 87, 93} },
		{  6, 0, srcp::GA, srcp::GAServo , {7, 87, 93} },
		{  7, 0, srcp::GA, srcp::GAServo , {8, 87, 93} },
		{  8, 0, srcp::GA, srcp::GAServo , {9, 87, 93} },
		{  9, 0, srcp::GA, srcp::GAServo , {10, 87, 93} },
		{ 10, 0, srcp::GA, srcp::GAServo , {11, 87, 93} },
		{ 11, 0, srcp::GA, srcp::GAServo , {12, 87, 93} },
		{ 12, 0, srcp::GA, srcp::GAServo , {13, 87, 93} },
		{ 13, 0, srcp::GA, srcp::GAServo , {14, 87, 93} },
		{ 14, 0, srcp::GA, srcp::GAServo , {15, 87, 93} },
		{ 15, 0, srcp::GA, srcp::GAServo , {16, 87, 93} },
		{ 16, 0, srcp::GA, srcp::GAServo , {17, 87, 93} },
		{ 17, 0, srcp::GA, srcp::GAServo , {18, 87, 93} },
		{ 18, 0, srcp::GA, srcp::GAServo , {19, 87, 93} },
                // Pin 20 + 21 I2C
		{ 19, 0, srcp::GA, srcp::GAServo , {22, 87, 93} },
		{ 20, 0, srcp::GA, srcp::GAServo , {23, 87, 93} },
		{ 21, 0, srcp::GA, srcp::GAServo , {24, 87, 93} },
		{ 22, 0, srcp::GA, srcp::GAServo , {25, 87, 93} },
		{ 23, 0, srcp::GA, srcp::GAServo , {26, 87, 93} },
		{ 24, 0, srcp::GA, srcp::GAServo , {27, 87, 93} },
		{ 25, 0, srcp::GA, srcp::GAServo , {28, 87, 93} },
		{ 26, 0, srcp::GA, srcp::GAServo , {29, 87, 93} },
		{ 27, 0, srcp::GA, srcp::GAServo , {30, 87, 93} },
		{ 28, 0, srcp::GA, srcp::GAServo , {31, 87, 93} },
		{ 29, 0, srcp::GA, srcp::GAServo , {32, 87, 93} },
		{ 30, 0, srcp::GA, srcp::GAServo , {33, 87, 93} },
		{ 31, 0, srcp::GA, srcp::GAServo , {34, 87, 93} },
		{ 32, 0, srcp::GA, srcp::GAServo , {35, 87, 93} },
		{ 33, 0, srcp::GA, srcp::GAServo , {36, 87, 93} },
		{ 34, 0, srcp::GA, srcp::GAServo , {37, 87, 93} },

		// Signale Pin 38 - 53
		{ 35, 0, srcp::GA, srcp::GASignal, {38, 39} },
		{ 36, 0, srcp::GA, srcp::GASignal, {40, 41} },
		{ 37, 0, srcp::GA, srcp::GASignal, {42, 43} },
		{ 38, 0, srcp::GA, srcp::GASignal, {44, 45} },
		{ 39, 0, srcp::GA, srcp::GASignal, {46, 47} },
		{ 40, 0, srcp::GA, srcp::GASignal, {48, 49} },
                // Pin 50 - 53 doppelt belegt - SPI Schnittstelle
		{ 41, 0, srcp::GA, srcp::GASignal, {50, 51} },
		{ 42, 0, srcp::GA, srcp::GASignal, {52, 53} },

		// lokale Adresse 1 bis 16, Rueckmelder an Pins A0 - A15
		{ 1, 8, srcp::FB, srcp::FBSwitchSensor, { 54, 61 } },
		{ 9, 16, srcp::FB, srcp::FBSwitchSensor, { 62, 69 } },
		// EOF Geraete - nicht vergessen!
		{ -1 },
};*/

void setup()
{
#if	( DEBUG_SCOPE > 0 )
	Serial.begin(19200);
#endif

	WireServer.addDeviceManager( new dev::CoreDeviceManager() );
	WireServer.begin( deviceConfig, srcp::BOARD_GA, VERSION, 11 );

#if	( DEBUG_SCOPE > 0 )
	Serial << "GA listen " <<  WireServer.getMyAddr() << endl;
#endif
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
