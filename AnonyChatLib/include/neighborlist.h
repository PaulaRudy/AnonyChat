/*
 * neighborlist.h
 *
 *  Created on: Feb 13, 2014
 *      Author: Andrew
 */

#ifndef NEIGHBORLIST_H_
#define NEIGHBORLIST_H_

void AddToNeighbors(string newNeighbor);
void RemoveOldestNeighbor();

class NeighborData {
	long unsigned timeToLive;
	string address;
};

queue<NeighborData*> NeighborList;

#endif /* NEIGHBORLIST_H_ */
