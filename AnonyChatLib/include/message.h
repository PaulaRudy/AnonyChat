/*
 * message.h
 *
 *  Created on: Feb 15, 2014
 *      Author: Sam Lalezari
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "config.h"

class Message {
private:
	unsigned char sourceVID[constants::VID_SIZE];
	unsigned char destVID[constants::VID_SIZE];
	char message[constants::MAX_MESSAGE_SIZE];
	int utilityCounter;
public:
	Message(unsigned char source[constants::VID_SIZE], unsigned char dest[constants::VID_SIZE], char m[constants::MAX_MESSAGE_SIZE]);
};




#endif /* MESSAGE_H_ */
