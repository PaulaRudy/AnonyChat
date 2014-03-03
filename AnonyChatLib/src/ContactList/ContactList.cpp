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
			contact_list.erase(contact_list.begin()+i);
			return;
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


/**
 * Reads a contact list from a file that was created using ContactList::write().
 * Constructs a contact list from the contents of the file and then returns it.
 *
 * @param path The path the the file to read from.
 * @return A ContactList constructed from the given file.
 */
ContactList readContactList(string path){
	ifstream infile(path.c_str(), ifstream::in);
	ContactList list;

	/* Place holders for the parts of a contact */
	string name;
	string VID;

	while(infile.peek() != EOF){
		getline(infile, name); // Read the name of the contact from the file.
		getline(infile, VID); // Read the VID of the contact from the file.

		// Creates a new contact and adds it to the list.
		list.add(Contact((unsigned char*)VID.c_str(), name));

		getline(infile, name); // Read the blank line, but do nothing with it.
		name.clear(); // Clear the place holders so they are ready to use again.
		VID.clear();
	}

	return list;
}



/**
 * Checks to see if the contact list contains the given VID
 *
 * @param VID The virtual address to check for.
 * @return True if the list contains VID, false otherwise.
 */
bool ContactList::containsVID(char* VID){
	for(int i = 0; i < contact_list.size(); i++){
		Contact c = contact_list.at(i);
		if(memcmp(c.getAddr(), VID, constants::VID_SIZE)){
			return true; // Found a VID match in the contact list, return true.
		}
	}
	return false; // return false because the VID was not found in the list
}

/**
 * Updates the contact list using the message to find the right contact
 *
 * @param m The message to update the contact list with.
 */
void ContactList::update(Message m){

	/** Check to see if the contact list contains the message source VID **/
	if(this->containsVID((char*)m.getVID(true))){ // If the contact list contains the VID
		for(int i = 0; i<contact_list.size(); i++){
			Contact c = contact_list.at(i);
			if(memcmp(c.getAddr(), (char*)m.getVID(true), constants::VID_SIZE)){
				c.updateChatLog(m);
			}
		}
	} else { // Else create a new contact with new chat log
		Contact c = Contact(m.getVID(true), (const char*)m.getVID(true)); // new contact where the name is just the VID
		HistoryLog log; // A new history log for the contact
		log.addEntry(HistoryEntry(m.getVID(true), m.getMessage())); //Add a new history log entry with the message
		c.setChatLog(log); // set the chat log for the contact
	}
}


/**
 * Updates the chat log with the given message
 *
 * @param m The message to add to the log
 */
void Contact::updateChatLog(Message m){
	HistoryLog log = this->getChatLog();
	log.addEntry(HistoryEntry(m.getVID(true), m.getMessage()));
	this->setChatLog(log);
}
