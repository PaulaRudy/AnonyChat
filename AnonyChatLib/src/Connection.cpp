/*
 * Connection.h
 *
 *  Created on: Feb 15, 2014
 *      Author: Paula Rudy (paular@wpi.edu)
 */
#include "../include/Connection.h"
#include <stdlib.h>

//Grab the address stored in the sockaddr pointed to by sa, IPv4 *or* IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	//If the sockaddr is IPv4...
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);//return the IP address stored in the sin_addr of sa

	//The sockaddr must be IPv6
	return &(((struct sockaddr_in6*)sa)->sin6_addr);//return the IP address stored in the sin6_addr of sa
}

/*
 * Connects to the given IP address (passed in as IPAddress)
 * on the given port number (passed in as portNo).
 * Upon sucessful return, sockfd will contain the identifier for
 * the connected socket.
 */
bool ConnectToHost(int portNo, char* IPAddress, int* sockfd ){

	char *port;
	sprintf(port,"%d", portNo);

	struct addrinfo prepInfo;//Used to store information on the criteria for selecting the socket address structures returned in the listOfServerAddr.
	struct addrinfo *listOfServerSocketAddr;//The list of appropriate socket address structures available
	struct addrinfo *iteratorForListOfServerSocketAddr;//Used to iterate through listOfServerSocketAddr
	int returnValueFromgetaddrinfo;//Used to store a return value from a call to getaddrinfo

	//Set up prepInfo to say what kind of socket we want (prepInfo specifies the criteria for selecting the socket address structures returned in the listOfServerAddr)
	memset(&prepInfo, 0, sizeof prepInfo);
	prepInfo.ai_family = AF_UNSPEC; // *either* IPv4 or IPv6
	prepInfo.ai_socktype = SOCK_STREAM;//only use TCP

	//Set up the list of appropriate socket address structures available
	if ((returnValueFromgetaddrinfo = getaddrinfo(IPAddress, port, &prepInfo, &listOfServerSocketAddr)) != 0) {//If it fails...
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnValueFromgetaddrinfo));
		return false;
	}

	//Loop through all the results in listOfServerSocketAddr, create a socket and connect to the first address we can
	for(iteratorForListOfServerSocketAddr = listOfServerSocketAddr; iteratorForListOfServerSocketAddr != NULL; iteratorForListOfServerSocketAddr = iteratorForListOfServerSocketAddr->ai_next) {

		//Try to create the socket
		if ((*sockfd = socket(iteratorForListOfServerSocketAddr->ai_family, iteratorForListOfServerSocketAddr->ai_socktype,
				iteratorForListOfServerSocketAddr->ai_protocol)) == -1) {//If it fails...
			perror("client: socket()");
			continue;
		}

		//Try to connect to the address stored in this addrinfo, over the newly created socket
		if (connect(*sockfd, iteratorForListOfServerSocketAddr->ai_addr, iteratorForListOfServerSocketAddr->ai_addrlen) == -1) {//If it fails...
			close(*sockfd);
			perror("client: connect()");
			continue;
		}

		//Connection successful
		break;
	}

	if (iteratorForListOfServerSocketAddr == NULL) {//If we have iterated over the list of addrinfo ("listOfServerSocketAddr") and failed to successfully connect to any of the addresses...
		fprintf(stderr, "client: failed to connect\n");
		return false;
	}


	freeaddrinfo(listOfServerSocketAddr);//We're all done with this structure, so free the memory

	return true;
}
