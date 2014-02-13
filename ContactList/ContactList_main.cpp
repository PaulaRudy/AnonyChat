/*
 * ContactList.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Sam Lalezari
 */

#include "Contact.h"

using namespace std;

int main(int argc, char *argv[]){

	ContactList list;

	unsigned char addr[1024];
	memcpy(addr, (int []){0x00}, sizeof addr);
	Contact c = Contact(addr, "Scooby");

	list.add(c);

	return 0;
}
