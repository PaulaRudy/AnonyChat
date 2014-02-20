/*
 * neighborlist.h
 *
 *  Created on: Feb 13, 2014
 *      Author: Andrew
 */

#ifndef NEIGHBORLIST_H_
#define NEIGHBORLIST_H_

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#define PORT 9191
#define NEIGHBORVALUE1 4
#define NEIGHBORVALUE2 6
#define NEIGHBORVALUE3 8
#define UTILBENCHMARK1 80
#define UTILBENCHMARK2 160

int UserTrustLevel;

// takes the ip address of the neighbor to be added as a string
// adds that neighbor ip to the neighbor list (after evaluating
// the request) and returns void
void AddToNeighbors(std::string);
// takes the ip address of the neighbor to be removed as a string
// and attempts to remove the specified neighbor. returns 0 if
// successful or -1 if no such neighbor was in the list
int RemoveNeighbor(std::string);
// returns the number of neighbors this node has
int NumberOfNeighbors();
// placeholder for the message send function
int SendToAddress(int, NeighborRequestPDU, int);


//
std::vector<std::string> NeighborList;

#endif /* NEIGHBORLIST_H_ */
