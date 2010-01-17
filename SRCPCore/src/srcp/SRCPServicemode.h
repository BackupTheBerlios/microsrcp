/*
	SRCPPower - Interace welches ein SRCP ServiceMode (CV Programmierung)
	Geraet implementieren muss.

	Siehe auch: http://srcpd.sourceforge.net/srcp/

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

#ifndef SRCPSERVICEMODE_H_
#define SRCPSERVICEMODE_H_

class SRCPServicemode
{
public:
	SRCPServicemode();
	int init( char* protocol );
	int set( int addr, char *type, char* options );
	int get( int addr, char* type, char* options );
};

#endif /* SRCPSERVICEMODE_H_ */
