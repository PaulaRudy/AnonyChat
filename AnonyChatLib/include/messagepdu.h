/*
 * messagepdu.h
 *
 * TODO: Needs a description. Needs comments.
 * TODO: Should we integrate this and message.h?
 *
 *  Created on: Feb 13, 2014
 *      Authors:
 *      	Andrew
 *      	Paula Rudy (paular@wpi.edu)
 */

#ifndef ESSENTIALS_H_
#define ESSENTIALS_H_

#include <string>

class AnonyChatPDU {
public:
	unsigned opcode;
	long long unsigned src;
	long long unsigned dest;
	int utilCounter;
};

class NeighborRequestPDU : public AnonyChatPDU {
public:
	int numNeighbors;
	char *n_addr;
};

class NetworkMessagePDU : public AnonyChatPDU {
public:
	unsigned size;
	char *msg;
};


#endif /* ESSENTIALS_H_ */
