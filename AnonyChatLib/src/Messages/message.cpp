/*
 * message.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: Sam Lalezari
 *      		Andrew McKay
 */

#include "../../include/message.h"

Message::Message(unsigned char source[constants::VID_SIZE], unsigned char dest[constants::VID_SIZE],
		char flag, char m[constants::MAX_MESSAGE_SIZE]){

	memcpy(sourceVID, source, constants::VID_SIZE);
	memcpy(destVID, dest, constants::VID_SIZE);
	messageFlag = flag;
	memcpy(message, m, constants::VID_SIZE);
	sendTime = 0;
	utilityCounter = 0;
	messageSize += sizeof(messageFlag) + sizeof(messageSize) + sizeof(sourceVID) + sizeof(destVID) + sizeof(utilityCounter) + sizeof(sendTime);
	messageSize += strlen(message);
}

/**
 * @return The size of the message, including all of the headers.
 */
size_t Message::getMessageSize(){
		return messageSize;
}

/**
 * @return The source or destination virtual address as indicated by the bool argument
 * true for source false for dest
 */
unsigned char* Message::getVID(bool wantSource) {
	if (wantSource)	return sourceVID;
	else return destVID;
}

/**
 * @return The number of utility counters associated with this message
 */
int Message::getUCounters() {
	return utilityCounter;
}

void Message::setUCounters(int UCval) {
	utilityCounter = UCval;
	return;
}

/*
 *  Sends the given message using the users routing table and given neighbor information.
 */
void sendMessage(Message m, char* neighborRecievedFrom){
	time_t setTime;
	time(&setTime);
	m.setTime(setTime);

//	getRoutingTable().RouteMessage(m, neighborRecievedFrom);
}


/**
 * Opens the message, and updates the users contact list with it.
 *
 * @param m The message to open.
 * @param l The contact list to update.
 */
void openMessage(Message m){
	///getContactList().update(m); // Update the contact list with the message
}

void recieveMessage(Message m, char* neighborRecievedFrom){

	/** Update the routing table entry for the source VID and the neighbor the message was recieved from */
//	NeighborUtilCountPair neighborUtil = NeighborUtilCountPair(neighborRecievedFrom, m.getUCounters());
//	getRoutingTable().UpdateTableEntry(m.getVID(true), neighborUtil);

	/** Check to see if the message is destined for the user. If it is, then proceed to open the message */
//	if(memcmp(m.getVID(false), getUserVID(), constants::VID_SIZE)){
//		openMessage(m, getContactList());
//	}

	/** Message was not destined for user, so continue with forwarding it **/

//	sendMessage(getRoutingTable(), m, neighborRecievedFrom);



}
