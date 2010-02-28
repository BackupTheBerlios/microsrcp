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


#include "WaveDeviceManager.h"
#include "GAWave.h"

AF_Wave card;
File sound;
Wavefile wave; // only one!

namespace wav
{

// Preinstantiate Objects //////////////////////////////////////////////////////

char* WaveDeviceManager::nextSound = 0;

WaveDeviceManager::WaveDeviceManager()
{
	next = 0;

	pinMode( 2, OUTPUT );
	pinMode( 3, OUTPUT );
	pinMode( 4, OUTPUT );
	pinMode( 5, OUTPUT );
	pinMode( 9, OUTPUT );

	if ( !card.init_card() )
	{
#if	( DEBUG_SCOPE > 0 )
		Serial.println( "Card init. failed!" );
#endif
		return;
	}
	if ( !card.open_partition() )
	{
#if	( DEBUG_SCOPE > 0 )
		Serial.println( "No partition!" );
#endif
		return;
	}
	if ( !card.open_filesys() )
	{
#if	( DEBUG_SCOPE > 0 )
		Serial.println( "Couldn't open filesys" );
#endif
		return;
	}

	if ( !card.open_rootdir() )
	{
#if	( DEBUG_SCOPE > 0 )
		Serial.println( "Couldn't open dir" );
#endif
		return;
	}

#if	( DEBUG_SCOPE > 0 )
	Serial << "init card i.o" << endl;

	ls();
#endif

}

srcp::SRCPGenericAccessoire* WaveDeviceManager::createGA( srcp::device_config_t config, srcp::SRCPGenericAccessoire* next )
{
	switch ( config.subDevice )
	{
		case srcp::GAWave:
			char f[5];
			f[0] = config.args[0];
			f[1] = config.args[1];
			f[2] = config.args[2];
			f[3] = config.args[3];
			f[4] = '\0';
			return	( new GAWave( config.start_addr, f, next ) );

		default:
			break;
	}
	return	( 0 );
}

void WaveDeviceManager::ls()
{
#if	( DEBUG_SCOPE > 0 )
	char name[13];
	int ret;

	card.reset_dir();
	Serial.println( "Files found:" );
	while ( 1 )
	{
		ret = card.get_next_name_in_dir( name );
		if ( !ret )
		{
			card.reset_dir();
			return;
		}
		Serial.println( name );
	}
#endif
}

void WaveDeviceManager::playfile( char *name )
{
	sound = card.open_file( name );
	if ( !sound )
	{
#if	( DEBUG_SCOPE > 0 )
		Serial.println( " Couldn't open file" );
#endif
		return;
	}
	if ( !wave.create( sound ) )
	{
#if	( DEBUG_SCOPE > 0 )
		Serial.println( " Not a valid WAV" );
#endif
		return;
	}
	// ok time to play!
	wave.play();
}

void WaveDeviceManager::playcomplete( char *name )
{
	card.reset_dir();
	playfile( name );

	while ( wave.isplaying )
		delay( 500 );

	card.close_file( sound );
}

void WaveDeviceManager::play()
{
	if	( nextSound != 0 )
	{
		char* n = nextSound;
		nextSound = 0;
		playcomplete( n );
	}
}

}
