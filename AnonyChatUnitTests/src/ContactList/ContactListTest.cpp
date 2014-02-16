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


	Contact c = Contact(0, "Scooby");

	list.add(c);
	list.add(c);

	cout << list.toString();

	return 0;
}
