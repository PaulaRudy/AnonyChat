/*
 * Connection.h
 *
 *  Created on: Feb 15, 2014
 *      Author: Paula Rudy (paular@wpi.edu)
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//Grab the address stored in the sockaddr pointed to by sa, IPv4 *or* IPv6:
void *get_in_addr(struct sockaddr *sa);

/*
 * Connects to the given IP address (passed in as IPAddress)
 * on the given port number (passed in as portNo).
 * Upon sucessful return, sockfd will contain the identifier for
 * the connected socket.
 */
bool ConnectToHost(int portNo, char* IPAddress, int* sockfd );

#endif /* CONNECTION_H_ */
