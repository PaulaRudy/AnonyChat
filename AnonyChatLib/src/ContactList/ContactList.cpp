/*
 * ContactList.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Sam Lalezari
 */

#include "../../include/ContactList/ContactList.h"

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
	chatLog = HistoryLog(name, virtual_add);
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
 * Checks to make sure that there are no other contacts with the same name in the list.
 */
void ContactList::add(Contact c){
	for(unsigned int i = 0; i < contact_list.size(); i++){
		Contact contact = contact_list.at(i);
		if(!contact.getName().compare(c.getName())){
			return;
		}
	}
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

/**
 * Removes the contact from the contact list based on the given contact name.
 */
void ContactList::remove(string contactName){
	for(unsigned int i = 0; i < contact_list.size(); i++){
		Contact c = contact_list.at(i);
		if(c.getName().compare(contactName) == 0){

		}
	}
}

/**
 * Writes the contact list to a given ofstream
 *
 * A single end line char delimites the name from it's VID, and two end line characters
 * delimites two contacts.
 *
 * For example, if there was a contact list with two contacts Sue and Sally
 *  with VIDs of 1 and 2, respectivly. The output will look like this:
 *
 *  -----------------------
 *  Sue
 *  1
 *
 *  Sally
 *  2
 *  -----------------------
 *
 * @param s the stream to write the file to
 *
 */
void ContactList::write(ofstream& s){
	for(unsigned int i = 0; i < contact_list.size(); i++){
		Contact c = contact_list.at(i);
		s << c.getName();
		s << "\n";
		s << c.getAddr();
		s << "\n\n";
	}
}

ContactList readContactList(string path){
	ifstream infile(path.c_str(), ifstream::in);
	ContactList list;

	string name;
	string VID;

	while(infile.peek() != EOF){
		getline(infile, name);
		getline(infile, VID);
		list.add(Contact((unsigned char*)VID.c_str(), name));
		getline(infile, name);
		name.clear();
		VID.clear();
	}

	return list;
}


