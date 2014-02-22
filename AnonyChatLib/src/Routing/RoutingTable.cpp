#include <iostream>
#include <vector>

#include "RoutingTable.h"

using namespace std;

int UpdateTableEntry (VirtualAddress destaddr, string ipaddr, int utilcounters) {
	map<VirtualAddress, map<string, int>>::iterator it;
	map<string, int> interfacelist;
	map<string, int>::iterator listIT;
	vector<pair<string, int>> mapSort;

	// find the entry associated with the virtual address
	it = RoutingTable.find(destaddr);

	// if there was no entry in the routing table associated
	// with the specified destination address
	if (it == RoutingTable.end()) {

		// first we need to make our connection info into a map
		interfacelist.insert(pair<string, int> (ipaddr, utilcounters));

		// now we just insert it into the routing table map
		RoutingTable.insert(pair<VirtualAddress, map<string, int>> (destaddr, interfacelist));

		// and we return 0! this was the easy one...
		return 0;

	} // otherwise if there WAS an entry in the table associated with this virtual address

	// next we check if this virtual address is already associated with this connection ip
	listIT = it->second.find(ipaddr);

	// if there was not already an association between this connection ip and virtual address
	if (listIT == it->second.end()) {

		// then we just need to insert this connection/util counter combination
		it->second.insert(pair<string, int> (ipaddr, utilcounters));

		// in order to sort the interior map we need to convert it to a vector
		mapSort = new vector<pair<string, int>> (it->second.begin(), it->second.end());

		// we now can sort the map using library functions
		sort(mapSort.begin(), mapSort.end(), &Routing_SortConnections);

		// we should be able to return now
		return 0;

	// if there was an association between this connection and virtual ip address
	// and the associated utility counters are more than the new value
	} else if (listIT->second > utilcounters) {

		// then we replace the old utility counters value
		listIT->second = utilcounters;

		// and we sort the map again
		// then we just need to insert this connection/util counter combination
		it->second.insert(pair<string, int> (ipaddr, utilcounters));

		// in order to sort the interior map we need to convert it to a vector
		mapSort = new vector<pair<string, int>> (it->second.begin(), it->second.end());

		// we now can sort the map using library functions
		sort(mapSort.begin(), mapSort.end(), &Routing_SortConnections);

		// then we can return
		return 0;

	} // otherwise we don't need to do anything (the new route was not fewer hops)

	return 0;
}


int RemoveTableEntry (VirtualAddress rmAddr) {
	map<VirtualAddress, map<string, int>>::iterator it;

	// we have to find the entry to remove
	it = RoutingTable.find(rmAddr);

	// if we didn't find anything return an error
	if (it == RoutingTable.end()) return 1;

	// otherwise we just remove it
	RoutingTable.erase(it);

	return 0;
}

bool Routing_SortConnections (pair<string, int> first, pair<string, int> second) {
	if (first.second <= second.second) {
		return true;
	} else {
		return false;
	}
}
