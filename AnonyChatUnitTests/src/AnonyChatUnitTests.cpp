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
#include "chatHistory.h"

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

	cout << "\n\nContactList Removing Scooby:\n";
	list.remove("Scooby");
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

	cout << "\n\nHistoryEntry toString:\n";
	HistoryEntry e1 = HistoryEntry("Sam", "This is a test message");
	e1.setSourceVID(testVID);
	cout << e1.toString().c_str();

	cout << "\n\nHistoryLog Printing:\n";
	HistoryEntry e2 = HistoryEntry ("Sam", "This is another test message.");
	e2.setSourceVID(testVID);

	HistoryEntry e3 = HistoryEntry ("Sam", "This is a third test message.");
	e3.setSourceVID(testVID);

	HistoryLog historyLog = HistoryLog("Sam", testVID);
	historyLog.addEntry(e1);
	historyLog.addEntry(e2);
	historyLog.addEntry(e3);

	historyLog.print();


	cout << "\n\nHistoryLog Writing:\n";
	ofstream historyLogFile;
	historyLogFile.open("testHistoryLogWrite");
	historyLog.write(historyLogFile);
	cout << "wrote to historyLogFile\n";
	historyLogFile.close();


	cout << "\n\nHistoryLog Reading:\n";
	HistoryLog testLogFromRead = readHistoryLog("testHistoryLogWrite");
	testLogFromRead.print();

	return 0;
}
