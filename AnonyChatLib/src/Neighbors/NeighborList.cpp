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
#include "NeighborList.h"

/**
 * Function that adds a neighbor to the already existing list of neighbors.
 * Takes:
 * 		 "newNeighborIP"
 * 		 		The ip address of the new neighbor as a string
 * TODO: Still needs:
 * 		global variable representing the list of neighbors
 * 		global mutex for the list of neighbors
 */
void NeighborList::AddToNeighbors(Neighbor newNeighbor) {

	auto result = find(list.begin(), list.end(), newNeighbor);//Try to find the newNeighborIP in our current list of neighbors

	if (result == list.end()) {//If we did not find the neighbor in the list...

		if (list.size() == MAXNEIGHBORS)//If we are at capacity and need to remove the oldest neighbor before adding a new one...
			list.erase(list.begin());//... erase the first neighbor on the list (which will be the oldest, since we add to the back)

		list.push_back(newNeighbor);//Add the new neighbor to the end of the list
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
int NeighborList::RemoveNeighbor(Neighbor newNeighbor) {
	// attempt to find the specified neighborIP in our neighbor list
	auto result = find(list.begin(), list.end(),newNeighbor);

	if (result != list.end()) {//We found it!
		list.erase(result);//Erase the entry
		return 0;
	} else { //We didn't find it
		return -1;
	}
}

/**
 * takes as input the filename to use to configure the neighbor list
 */

void NeighborList::ReadNeighborList(char *filename) {
	std::ifstream setupfile;
	std::string line;

	// open the file for reading
	setupfile.open(filename);

	// make sure our list is empty
	list.clear();

//	// read in each ip address
//	while (getline (setupfile, line)) {
//		list.push_back(line);
//	}
//
//	// once we're done close the file
//	setupfile.close();
//
//	//return
//	return;
}


/**
 * takes as input the filename to use to store the neighbor list
 */

void NeighborList::WriteNeighborList(char *filename) {
	std::ofstream setupfile;
	std::vector<std::string>::iterator it;

	// open the file for writing
	setupfile.open(filename);

	// iterate through the list, putting the contents of each line into the file
	//for (it = list.begin(); it != list.end(); it++) {
//		setupfile << *it;
//		setupfile << '\n';
//	}

	// we're done, close our file
	setupfile.close();

	// return
	return;

}



