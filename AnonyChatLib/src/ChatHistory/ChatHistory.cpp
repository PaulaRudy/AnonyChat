/*
 * ChatHistory.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Sam Lalezari
 */

#include "chatHistory.h"

/**
 * Constructor for a History Entry
 * @param source The source name that the new history entry will be assigned to.
 * @param m The message to add to the history entry.
 */
HistoryEntry::HistoryEntry(string source, char m[constants::MAX_MESSAGE_SIZE]){
	sourceName = source;
	message = m;
}

/**
 * Constructor for a History Entry
 * @param source The source VID that the new history entry will be assigned to.
 * @param m The message to add to the history entry.
 */
HistoryEntry::HistoryEntry(unsigned char source[constants::VID_SIZE], char m[constants::MAX_MESSAGE_SIZE]){
	sourceVID = source;
	message = m;
}

/**
 * Constructor for a History Log
 * @param name The source name that the new history log will be assigned to.
 * @param VID The most recent VID to be associated with this History Log.
 */
HistoryLog::HistoryLog(string name, unsigned char VID[constants::VID_SIZE]){
	sourceName = name;
	sourceVID = VID;
}

/**
 *  Adds an entry to the History Log, and returns the index of the new entry.
 *  @param entry The HistoryEntry to add to the log.
 *  @return The index of the new entry.
 */
int HistoryLog::addEntry(HistoryEntry entry){
	int index = log.size();
	entry.setEntryIndex(index);

	log.push_back(entry);

	return index;
}
