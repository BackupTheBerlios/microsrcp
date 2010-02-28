/*
	SRCPDeviceMaster - Verwaltet eine verkette Liste von
	SRCPDeviceManagern. Wurde erstellt um Speicher zu sparen.
	Jeder SRCP- oder I2C-Server braucht einen DeviceMaster.

	Funktionsweise:
	- Im Hauptsketch werden dem DeviceMaster - DeviceManager
	  fuer die Erstellung von Geraeten uebergeben.
	- Mittels der beim init uebergebenen Geraete Konfiguration
	  werden diese erstellt, dazu werden nacheinnander die
	  DeviceManager aufgerufen, solange bis ein entsprechendes
	  Geraet gefunden wird.

	Um z.B. weitere Geraete zu definieren sind Geraete-
	Treiber (siehe dev/..) und ein DeviceManager zu schreiben.
	Um die Geraete zu verwenden, sind diese in deviceConfig zu
	definieren und der neu erstellte DeviceManager dem
	DeviceMaster zu uebergeben.

	Beispiel siehe SRCPGASlave, SRCPGLSlave, SRCPServer.

	--------------------------------------------------------------------------
	EEPROM Aufbau
	--------------------------------------------------------------------------
	! CV ! Name			! Beschreibung						! Bereich ! Wert !
	! 1	 ! I2C Adresse	! Adresse im I2C Bus				! 0 - 127 !    3 !
	! 2	 ! Board-Id		! Identifiziert das Board 1)	    ! 0 - 127 !    0 !
	!	 !				!   0 = Zentrale					!		  !		 !
	!	 !				!   3 = GA Board					!		  !		 !
	!	 !				!  90 = Wave Board					!		  !		 !
	!	 !				!  99 = GL DCC Board				!		  !		 !
	!	 !				! 100 = GL Analog Board				!		  !		 !
	! 7	 ! Versionsnr.	! Versionsnummer 1)					!		  !		 !
	! 8	 ! Hersteller	! Hersteller-Kennung 2)				!		  !  123 !
	--------------------------------------------------------------------------
	! 12 ! Geraettyp	! FB = 1 , GA = 2, GL = 3			!		  !		 !
	! 13 ! Geraet		! siehe unten						!		  !		 !
	! 14 !				! von Adresse 3)					!		  !		 !
	! 15 !				!									!		  !		 !
	! 16 !				! bis Adresse, 0 wenn nur 1 Geraet	!		  !		 !
	! 17 !				! 3)								!		  !		 !
	! 18 !          	! Optionaler Parameter, siehe Geraet!		  !		 !
	! 19 !       		! "									!		  !		 !
	! 20 !          	! "									!		  !		 !
	! 21 !       		! "									!		  !		 !
	! 22 !          	! Reserve, speichert z.B. den 		!		  !		 !
	! 23 !       		! Zustand eines Geraetes			!		  !		 !
	--------------------------------------------------------------------------
	! 24 ! von     		! Parameter naechstes Geraet		!		  !		 !
	! 35 ! bis    		!                         			!		  !		 !
	--------------------------------------------------------------------------
	! 36 ! von     		! etc.								!		  !		 !
	! 47 ! bis    		!                         			!		  !		 !
	--------------------------------------------------------------------------

    1) Stimmt Board-Id oder Versionsnummer nicht mit dem EEPROM ueberein, wird
       dieses auf die Defaultwerte zurueckgesetzt. Damit wird verhindert, dass
       beim Neuprogrammieren des Boardes falsche Werte im EEPROM stehen.
    2) Wird die Hersteller-Kennung auf 8 gesetzt werden die EEPROM-Werte
       beim naechsten Einschalten oder Reset zurueckgesetzt (Standard DCC).
       Ein Wert 255 fuehrt einen Software-Reset des Board durch, ohne die
       gespeicherten Werte zu veraendern.
    3) Adresse besteht aus hoeherwertigen Byte (v/256) und niederwertigem
       Byte (v%256).

	FB - Feedback: Sensoren, Rueckmelder
	--------------------------
	0 = Sensor an externen I2C Board
	1 = reserviert
	2 = einfacher Sensor, z.B. Schutzgasrohrkontakt

	GA - Generic Accessoire: Servo, Signal etc.
	-------------------------------------------
	0 = Adressen sind einem externen I2C Board zugeordnet
	1 = DCC Board (Booster)
	2 = Signal
	3 = Servo
	4 = Wave Shield

	GL - Generic Loco: Lokomotive, Motorentreiber
	---------------------------------------------
	0 = Adressen sind einem externen I2C Board zugeordnet
	1 = DCC Board (Booster)
	2 = Motor an PWM Pin, z.B. gesteuert via Transistor
	3 = AF_Motor Shield
	4 = Arduino Motor Shield zur Steuerung analoger Lokomotiven

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

#include "SRCPDeviceMaster.h"
#include "../i2c/I2CUtil.h"

namespace srcp
{

void SRCPDeviceMaster::init( device_config_t deviceConfig[], int id, int version )
{
	firstGA = 0;
	firstGL = 0;
	firstFB = 0;
	SRCPDeviceManager* manager;

	// Persistenter Speicher (EEPROM) initialisieren, damit richtige Werte vorhanden sind.
	Storage.init( deviceConfig, id, version );

	for ( int i = 1; ; i++ )
	{
		device_config_t device = Storage.getConfig( i );
		// EOF
		if	( device.start_addr == -1 )
			break;

#if	( DEBUG_SCOPE > 0 )
		Serial << "EEPROM pos: " << i * (sizeof(srcp::device_config_t) + 2) << ", dev: " << device.device << ":"
			   << (int) device.subDevice << ", addr " << device.start_addr << " - " << device.end_addr << ", args: ";
		for	( unsigned int i = 0; i < sizeof(device.args); i++ )
			Serial << (int) device.args[i] << " ";
		Serial << endl;
#endif

		switch ( device.device )
		{
			case GA:
				for	( manager = firstManager; manager != 0; manager = manager->getNextManager() )
				{
					SRCPGenericAccessoire* nga = manager->createGA( device, firstGA );
					if	( nga != 0 )
					{
						firstGA = nga;
						break;
					}
				}
				break;
			case GL:
				for	( manager = firstManager; manager != 0; manager = manager->getNextManager() )
				{
					SRCPGenericLoco* ngl = manager->createGL( device, firstGL );
					if ( ngl != 0 )
					{
						firstGL = ngl;
						break;
					}
				}
				break;
			case FB:
				for	( manager = firstManager; manager != 0; manager = manager->getNextManager() )
				{
					SRCPFeedback* nfb = manager->createFB( device, firstFB );
					if ( nfb != 0 )
					{
						firstFB = nfb;
						break;
					}
				}
				break;
			case	LAN:					// Device im Netzwerk (I2C, ...) erstellt lokalen Proxy
				for	( manager = firstManager; manager != 0; manager = manager->getNextManager() )
					if	(  manager->createProxy( device, &firstGA, &firstGL, &firstFB ) )
						break;
				break;
			case	NA:
				break;
		}
	}
}

int SRCPDeviceMaster::setGA( int addr, int port, int value, int delay )
{
	int rc = 200;
	Enumeration<SRCPGenericAccessoire> ga( addr, firstGAElement() );
	while	( ga.hasMoreElements() )
	{
		SRCPGenericAccessoire* pa = ga.nextElement();
		rc = pa->set( addr, port, value, delay );
	}
	return ( rc );

}

int SRCPDeviceMaster::setGL( int addr, int drivemode, int v, int v_max, int fn[] )
{
	int rc = 200;
	Enumeration<SRCPGenericLoco> gl( addr, firstGLElement() );
	while	( gl.hasMoreElements() )
	{
		SRCPGenericLoco* pl = gl.nextElement();
		rc = pl->set( addr, drivemode, v, v_max, fn );
	}
	return	( rc );
}

void SRCPDeviceMaster::refresh()
{
	for	( SRCPFeedback* n = firstFeedbackElement(); n != 0; n = n->nextElement() )
		n->refresh();
}

int SRCPDeviceMaster::getFB( int addr )
{
	// TODO mehr als 1 Element abhandeln
	Enumeration<SRCPFeedback> e( addr, firstFeedbackElement() );
	if	( e.hasMoreElements() )
	{
		SRCPFeedback* f = e.nextElement();
		return	( f->get( addr ));
	}
	return	( 0 );
}

void SRCPDeviceMaster::setPower( int on )
{
	for	( SRCPGenericAccessoire* n = firstGAElement(); n != 0; n = n->nextElement() )
		n->setPower( on );

	for	( SRCPGenericLoco* n = firstGLElement(); n != 0; n = n->nextElement() )
		n->setPower( on );
}

int SRCPDeviceMaster::setSM( int bus, int addr, int device, int cv, int value )
{
	// Zentrale?
	if	( device == CV && addr == 0 && bus == 0 )
	{
		// 8,255 = Reset Chip, Werte im EEPROM bleiben erhalten
		if	( cv == CV_PRODUCER && value == 255 )
			asm volatile ("jmp 0x0000");

		Storage.write( cv, value );
		return	( 200 );
	}

	// CV's eines I2C Boardes setzen
	if	( device == CV && bus == 0 )
	{
		// TODO besser waere keine direktes Ansprechen des I2C Buses
		i2c::I2CUtil::setSM( addr, bus, 0, device, cv, value );
		return	( 200 );
	}

	// SM Parameter lokaler Boards setzen
	for	( SRCPGenericAccessoire* n = firstGAElement(); n != 0; n = n->nextElement() )
		n->setSM( bus, addr, device, cv, value );

	for	( SRCPGenericLoco* n = firstGLElement(); n != 0; n = n->nextElement() )
		n->setSM( bus, addr, device, cv, value );

	return	( 200 );
}

int SRCPDeviceMaster::getSM( int bus, int addr, int device, int cv )
{
	// Zentrale?
	if	( device == CV && addr == 0 && bus == 0 )
		return	( Storage.read( cv ) );

	// CV's eines I2C Boardes lesen
	if	( device == CV && bus == 0 )
		// besser waere keine direktes Ansprechen des I2C Buses
		return	( i2c::I2CUtil::getSM( addr, bus, 0, device, cv ) );

	int rc = -1;
	for	( SRCPGenericAccessoire* n = firstGAElement(); n != 0; n = n->nextElement() )
		if	( (rc = n->getSM( bus, addr, device, cv ) ) != -1 )
			return	( rc );

	for	( SRCPGenericLoco* n = firstGLElement(); n != 0; n = n->nextElement() )
		if	( (rc = n->getSM( bus, addr, device, cv ) ) != -1 )
			return	( rc );

	return	( -1 );
}

int SRCPDeviceMaster::getDescription( int bus, int addr, int device, int rc[] )
{
	if	( addr == 0 && bus == 0 && device == LAN  )
	{
		rc[0] = 0;
		rc[1] = 0;
		for	( SRCPFeedback* n = firstFeedbackElement(); n != 0; n = n->nextElement() )
		{
			// das 1. Mal
			if	( rc[0] == 0 )
			{
				rc[0] = n->getStartAddr();
				rc[1] = n->getEndAddr();
				continue;
			}
			rc[0] = (n->getStartAddr() < rc[0]) ? n->getStartAddr() : rc[0];
			rc[1] = (n->getEndAddr()   > rc[1]) ? n->getEndAddr()   : rc[1];
		}
		rc[2] = 0;
		rc[3] = 0;
		for	( SRCPGenericAccessoire* n = firstGAElement(); n != 0; n = n->nextElement() )
		{
			// das 1. Mal
			if	( rc[2] == 0 )
			{
				rc[2] = n->getStartAddr();
				rc[3] = n->getEndAddr();
				continue;
			}
			rc[2] = (n->getStartAddr() < rc[2]) ? n->getStartAddr() : rc[2];
			rc[3] = (n->getEndAddr()   > rc[3]) ? n->getEndAddr()   : rc[3];
		}
		rc[4] = 0;
		rc[5] = 0;
		for	( SRCPGenericLoco* n = firstGLElement(); n != 0; n = n->nextElement() )
		{
			// das 1. Mal
			if	( rc[4] == 0 )
			{
				rc[4] = n->getStartAddr();
				rc[5] = n->getEndAddr();
				continue;
			}
			rc[4] = (n->getStartAddr() < rc[4]) ? n->getStartAddr() : rc[4];
			rc[5] = (n->getEndAddr()   > rc[5]) ? n->getEndAddr()   : rc[5];
		}
	}
	// 3 x 2 Integer
	return	( 12 );
}

SRCPDeviceMaster::~SRCPDeviceMaster()
{
	// TODO Speicher aufraeumen
}

}
