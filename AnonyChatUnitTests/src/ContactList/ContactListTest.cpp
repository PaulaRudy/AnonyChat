/*
 * ContactList.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Sam Lalezari
 */

#include "ContactList/ContactList.h"

using namespace std;

int contactListTest(){

	ContactList list;

	unsigned char addr[1024];
	memcpy(addr, (int []){0x61}, sizeof addr);
	Contact c = Contact(addr, "Scooby");

	list.add(c);
	list.add(c);

	cout << list.toString();

	return 0;
}
