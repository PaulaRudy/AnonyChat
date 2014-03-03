/*
 * message.h
 *
 *  Created on: Feb 15, 2014
 *      Author: Sam Lalezari
 *      		Andrew McKay
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "config.h"
#include <time.h>

// messages between neighbors shall be sent to port 9191
#define MESSAGEPORT 9191

class Message {
private:
	char messageFlag;
	long long messageSize;
	unsigned char sourceVID[constants::VID_SIZE];
	unsigned char destVID[constants::VID_SIZE];
	int utilityCounter;
	time_t sendTime;
	char message[constants::MAX_MESSAGE_SIZE];

public:
	Message(unsigned char source[constants::VID_SIZE], unsigned char dest[constants::VID_SIZE], char flag, char m[constants::MAX_MESSAGE_SIZE]);
	Message(char m[constants::MAX_MESSAGE_SIZE]){memcpy(message, m, constants::MAX_MESSAGE_SIZE);};
	size_t getMessageSize();
	long long unsigned getVID(bool);
	int getUCounters();
	void setUCounters(int);
};




#endif /* MESSAGE_H_ */
