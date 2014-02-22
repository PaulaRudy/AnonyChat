/*
 * message.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: Sam Lalezari
 */

#include "../../include/message.h"

Message::Message(unsigned char source[constants::VID_SIZE], unsigned char dest[constants::VID_SIZE],
		bool flag, char m[constants::MAX_MESSAGE_SIZE]){

	memcpy(sourceVID, source, constants::VID_SIZE);
	memcpy(destVID, dest, constants::VID_SIZE);
	broadcastFlag = flag;
	memcpy(message, m, constants::VID_SIZE);
	sendTime = 0;
	utilityCounter = 0;
}

/**
 * @return The size of the message, including all of the headers.
 */
size_t Message::getMessageSize(){
	size_t size = 0;

	size += sizeof(sourceVID) + sizeof(destVID) + sizeof(utilityCounter) + sizeof(sendTime) + sizeof(broadcastFlag);
	size += strlen(message);

	return size;
}
