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

#include "SRCPDeviceMaster.h"

namespace srcp
{

void SRCPDeviceMaster::init( device_config_t deviceConfig[] )
{
	firstGA = 0;
	firstGL = 0;
	firstPower = 0;
	firstFB = 0;
	SRCPDeviceManager* manager;

	for ( int i = 0; deviceConfig[i].start_addr != -1; i++ )
	{
		switch ( deviceConfig[i].device )
		{
			case GA:
				for	( manager = firstManager; manager != 0; manager = manager->getNextManager() )
				{
					SRCPGenericAccessoire* nga = manager->createGA( deviceConfig[i], firstGA );
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
					SRCPGenericLoco* ngl = manager->createGL( deviceConfig[i], firstGL );
					if ( ngl != 0 )
					{
						firstGL = ngl;
						break;
					}
				}
				break;
			case POWER:
				for	( manager = firstManager; manager != 0; manager = manager->getNextManager() )
				{
					SRCPPower* npower = manager->createPower( deviceConfig[i], firstPower );
					if ( npower != 0 )
					{
						firstPower = npower;
						break;
					}
				}
				break;
			case FB:
				for	( manager = firstManager; manager != 0; manager = manager->getNextManager() )
				{
					SRCPFeedback* nfb = manager->createFB( deviceConfig[i], firstFB );
					if ( nfb != 0 )
					{
						firstFB = nfb;
						break;
					}
				}
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

SRCPDeviceMaster::~SRCPDeviceMaster()
{
	// TODO Speicher aufraeumen
}

}
