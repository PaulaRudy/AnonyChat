/*
 * chatHistory.h
 *
 *  Created on: Feb 15, 2014
 *      Author: Sam Lalezari
 */

#ifndef CHATHISTORY_H_
#define CHATHISTORY_H_

#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "message.h"

using namespace std;

class HistoryEntry{
private:
	string sourceName;
	unsigned char sourceVID[constants::VID_SIZE];
	char message[constants::MAX_MESSAGE_SIZE];
	int entryIndex = -1;
public:
	HistoryEntry(string sourceName, char message[constants::MAX_MESSAGE_SIZE]);
	HistoryEntry(unsigned char sourceVID[constants::VID_SIZE], char message[constants::MAX_MESSAGE_SIZE]);
	int getEntryIndex(){return entryIndex;};
	unsigned char* getSourceVID(){return sourceVID;};
	char* getMessage(){return message;};
	string getSourceName(){return sourceName;};
	int setEntryIndex(int i){entryIndex = i; return entryIndex;};
	void setSourceVID(unsigned char source[constants::VID_SIZE]){sourceVID = source;};
};

class HistoryLog{
private:
	vector<HistoryEntry> log;
	string sourceName;
	unsigned char sourceVID[constants::VID_SIZE];
public:
	HistoryLog(string name, unsigned char VID[constants::VID_SIZE]);
	string getSourceName(){return sourceName;};
	vector<HistoryEntry> getLog(){return log;};
	HistoryEntry getEntry(int i){return log.at(i);};
	unsigned char* getSourceVID(){return sourceVID;};
	void setSourceName(string name){sourceName = name;};
	int addEntry(HistoryEntry);
	int getLogSize(){return log.size();};
};

#endif /* CHATHISTORY_H_ */
