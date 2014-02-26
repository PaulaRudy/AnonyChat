//TODO: Put a header comment here
#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "message.h"
#include "Connection.h"

class NeighborUtilCountPair{
public:
	std::string IPAddress;
	int utilityCount;

	NeighborUtilCountPair(std::string IPAddressToSet, int utilityCountToSet){
		IPAddress = IPAddressToSet;
		utilityCount = utilityCountToSet;
	}

	NeighborUtilCountPair(const NeighborUtilCountPair &copy ){
		IPAddress = copy.IPAddress;
		utilityCount = copy.utilityCount;
	}

	NeighborUtilCountPair& operator=(const NeighborUtilCountPair &toSet){
		this->IPAddress = toSet.IPAddress;
		this->utilityCount = toSet.utilityCount;
		return *this;
	}

	int operator< (const NeighborUtilCountPair& second) const
	{
		if (this->utilityCount < second.utilityCount)
			return 1;
		else
			return 0;
	}
};

class RoutingTable {
private:
	std::multimap<long long unsigned, NeighborUtilCountPair> table; //The actual routing table
public:
	RoutingTable();//Constructor. See RoutingTable.cpp for more details.
	~RoutingTable();//Destructor. See RoutingTable.cpp for more details.
	int UpdateTableEntry(long long unsigned virtualAddress, NeighborUtilCountPair toAdd);//Function to add or update an entry in the routing table. See RoutingTable.cpp for more details.
	int RemoveTableEntry(long long unsigned virtualAddress);//Function to remove an entry from the routing table. See RoutingTable.cpp for more details.]
	// function to determine the path of a message
	// takes as argument the message to be passed
	// and a connection indicating which connection
	// if any the message was received from
	// returns an int indicating success or failure
	//		failure is possible if every neighbor is down!
	//		otherwise the routing protocol will attempt to recover
	int RouteMessage(Message, Connection);//TODO: finish this.
};


///**
// * Function used to sort the list of NeighborUtilCountPairs for a given virtual address
// * Takes:
// * 		"first" and "second"
// * 			The information for two connections to put in order
// * Returns:
// * 		True if "first" and "second" are in the correct order.
// * 		Otherwise returns false.
// */
//bool Routing_SortConnections( NeighborUtilCountPair first,  NeighborUtilCountPair second) {
//	return  first < second ;
//}
//#endif
