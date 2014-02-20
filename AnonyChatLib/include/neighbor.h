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
#define NEIGHBORVALUE1 5
#define NEIGHBORVALUE2 10
#define NEIGHBORVALUE3 15
#define UTILBENCHMARK1 80
#define UTILBENCHMARK2 160

int UserTrustLevel;

void AddToNeighbors(std::string newNeighbor);
int NumberOfNeighbors();
int SendToAddress(int, NeighborRequestPDU, int);

std::vector<std::string> NeighborList;

#endif /* NEIGHBORLIST_H_ */
