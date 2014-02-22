/**
 * neighborlist.h
 *
 * TODO: Needs a description here
 *
 *  Created on: Feb 13, 2014
 *      Authors:
 *      	Andrew
 *      	Paula Rudy (paular@wpi.edu)
 */
#ifndef NEIGHBORLIST_H_
#define NEIGHBORLIST_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

//TODO: These should probbably go in the config.h header
#define PORT 9191
#define NEIGHBORVALUE1 4
#define NEIGHBORVALUE2 6
#define MAXNUMNEIGHBORSALLOWED 8
#define UTILBENCHMARK1 80
#define UTILBENCHMARK2 160
int UserTrustLevel;

class NeighborList {
private:
	std::vector<std::string> list; //The actual list of neighbors
public:
	NeighborList();//Constructor. See NeighborList.cpp for more details.
	~NeighborList();//Destructor. See NeighborList.cpp for more details.
	void AddToNeighbors(std::string);//Function that adds a neighbor to the already existing list of neighbors. See NeighborList.cpp for more details.
	int RemoveNeighbor(std::string);//Function to remove a specific neighbor from the list. See NeighborList.cpp for more details.
	int NumberOfNeighbors(){return NeighborList.size();};//Return the number of neighbors this node has
};

#endif /* NEIGHBORLIST_H_ */
