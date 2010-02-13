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

#ifndef SRCPDEVICEMASTER_H_
#define SRCPDEVICEMASTER_H_

#include "SRCPCommand.h"
#include "SRCPDeviceManager.h"

namespace srcp
{
template<class T>
class Enumeration
{
private:
	int addr;
	T* ptr;
public:
	Enumeration( int addr, T* ptr ) { this->addr = addr; this->ptr = ptr; }
	int hasMoreElements()
	{
		// leere Pointerliste oder bereits durchlaufen
		if	( ptr == 0 )
			return	( 0 );
		if	( ptr->checkAddr( addr ) )
			return	( 1 );

		while	( (ptr = ptr->nextElement()) != 0 )
		{
			if	( ptr->checkAddr( addr ) )
				return	( 1 );
		}
		return	( 0 );
	}
	T* nextElement() { T* next = ptr; ptr = ptr->nextElement(); return( next ); }
};

class SRCPDeviceMaster
{
private:
	SRCPDeviceManager *firstManager;
	SRCPGenericAccessoire *firstGA;
	SRCPGenericLoco *firstGL;
	SRCPFeedback *firstFB;
	device_config_t deviceConfig[];
public:
	SRCPDeviceMaster() { firstManager = 0; }
	virtual ~SRCPDeviceMaster();
	/** um alle Element zu transfersieren ohne Eingrenzung, anschliessend mit ga->next() weiterlesen */
	SRCPGenericAccessoire* firstGAElement() { return( firstGA ); }
	SRCPGenericLoco* firstGLElement() { return( firstGL ); }
	SRCPFeedback* firstFeedbackElement() { return( firstFB ); }
	void addDeviceManager( SRCPDeviceManager *manager ) { manager->setNextManager( firstManager ); firstManager = manager; }
	void init( device_config_t deviceConfig[] );
	int setGA( int addr, int port, int value, int delay );
	int setGL( int addr, int drivemode, int v, int v_max, int fn[] );
	void refresh();
	int getFB( int addr );
	void setPower( int on );
	int setSM( int bus, int addr, int cv, int value );
	int getSM( int bus, int addr, int cv );
};

}

#endif /* SRCPDEVICEMASTER_H_ */
