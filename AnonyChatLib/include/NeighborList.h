/**
 * Neighborlist.h
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
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include "Connection.h"

//TODO: These should probbably go in the config.h header
#define PORT 9191
#define MINNEIGHBORS 4
#define MAXNEIGHBORS 8
#define UTILBENCHMARK 50
#define MAX_UCS 80
int UserTrustLevel;

class Neighbor{
public:
	std::string IPaddress;
	Connection *neighborConnection;

	Neighbor(std::string IPaddressToSet, Connection *neighborConnectionToSet){
		IPaddress = IPaddressToSet;
		neighborConnection = neighborConnectionToSet;
	}

	//Copy constructor. Needed for functions defined in <algorithm>
	Neighbor(const Neighbor &copy) {
		IPaddress = copy.IPaddress;
		neighborConnection = copy.neighborConnection;
	}

	//Overloaded "=" operator for this class. Needed for functions defined in <algorithm>
	Neighbor& operator=(const Neighbor &toSet) {
		this->IPaddress = toSet.IPaddress;
		this->neighborConnection = toSet.neighborConnection;
		return *this;
	}

	//Overloaded "==" operator for this class. Needed for functions defined in <algorithm>
	bool operator==(const Neighbor &toCompare) const {
		return (this->IPaddress.compare(toCompare.IPaddress) != 0);
	}

	~Neighbor(){
		neighborConnection->closeConnection();
		//TODO: mutex pointers? thread pointer?
	}

};

class NeighborList {
public:
	std::vector<Neighbor> list; //The actual list of neighbors
	NeighborList();//Constructor. See NeighborList.cpp for more details.
	~NeighborList();//Destructor. See NeighborList.cpp for more details.
	void ReadNeighborList(char *); //takes a filename and populates the neighbor list from the file specified
	void WriteNeighborList(char *); //takes a filename and writes the neighbor list to the file specified
	void AddToNeighbors(Neighbor newNeighbor);//Function that adds a neighbor to the already existing list of neighbors. See NeighborList.cpp for more details.
	int RemoveNeighbor(Neighbor newNeighbor);//Function to remove a specific neighbor from the list. See NeighborList.cpp for more details.
	int NumberOfNeighbors(){return list.size();};//Return the number of neighbors this node has
};

NeighborList TheNeighbors;

#endif /* NEIGHBORLIST_H_ */
