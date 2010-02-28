/*
	WaveDeviceManager - Erzeugt die Wave Geraete und stellt
	allgemeine Funktionen zum abspielen von Sounds zur
	Verfuegung.

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

#ifndef WAVEDEVICEMANAGER_H_
#define WAVEDEVICEMANAGER_H_

#include <AF_Wave.h>
#include <wave.h>

#include <../srcp/SRCPCommand.h>
#include <../srcp/SRCPDeviceManager.h>
#include <../srcp/SRCPGenericLoco.h>
#include <../srcp/SRCPGenericAccessoire.h>

extern AF_Wave card;
extern File sound;
extern Wavefile wave; // only one!

namespace wav
{

class WaveDeviceManager : public srcp::SRCPDeviceManager
{
private:
	static char* nextSound;
	static void playcomplete(char *name);
	static void playfile(char *name);
	static void ls();
public:
	WaveDeviceManager();
	srcp::SRCPGenericAccessoire* createGA( srcp::device_config_t config, srcp::SRCPGenericAccessoire* next );
	static void play();
	static void setSound( char* next ) { nextSound = next; }
};

}

#endif /* WAVEDEVICEMANAGER_H_ */
