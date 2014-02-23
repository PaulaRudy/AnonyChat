/*
 * NeighborRequestConnection.h
 *
 *	This defines the class used to manage neighbor requests -
 *	to both listen on an open TCP connection for incoming neighbor requests
 *	(see openFrontDoor andlistenThreadFunction both defined in NeighborRequestConnection.cpp),
 *	and to send a neighbor request (as a NeighborRequestPDU -see messagePDU.h) to a specific
 *	IP address. The definitions for the functions declared here can be found in
 *	NeighborRequestConnection.cpp.
 *
 *	Please note that there are multiple threads being used in the functions declared here, so
 *	it is prudent to review NeighborRequestConnection.cpp before attempting to use them.
 *
 *  Created on: Feb 22, 2014
 *      Author: Paula Rudy (paular@wpi.edu)
 */

#ifndef NEIGHBORREQUESTCONNECTION_H_
#define NEIGHBORREQUESTCONNECTION_H_

//TODO: This should probably go in config.h
#define NEIGHBORREQUESTCONNECTIONLISTENPORT 9000

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <thread>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>
#include <ifaddrs.h>
#include <mutex>
#include "messagepdu.h"
#include "NeighborList.h"

class NeighborRequestConnection {
private:
	int frontDoorSockfd;//The socket descriptor of the socket used to accept incoming neighbor request connections
	bool flagToFrontDoorThread;//Used to signal to frontDoorThread that it should stop and join();
	std::mutex flagToFrontDoorThreadMutex;//Mutex lock for flagToFrontDoorThread
	std::thread frontDoorThread;//Used listen at the frontDoor for incoming connection requests and to handle them as they occur. See listenThreadFunction as defined in NeighborRequestConnection.cpp
	int openFrontDoor(char* IPAddress);//Used to open or reopen the front door connection.  See NeighborRequestConnection.cpp for more details.
	int useFrontDoor(NeighborRequestPDU toSend, NeighborRequestPDU *toReceive, char* IPAddress);//Internally used to send a NeighborRequestPDU to a specific IP address. See NeighborRequestConnection.cpp for more details.
	bool EvaluateNeighborRequest(NeighborRequestPDU newNeighbor);//Used to decide to accept a neighbor, ask the user, or discard a neighbor request passed in as newNeighbor. See NeighborRequestConnection.cpp for more details.
public:
	NeighborRequestConnection();//Constructor. See NeighborRequestConnection.cpp for more details.
	~NeighborRequestConnection();//Destructor. Needed to signal the frontDoorThread to clean up and exit before the NeighborRequestConnection is destroyed.
	int broadcastNeighborRequest();//Used to broadcast a neighbor request on all active connections. See NeighborRequestConnection.cpp for more details.
	int invite_neighbor(char * ipaddr, NeighborList list);//Invite a node at a specific IP address to become our neighbor. Uses the private useFrontDoor function.  NeighborRequestConnection.cpp for more details.
};

void listenThreadFunction(bool *flagToFrontDoorThread, int frontDoorSockfd,
		std::mutex *flagToFrontDoorThreadMutex);//The function executed by the frontDoorThread. See NeighborRequestConnection.cpp for more details.

//Helper function: grabs the address stored in the sockaddr pointed to by sa, IPv4 *or* IPv6
void *getAddrFromSockaddr(struct sockaddr *sa)
{
	//If the sockaddr is IPv4...
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);//return the IP address stored in the sin_addr of sa

	//The sockaddr must be IPv6
	return &(((struct sockaddr_in6*)sa)->sin6_addr);//return the IP address stored in the sin6_addr of sa
}

//Helper function: handler used to reap dead processes
void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

#endif /* NEIGHBORREQUESTCONNECTION_H_ */
