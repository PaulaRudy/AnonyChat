/*
 * Contact.h
 *
 *	TODO: Needs comments/headers
 *
 *  Created on: Feb 13, 2014
 *      Author: sam
 */

#ifndef CONTACT_H_
#define CONTACT_H_

#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "../chatHistory.h"


using namespace std;

class Contact {
private:
	unsigned char virtual_address[1024]; // Holds the most recent virtual address for the contact. Should this be 1024 bytes?
	string contact_name; // Holds the name assigned to the contact.
	HistoryLog chatLog;
public:
	Contact(unsigned char va[1024], string name);
	bool isEmpty();
	string getName(){return contact_name;};
	unsigned char* getAddr(){return virtual_address;};
	string toString();
	HistoryLog getChatLog(){return chatLog;};
	void setChatLog(HistoryLog log){chatLog = log;};
	void updateChatLog(Message m);
};

class ContactList{
private:
	vector<Contact> contact_list;
public:
	void add(Contact c);
	void remove(string contactName);
	vector<Contact> getList(){return contact_list;};
	string toString();
	void write(ofstream& s);
	bool containsVID(char* virtualAdd);
	void update(Message m);
};

void printContact(Contact c);

ContactList readContactList(string path);

bool operator == (const Contact &p1, const Contact &p2);

#endif /* CONTACT_H_ */
