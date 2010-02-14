/*
	SRCPWaveSlave - Spielt 16 bit Mono Dateien mit 22Khz ab.

	Bestehend aus:
	- Arduino Board mit min. ATmega168p - http://www.arduino.cc
	- Wave Shield - http://www.ladyada.net/make/waveshield/

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

#include <WProgram.h>
#include <AF_Wave.h>
#include "wave.h"

#include "srcp/SRCPDeviceMaster.h"
#include "wav/WaveDeviceManager.h"
#include "i2c/I2CServer.h"

#define VERSION 10	// Version 1.0

// Definition der lokalen Geraete
srcp::device_config_t deviceConfig[] =
	{
		// Pseudo lokale Adressen, ensprechen den Dateien welche abgespielt werden sollen
		{ 1, 0, srcp::GA, srcp::GAWave , {'R', 'E', 'G', '1'} },	// Dateiname max. 4 Zeichen ohne Erweiterung .WAV
		{ 2, 0, srcp::GA, srcp::GAWave , {'R', 'E', 'G', '2'} },	// ACHTUNG: nur Grossbuchstaben verwenden!
		{ 3, 0, srcp::GA, srcp::GAWave , {'G', 'L', 'A', '1'} },
		{ 4, 0, srcp::GA, srcp::GAWave , {'G', 'L', 'A', '2'} },

		// EOF Geraete - nicht vergessen!
		{ -1 },
};

void setup()
{
	Serial.begin( 19200 ); // set up Serial library at 9600 bps

	WireServer.addDeviceManager( new wav::WaveDeviceManager() );
	WireServer.begin( deviceConfig, srcp::BOARD_WAVE, VERSION );

	Serial << "Wave listen " <<  WireServer.getMyAddr() << endl;
}

void loop()
{
	// Refresh Rueckmelder o.ae.
	WireServer.run();

	// Sound abspielen, welche mit setSound eingetragen wurden.
	wav::WaveDeviceManager::play();

	delay( 100 );
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

