//TODO: Put a header comment here
#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include "message.h"
#include "Connection.h"

/**
 * This class is used only within the routing table to hold an IP address
 * of a currently connected neighbor and the lowest utility count observed
 * on messages arrived through that neighbor for a specific virtual address.
 *
 * TODO: Put this in it's own header maybe? I don't see how it would be needed without needing the routing table class too, so maybe not...
 */
class NeighborUtilCountPair{
public:
	std::string IPAddress;//The IP address of this connection
	int utilityCount;//The lowest utility count observed on messages arriving through that connection to a specific virtual address

	//Basic constructor
	NeighborUtilCountPair(std::string IPAddressToSet, int utilityCountToSet) {
		IPAddress = IPAddressToSet;
		utilityCount = utilityCountToSet;
	}

	//Copy constructor. Needed for functions defined in <algorithm>
	NeighborUtilCountPair(const NeighborUtilCountPair &copy) {
		IPAddress = copy.IPAddress;
		utilityCount = copy.utilityCount;
	}

	//Overloaded "=" operator for this class. Needed for functions defined in <algorithm>
	NeighborUtilCountPair& operator=(const NeighborUtilCountPair &toSet) {
		this->IPAddress = toSet.IPAddress;
		this->utilityCount = toSet.utilityCount;
		return *this;
	}

	//Overloaded "==" operator for this class. Needed for functions defined in <algorithm>
	int operator==(const NeighborUtilCountPair &toCompare) const {
		if ((this->IPAddress != toCompare.IPAddress) || (this->utilityCount != toCompare.utilityCount))
			return 0;
		else
			return 1;
	}

	//Overloaded "<" operator for this class. Needed for functions defined in <algorithm>
	int operator<(const NeighborUtilCountPair& second) const {
		if (this->utilityCount < second.utilityCount)
			return 1;
		else
			return 0;
	}
};

/**
 * The actual routing table.
 * This class contains the routing table itself and it's functions.
 * See RoutingTable.cpp for more details on the functions declared
 * below.
 */
class RoutingTable {
private:
	std::multimap<long long unsigned, NeighborUtilCountPair> table; //The actual routing table
	int fillAndSortListOfRoutesForVirtualAddress(long long unsigned virtualAddress,std::list<NeighborUtilCountPair> *toFill);//Fills the list of NeighborUtilCountPairs with all entries in the routing table for the given virtual address, and sorts the list in ascending utility count order. See RoutingTable.cpp for more details.
	int RemoveTableEntry(long long unsigned virtualAddress, NeighborUtilCountPair toRemove);//Function to remove only the entry from the routing table with the given virtual address and NeighborUtilCountPair. See RoutingTable.cpp for more details.
public:
	RoutingTable();//Constructor. See RoutingTable.cpp for more details.
	~RoutingTable();//Destructor. See RoutingTable.cpp for more details.
	void UpdateTableEntry(long long unsigned virtualAddress, NeighborUtilCountPair toAdd);//Function to add or update an entry in the routing table. See RoutingTable.cpp for more details.
	int RemoveTableEntry(long long unsigned virtualAddress);//Function to remove all entries from the routing table with the given virtual address. See RoutingTable.cpp for more details.
	// function to determine the path of a message
	// takes as argument the message to be passed
	// and a connection indicating which connection
	// if any the message was received from
	// returns an int indicating success or failure
	//		failure is possible if every neighbor is down!
	//		otherwise the routing protocol will attempt to recover
	int RouteMessage(Message, Connection);//TODO: finish this.
};

#endif
