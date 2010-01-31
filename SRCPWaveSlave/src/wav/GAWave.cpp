/*
	GAWave - Abspielen einer Wave Datei.

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

#include "GAWave.h"
#include "WaveDeviceManager.h"

namespace wav
{

GAWave::GAWave( int addr, char* file, srcp::SRCPGenericAccessoire* next)
{
	this->addr = addr;
	this->next = next;

	strcpy( this->file, file );
	strcat( this->file, ".WAV" );
}

int GAWave::set( int addr, int port, int value, int delay )
{
	//Serial << "play " << file << endl;

	// nur Namen setzen, noch nicht abspielen, weil in Interrupt-Routine!
	WaveDeviceManager::setSound( file );
	return	( 200 );
}

}