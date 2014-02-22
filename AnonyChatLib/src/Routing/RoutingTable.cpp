//TODO: Put a header comment here
#include "RoutingTable.h"

//TODO: Put a function header comment here
RoutingTable::RoutingTable(void) {
//TODO: Put code here to initalize the routing table from a file
}

/**
 * Function to add an entry to the routing table
 * Takes:
 * 		"destaddr"
 * 			A VirtualAddress (see RoutingTable.h)representing the destination virtual address
 * 			for which the entry is to be updated or added
 * 		"ipaddr"
 * 			The ip address of the neighbor the routing table entry is to represent
 * 		"utilcounter"
 * 			An int representing the utility counter of the message when it was received
 *
 * Returns an int indicating success (0) or failure (1)
 */
int RoutingTable::UpdateTableEntry(VirtualAddress destaddr, string ipaddr,
		int utilcounter) {

	it = table.find(destaddr);// find the entry associated with the destination virtual address

	if (it == table.end()) {// If there was no entry in the routing table associated with the specified destination address....

		//Make a new entry and add it to the table:
		map<string, int> interfacelist;

		interfacelist.insert(pair<string, int> (ipaddr, utilcounter));//First we need to make our connection info (IP address of the connection and the associated utility count) into a map

		table.insert(
				pair<VirtualAddress, map<string, int>> (destaddr, interfacelist));// Now we just insert it into the routing table map associated with the destination virtual address

	} else { //We found an entry associated with this destination virtual address in the table

		listIT = it->second.find(ipaddr); //Check the entry for this destination virtual address's map of utility count to connection IP address for an existing association with this connection (identified by ip address)

		if (listIT == it->second.end()) {// If there *isn't* an existing association between this connection ip and this destination virtual address in the map

			it->second.insert(pair<string, int> (ipaddr, utilcounter));// Then we need to insert this connection/util counter combination into the map for this VID

			//Sort the map (the map of utility count/connection IP address pairs for this connection) before returning:
			//TODO: This should probbably be a function.
			vector<pair<string, int>> mapSort;

			mapSort = new vector<pair<string, int>> (it->second.begin(), //In order to sort the map we need to convert it to a vector first
					it->second.end());

			sort(mapSort.begin(), mapSort.end(), &RoutingTable::Routing_SortConnections);//We can now sort the map using "Routing_SortConnections" function (defined in this file)

		} else if (listIT->second > utilcounter) {// If there *was* an association between this connection and this destination virtual ip address, and the associated utility counter is more than the new utility count

			listIT->second = utilcounter;//Then we replace the old utility count value...

			it->second.insert(pair<string, int> (ipaddr, utilcounter));// And insert this connection/util counter combination

			//Sort the map (the map of utility count/connection IP address pairs for this connection) before returning:
			//TODO: This should probbably be a function.
			vector<pair<string, int>> mapSort;

			mapSort = new vector<pair<string, int>> (it->second.begin(), //In order to sort the interior map we need to convert it to a vector first
					it->second.end());

			sort(mapSort.begin(), mapSort.end(), &RoutingTable::Routing_SortConnections);//We can now sort the map using the "Routing_SortConnections" function (defined in this file)

		} // otherwise we don't need to do anything (The new route was not fewer hops)

	}

	return 0;//Indicate success
}

/**
 * Function to remove an entry from the routing table.
 *
 * Takes:
 * 		"rmAddr"
 * 			A VirtualAddress (see RoutingTable.h) indicating the destination virtual address entry
 * 			in the table to remove
 *
 * Returns an int indicating success (0) or failure (1)
 */
int RoutingTable::RemoveTableEntry(VirtualAddress rmAddr) {

	it = table.find(rmAddr);// Find the entry to remove

	//If we didn't find anything, return an error
	if (it == table.end())
		return 1;

	table.erase(it);//Otherwise we just remove it

	return 0;//Indicate success
}

/**
 * Function used to sort the map of connection information for a given virtual address
 * Takes:
 * 		"first" and "second"
 * 			The information for two connections to put in order
 * Returns:
 * 		True if "first" and "second" are in the correct order.
 * 		Otherwise returns false.
 */
bool Routing_SortConnections(pair<string, int> first, pair<string, int> second) {
	if (first.second <= second.second) {
		return true;
	} else {
		return false;
	}
}

//TODO: Put a function header comment here
RoutingTable::~RoutingTable(void) {
//TODO: Put code here to store the routing table to a file before exit
}

