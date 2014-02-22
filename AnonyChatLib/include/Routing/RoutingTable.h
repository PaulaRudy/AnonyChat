#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_

#include <iostream>
#include <list>
#include "message.h"
#include "Connection.h"

using namespace std;

class VirtualAddress {
	unsigned char virtual_address[1024];
};

map<VirtualAddress, map<string, int>> RoutingTable;

// function to add an entry to the routing table
// takes as an unsigned char array representing the virtual address for which the entry is to be added
// the ip address of the neighbor the routing table entry is to represent, and an int
// representing the number of utility counters when the message was received
// returns an int indicating success or failure
int UpdateTableEntry (VirtualAddress, string, int);

// function to remove an entry from the routing table
// takes as an argument an unsigned character array indicating the virtual address
// to remove information for from the routing table
int RemoveTableEntry (VirtualAddress);

// function used to sort the map of connection information for a given virtual address
// takes as an argument the information for two connections
// and returns true if they are in the correct order
// otherwise returns false
bool Routing_SortConnections(pair<string, int>, pair<string, int>);

// function to determine the path of a message
// takes as argument the message to be passed
// and a connection indicating which connection
// if any the message was received from
// returns an int indicating success or failure
//		failure is possible if every neighbor is down!
//		otherwise the routing protocol will attempt to recover
int RouteMessage (Message, Connection);

#endif
