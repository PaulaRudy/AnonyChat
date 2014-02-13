/*
 * ContactList.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Sam Lalezari
 */

#include "Contact.h"

bool Contact::isEmpty(){
	if(contact_name.empty() && virtual_address == NULL){
		return true;
	} else {
		return false;
	}
}


Contact::Contact(unsigned char virtual_add[1024], string name){
	contact_name = name;
	memcpy(virtual_address, virtual_add, 1024);
}

void printContact(Contact c){
	cout << c.getName()+ " ";
	cout << c.getAddr();
}


void ContactList::add(Contact c){

	printContact(c);
	for(int x = 0; x < 1024; x++){
		Contact temp_contact = contact_list[x];
		//cout << x << " checking if temp_contact is empty... \n";
		if(temp_contact.isEmpty()){
			cout << "temp_contact was empty\n";
			contact_list[x] = c;
			printContact(c);
			return;;
		}

	}

}
