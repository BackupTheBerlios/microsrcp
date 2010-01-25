/*
 * GAOpenDCC.h
 *
 *  Created on: 24.01.2010
 *      Author: mbern
 */

#ifndef GAOPENDCC_H_
#define GAOPENDCC_H_

#include <srcp/SRCPGenericAccessoire.h>

namespace dcc
{

class GAOpenDCC : public srcp::SRCPGenericAccessoire
{
private:
	int startAddr;
	int endAddr;
public:
	GAOpenDCC( int startAddr, int endAddr, srcp::SRCPGenericAccessoire* next );
	int set( int addr, int port, int value, int delay );
	int checkAddr( int addr ) { return ( addr >= startAddr && addr <= endAddr); }
};

}

#endif /* GAOPENDCC_H_ */
