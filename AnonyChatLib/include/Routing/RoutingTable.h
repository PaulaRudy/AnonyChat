#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "message.h"
#include "Connection.h"

using namespace std;

/**
 * An struct made of an unsigned char array representing the
 * virtual address of a destination entry in the Routing Table
 */
struct VirtualAddress {
	unsigned char virtual_address[1024];

};

class RoutingTable {
private:
	map<VirtualAddress, map<string, int>> table; //The actual routing table
	map<VirtualAddress, map<string, int>>::iterator it;//An iterator to traverse the table by destination virtual address
	map<string, int>::iterator listIT;//An iterator to traverse the list of utility count/connection IP address pairs for a single destination virtual address entry in the routing table
public:
	RoutingTable();//Constructor. See RoutingTable.cpp for more details.
	~RoutingTable();//Destructor. See RoutingTable.cpp for more details.
	int UpdateTableEntry(VirtualAddress, string, int);//Function to add or update an entry in the routing table. See RoutingTable.cpp for more details.
	int RemoveTableEntry(VirtualAddress);//Function to remove an entry from the routing table. See RoutingTable.cpp for more details.]
	bool Routing_SortConnections(pair<string, int> , pair<string, int> );// Function used to sort the map of connection information for a given destination virtual address entry in the routing table. See RoutingTable.cpp for more details.
};



// function to determine the path of a message
// takes as argument the message to be passed
// and a connection indicating which connection
// if any the message was received from
// returns an int indicating success or failure
//		failure is possible if every neighbor is down!
//		otherwise the routing protocol will attempt to recover
int RouteMessage(Message, Connection);//TODO: finish this.

#endif
