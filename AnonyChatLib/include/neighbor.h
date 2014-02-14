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

void AddToNeighbors(string newNeighbor);
int NumberOfNeighbors();

vector<string> NeighborList;

#endif /* NEIGHBORLIST_H_ */
