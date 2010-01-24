/*
 * GLOpenDCC.h
 *
 *  Created on: 24.01.2010
 *      Author: mbern
 */

#ifndef GLOPENDCC_H_
#define GLOPENDCC_H_

#include <srcp/SRCPGenericLoco.h>

namespace dcc
{

class GLOpenDCC : public srcp::SRCPGenericLoco
{
private:
	int startAddr;
	int endAddr;
public:
	GLOpenDCC( int startAddr, uint8_t endAddr, srcp::SRCPGenericLoco* next );
	int set( int addr, int drivemode, int v, int v_max, int fn[] );
	int checkAddr( int addr ) { return ( addr >= startAddr && addr <= endAddr); }
};

}

#endif /* GLOPENDCC_H_ */
