/*
 * NeighborList.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Andrew
 */


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include "neighbor.h"

/*
 * void AddToNeighbors(string newNeighbor) -- function that adds a neighbor to the already existing list of neighbors
 * takes as argument the ip address of the new neighbor as a string
 * returns void
 * requires:
 * 		global variable representing the list of neighbors
 * 		global mutex for the list of neighbors
 */

void AddToNeighbors(std::string newNeighborIP) {

	// attempt to find the newNeighborIP in our current list of neighbors
	auto result = find(NeighborList.begin(), NeighborList.end(), newNeighborIP);

	// if did find the neighbor
	if (result != NeighborList.end()) {
		// it's already a neighbor! we're done here!
		return;
	// otherwise if the neighbor list is not already at capacity
	} else if (NeighborList.size() < NEIGHBORVALUE3) {
		NeighborList.push_back(newNeighborIP);
	// otherwise we are at capacity and need to remove the oldest neighbor
	} else {
		// remove oldest neighbor
		NeighborList.erase(NeighborList.begin());
		NeighborList.push_back(newNeighborIP);
	}

	return;
}


int RemoveNeighbor(std::string NeighborIP) {
	// attempt to find the specified neighborIP in our neighbor list
	auto result = find(NeighborList.begin(), NeighborList.end(), NeighborIP);

	// if we did find it,
	if (result != NeighborList.end()) {
		NeighborList.erase(result);
		return 0;
	// otherwise if not
	} else {
		return -1;
	}
}


int NumberOfNeighbors() {
	return NeighborList.size();
}
