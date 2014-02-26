//TODO: Put a header comment here
#include "RoutingTable.h"

//TODO: Put a function header comment here
RoutingTable::RoutingTable(void) {
//TODO: Put code here to initalize the routing table from a file
}

//Function to add or update an entry in the routing table.
int RoutingTable::UpdateTableEntry(long long unsigned virtualAddress, NeighborUtilCountPair toAdd){

	std::multimap<char,int>::iterator it = table.find(virtualAddress);

	if (it == table.end()) {// If there was no entry in the routing table associated with the specified destination address....

	}
}


//Function to remove an entry from the routing table.
int RoutingTable::RemoveTableEntry(long long unsigned virtualAddress){

}



//TODO: finish this.
int RoutingTable::RouteMessage(Message, Connection){

}

//TODO: Put a function header comment here
RoutingTable::~RoutingTable(void) {
//TODO: Put code here to store the routing table to a file before exit
}

