/*
 * ChatHistory.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Sam Lalezari
 */

#include "../../include/chatHistory.h"

/**
 * Constructor for a History Entry
 * @param source The source name that the new history entry will be assigned to.
 * @param m The message to add to the history entry.
 */
HistoryEntry::HistoryEntry(string source, char m[constants::MAX_MESSAGE_SIZE]){
	sourceName = source;
	memcpy(message, m, constants::MAX_MESSAGE_SIZE);
	entryIndex = -1;
	time(&entryTime);
}

/**
 * Constructor for a History Entry
 * @param source The source VID that the new history entry will be assigned to.
 * @param m The message to add to the history entry.
 */
HistoryEntry::HistoryEntry(unsigned char source[constants::VID_SIZE], char m[constants::MAX_MESSAGE_SIZE]){
	memcpy(sourceVID, source, constants::VID_SIZE);
	memcpy(message, m, constants::MAX_MESSAGE_SIZE);
	entryIndex = -1;
	time(&entryTime);

}

/**
 * @return A string that is human readable, and also parsable by the readHistoryLog() function.
 *		   The output will look like the following:
 *		   ========================================
 *		   Index;Time; SourceName\n
 *		   VID\n
 *		   Message\n
 *
 *		   ========================================
 *
 *		   For example, a message sent by Andrew with
 *		   an index of 0 will look like:
 *		   ========================================
 *		   0;1393476530; Andrew
 *		   8435714272899213895181290124015812923481
 *		   This is a message that has been sent from Andrew.
 *
 *		   ========================================
 *
 * */
string HistoryEntry::toString(){

	stringstream ss;
	ss << entryIndex << ";" << entryTime << ";" + sourceName + "\n";
	ss << sourceVID << "\n";
	ss << message << "\n";

	string returnString = ss.str();

	return returnString;
}

/**
 * Constructor for a History Log
 * @param name The source name that the new history log will be assigned to.
 * @param VID The most recent VID to be associated with this History Log.
 */
HistoryLog::HistoryLog(string name, unsigned char VID[constants::VID_SIZE]){
	sourceName = name;
	memcpy(sourceVID, VID, constants::VID_SIZE);
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

/**
 * Writes the history log to a given ofstream
 *
 * The first two lines contain the source name and the source vid.
 * Two end line characters delimite two HistoryEntrys from each other. The
 * HistoryEntry is formatted the same way as in HistoryEntry::toString().
 *  -----------------------
 *  SourceName\n
 *  SourceVID\n
 *  \n
 *  entryIndex; entryTime; Name\n
 *  VID\n
 *  Message\n
 *
 *  -----------------------
 *
 * For example, if there was a history log with two entries from Sue
 *  with VID of 1. The output will look like this:
 *
 *  -----------------------
 *  Sue
 *  1
 *
 *  0;1393476530; Sue
 *  1
 *  This is a message from Sue.
 *
 *  1;1393476532; Sue
 *  1
 *  This is a message from Sue.
 *
 *  -----------------------
 *
 * @param s the stream to write the file to
 *
 */
void HistoryLog::write(ofstream& s){
	s << sourceName + "\n";
	s << sourceVID << "\n\n";

	for(unsigned int i = 0; i < log.size(); i++){
		HistoryEntry e = log.at(i);
		s << e.toString() + "\n";
	}
}

/**
 * Prints the history log to the standard output. It is formatted the
 *  same way has HistoryLog::write().
 */
void HistoryLog::print(){
	cout << "Chat log with " + sourceName + "\n";
	cout << sourceVID << "\n\n";

	for(unsigned int i = 0; i < log.size(); i++){
		HistoryEntry e = log.at(i);
		cout << e.toString().c_str() << "\n";
	}
}

/**
 * Reads a history log from a file that was created using HistoryLog::write().
 * Constructs a log from the contents of the file and then returns it.
 *
 * @param path The path the the file to read from.
 * @return A HistoryLog constructed from the given file.
 */
HistoryLog readHistoryLog(string path){
	ifstream infile(path.c_str(), ifstream::in);
	HistoryLog log;

	/* Place holders for the parts of a history entry */
	string name;
	string index;
	string VID;
	string message;
	string timestring;

	getline(infile, name); // Get's the source name at the beginning of the file.
	getline(infile, VID);  // Get's the source VID  at the beginning of the file.

	log = HistoryLog(name, (unsigned char*)VID.c_str()); // Creates a new log using the name and VID at the top of the file.

	getline(infile, name); //Read the blank line, but does nothing with name.
	name.clear(); // Clears the blank line from name
	VID.clear(); // Clears the VID

	while(infile.peek() != EOF){
		getline(infile, index, ';'); // Not really used, because the functions is assuming that the indexes
									 // of entries are the same as the order they are in.
		getline(infile, timestring, ';');
		getline(infile, name);
		getline(infile, VID);
		getline(infile, message);

		HistoryEntry e = HistoryEntry(name, (char*)message.c_str()); // A new temporary history entry
		e.setSourceVID((unsigned char*)VID.c_str());
		e.setTime((time_t)timestring.c_str());
		log.addEntry(e); // Adds the temp entry to the history log

		getline(infile, name); // Read the blank line, but do nothing with it.
		name.clear(); // Clear the place holders so they are ready to use again.
		VID.clear();
		message.clear();
		index.clear();
		timestring.clear();
	}

	return log;
}

