/*
 * essentials.h
 *
 *  Created on: Feb 13, 2014
 *      Author: Andrew
 */

#ifndef ESSENTIALS_H_
#define ESSENTIALS_H_

class AnonyChatPDU {
	unsigned opcode;
	long long unsigned src;
	long long unsigned dest;
	int utilCounter;
};

class NeighborRequestPDU : public AnonyChatPDU {
	int numNeighbors;
	std::string n_addr;
};

class NetworkMessagePDU : public AnonyChatPDU {
	unsigned size;
	std::string msg;
};


#endif /* ESSENTIALS_H_ */
