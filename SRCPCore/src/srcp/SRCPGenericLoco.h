/*
	SRCPFeedback - Interface wo alle Funktionen definiert die
	ein Generic Loco (Motorentreiber ...) Geraet
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

#ifndef SRCPGENERICLOCO_H_
#define SRCPGENERICLOCO_H_

#include <inttypes.h>

namespace srcp
{
class SRCPGenericLoco
{
protected:
	int addr;
	SRCPGenericLoco *next;
public:
	SRCPGenericLoco* nextElement() { return ( next ); }
	void setNextElement( SRCPGenericLoco* next ) { this->next = next; }
	virtual int checkAddr( int addr ) { return( this->addr == addr ); }
	virtual int get( int addr, int port ) { return ( 423 ); }
	virtual int set( int addr, int drivemode, int v, int v_max, int fn[] ) { return ( 423 ); }
	virtual void setPower( int on ) {}
	virtual int setSM( int bus, int addr, int device, int cv, int value ) { return( 200 ); }
	virtual int getSM( int bus, int addr, int device, int cv ) { return( -1 ); };
	virtual int	getStartAddr() { return( this->addr ); };
	virtual int getEndAddr() { return( this->addr ); }
	virtual int getAddr() { return( this->addr ); }
};
}

#endif /* SRCPGENERICLOCO_H_ */
