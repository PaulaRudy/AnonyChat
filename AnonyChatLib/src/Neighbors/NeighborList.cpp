/*
 * NeighborList.cpp
 *
 * TODO: Needs a description here
 *
 *  Created on: Feb 13, 2014
 *      Authors:
 *      	Andrew
 *      	Paula Rudy (paular@wpi.edu)
 */
#include "neighborList.h"

/**
 * Function that adds a neighbor to the already existing list of neighbors.
 * Takes:
 * 		 "newNeighborIP"
 * 		 		The ip address of the new neighbor as a string
 * TODO: Still needs:
 * 		global variable representing the list of neighbors
 * 		global mutex for the list of neighbors
 */
void NeighborList::AddToNeighbors(std::string newNeighborIP) {

	auto result = find(NeighborList.begin(), NeighborList.end(), newNeighborIP);//Try to find the newNeighborIP in our current list of neighbors

	if (result == NeighborList.end()) {//If we did not find the neighbor in the list...

		if (NeighborList.size() == MAXNUMNEIGHBORSALLOWED)//If we are at capacity and need to remove the oldest neighbor before adding a new one...
			NeighborList.erase(NeighborList.begin());//... erase the first neighbor on the list (which will be the oldest, since we add to the back)

		NeighborList.push_back(newNeighborIP);//Add the new neighbor to the end of the list
	}

	return;
}

/**
 * Function to remove a specific neighbor from the list.
 * Takes:
 * 		"NeighborIP"
 * 			The ip address of the neighbor to be removed from the list as a string
 * Returns:
 * 		0 if successful, -1 if no such neighbor was in the list
 */
int NeighborList::RemoveNeighbor(std::string NeighborIP) {
	// attempt to find the specified neighborIP in our neighbor list
	auto result = find(NeighborList.begin(), NeighborList.end(), NeighborIP);

	if (result != NeighborList.end()) {//We found it!
		NeighborList.erase(result);//Erase the entry
		return 0;
	} else { //We didn't find it
		return -1;
	}
}
