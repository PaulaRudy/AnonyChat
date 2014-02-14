/*
 * Contact.h
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
#include <vector>


using namespace std;

class Contact {
private:
	unsigned char virtual_address[1024]; // Holds the most recent virtual address for the contact. Should this be 1024 bytes?
	string contact_name; // Holds the name assigned to the contact.
public:
	Contact(unsigned char va[1024], string name);
	Contact(){};
	bool isEmpty();
	string getName(){return contact_name;};
	unsigned char* getAddr(){return virtual_address;};
	string toString();
};

class ContactList{
private:
	vector<Contact> contact_list;
public:
	void add(Contact c);
	vector<Contact> getList(){return contact_list;};
	string toString();
};

Contact newContact(unsigned char virtual_add[1024], string name);

void printContact(Contact c);


bool operator == (const Contact &p1, const Contact &p2);

#endif /* CONTACT_H_ */
