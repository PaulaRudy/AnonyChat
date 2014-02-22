//============================================================================
// Name        : AnonyChatUnitTests.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SampleHeader.h>
#include "ContactList/ContactList.h"

using namespace std;

int main() {

	ContactList list;

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	sampleFunction(3);

	cout << "\n\nContactList Printing:\n";
	unsigned char testVID[constants::VID_SIZE] = "123456789101112131415";
	Contact a = Contact(testVID, "Scooby");
	Contact b = Contact(testVID, "Sam");
	Contact c = Contact(testVID, "Paula");
	Contact d = Contact(testVID, "Andrew");

	list.add(a);
	list.add(b);
	list.add(c);
	list.add(d);

	cout << list.toString();

	cout << "\n\nContactList Writing:\n";
	ofstream file;
	file.open("testwritefile.txt");
	cout << "wrote to file\n";
	list.write(file);
	file.close();


	cout << "\n\nContactList Reading:\n";
	ContactList list2 = readContactList("testwritefile.txt");
	cout << list2.toString();
	cout << "read from file";


	return 0;
}
