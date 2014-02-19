/*
 * ContactList.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Sam Lalezari
 */

#include "ContactList.h"

/**
 * Cheks to see if the contact is empty, if it is return true, otherwise return false
 */
bool Contact::isEmpty(){
	if(contact_name.empty() && virtual_address == NULL){
		return true;
	} else {
		return false;
	}
}

/**
 * Contact constructor that takes a virtual address and a name.
 */
Contact::Contact(unsigned char virtual_add[1024], string name){
	contact_name = name;
	memcpy(virtual_address, virtual_add, 1024);
}

/*
 * Turns the contact into a readable string
 */
string Contact::toString(){
	string return_string;

	return_string.append(contact_name);
	return_string.append(" | ");
	return_string.append((char *)virtual_address);

	return return_string;
}

/**
 * Prints the contact to cout
 */
void printContact(Contact c){
	cout << c.toString();
}

/**
 * Adds a contact to the contact list
 */
void ContactList::add(Contact c){
	contact_list.push_back(c);

}

/*
 * Returns a readable string for the contact list
 */
string ContactList::toString(){
	string return_string;
	for(size_t i = 0; i < contact_list.size(); i++){
		Contact c = contact_list.at(i);
		return_string.append(c.toString() + "\n");
	}
	return return_string;
}
