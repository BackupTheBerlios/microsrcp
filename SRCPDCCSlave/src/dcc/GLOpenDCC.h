/*
 * GLOpenDCC.h
 *
 *  Created on: 24.01.2010
 *      Author: mbern
 */

#ifndef GLOPENDCC_H_
#define GLOPENDCC_H_

#include <WProgram.h>
#include <srcp/SRCPGenericLoco.h>

namespace dcc
{

class GLOpenDCC : public srcp::SRCPGenericLoco
{
private:
	int startAddr;
	int endAddr;
public:
	GLOpenDCC( int startAddr, int endAddr, srcp::SRCPGenericLoco* next );
	int set( int addr, int drivemode, int v, int v_max, int fn[] );
	int checkAddr( int addr ) { return ( addr >= startAddr && addr <= endAddr); }
};

}

#endif /* GLOPENDCC_H_ */
