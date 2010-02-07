/*
	WiFiPrint - Hilfsklasse um Daten via TCP/IP zu senden.

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

#ifndef WIFIPRINT_H_
#define WIFIPRINT_H_


#include <Print.h>

namespace lan
{

class WiFiPrint : public Print
{
public:
    virtual void write(uint8_t) {};
    void write(const char *str);
    void write(const uint8_t *buffer, size_t size);
};

}

#endif /* WIFIPRINT_H_ */
