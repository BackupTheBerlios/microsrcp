/*
	SRCPPower - Interace welches ein SRCP Power (Strom ein/aus) Geraet
	implementieren muss.

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

#ifndef SRCPPOWER_H_
#define SRCPPOWER_H_

namespace srcp
{
class SRCPPower
{
protected:
	SRCPPower *next;
public:
	SRCPPower* nextElement() { return ( next ); }
	virtual int init( void ) { return ( 423 ); }
	virtual int get( void ) { return ( 423 ); }
	virtual int set( char *power, char* options ) { return ( 423 ); }
	virtual int term( void ) { return ( 423 ); }
	virtual char* info( char* power ) { return ( (char*) 0 ); }
};
}

#endif /* SRCPPOWER_H_ */
