/*
 SRCPGLSlave - I2C Board um 2 analoge Stromkreise zu regeln.
 
 Bestehend aus:
 - Arduino Board mit min. ATmega168p - http://www.arduino.cc
 - Motor Shield - http://arduino.cc/en/Main/ArduinoMotorShield
 
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
#include <I2CServer.h>
#include <SRCPDeviceMaster.h>
#include <CoreDeviceManager.h>

// Meine I2C Adresse
#define MY_ADDR	100

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
  // Devicemanager fuer lokale Geraete (Sensoren, Servos etc.) initialisieren
  WireServer.addDeviceManager( new dev::CoreDeviceManager() );
  // I2C Server starten
  WireServer.begin( MY_ADDR, deviceConfig );
}

void loop()
{
  // Refresh der lokalen Rueckmelder
  WireServer.run();
  // hier koennte noch mehr stehen
  delay( 100 );
}

