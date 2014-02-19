/*
 * Connection.cpp
 *
 *	Note: Currently needs some work on threading, comments
 *
 *  Created on: Feb 15, 2014
 *      Author: Paula Rudy (paular@wpi.edu)
 */
#include "../include/Connection.h"

Connection::Connection(int portNoSendToSet, int portNoRecieveToSet,
		char* IPAddressToSet) {
	portNoReceive = portNoRecieveToSet;
	portNoSend = portNoSendToSet;
	IPAddress = IPAddressToSet;
	openConnection();
}

Connection::Connection(int portNoSendToSet, char* IPAddressToSet) {
	portNoSend = portNoSendToSet;
	portNoReceive = (portNoSendToSet + 1);
	IPAddress = IPAddressToSet;
	openConnection();
}

Connection::Connection(char* IPAddressToSet) {
	portNoSend = 2020;
	portNoReceive = 2020;
	IPAddress = IPAddressToSet;
	openConnection();
}

int Connection::openConnection() {
	if (isOpen()) {
		return 0;
	} else {
		int rvReceive = openReceive(); //openRecieve must be first to accept incoming connections
		int rvSend = openSend();

		if ((rvSend == 1) || (rvReceive == 1))
			return 1;
		else
			return 0;
	}
}

int Connection::closeConnection() {
	if (!(isOpen())) {
		return 0;
	} else {
		int rvSend = closeSend();
		int rvReceive = closeReceive();

		if ((rvSend == 1) || (rvReceive == 1))
			return 1;
		else
			return 0;
	}

}

//TODO: modify for threads
int Connection::openSend() {
	if (isOpen()) {
		return 0;
	} else {
		char *port;
		sprintf(port, "%d", portNoSend);

		struct addrinfo prepInfo;//Used to store information on the criteria for selecting the socket address structures returned in the listOfServerAddr.
		struct addrinfo *listOfServerSocketAddr;//The list of appropriate socket address structures available
		struct addrinfo *iteratorForListOfServerSocketAddr;//Used to iterate through listOfServerSocketAddr
		int returnValueFromgetaddrinfo;//Used to store a return value from a call to getaddrinfo

		//Set up prepInfo to say what kind of socket we want (prepInfo specifies the criteria for selecting the socket address structures returned in the listOfServerAddr)
		memset(&prepInfo, 0, sizeof prepInfo);
		prepInfo.ai_family = AF_UNSPEC; // *either* IPv4 or IPv6
		prepInfo.ai_socktype = SOCK_STREAM;//only use TCP

		//Set up the list of appropriate socket address structures available
		if ((returnValueFromgetaddrinfo = getaddrinfo(IPAddress, port,
				&prepInfo, &listOfServerSocketAddr)) != 0) {//If it fails...
			fprintf(stderr, "Connection::openSend(): getaddrinfo: %s\n",
					gai_strerror(returnValueFromgetaddrinfo));
			return 1;
		}

		//Loop through all the results in listOfServerSocketAddr, create a socket and connect to the first address we can
		for (iteratorForListOfServerSocketAddr = listOfServerSocketAddr; iteratorForListOfServerSocketAddr
		!= NULL; iteratorForListOfServerSocketAddr
		= iteratorForListOfServerSocketAddr->ai_next) {

			//Try to create the socket
			if ((sockfdSend = socket(
					iteratorForListOfServerSocketAddr->ai_family,
					iteratorForListOfServerSocketAddr->ai_socktype,
					iteratorForListOfServerSocketAddr->ai_protocol)) == -1) {//If it fails...
				perror("Connection::openSend(): socket()");
				continue;
			}

			//Try to connect to the address stored in this addrinfo, over the newly created socket
			if (connect(sockfdSend, iteratorForListOfServerSocketAddr->ai_addr,
					iteratorForListOfServerSocketAddr->ai_addrlen) == -1) {//If it fails...
				close(sockfdSend);
				perror("Connection::openSend(): connect()");
				continue;
			}

			//Connection successful
			break;
		}

		if (iteratorForListOfServerSocketAddr == NULL) {//If we have iterated over the list of addrinfo ("listOfServerSocketAddr") and failed to successfully connect to any of the addresses...
			fprintf(stderr, "Connection::openSend(): failed to connect\n");
			return 1;
		}

		freeaddrinfo(listOfServerSocketAddr);//We're all done with this structure, so free the memory

		return 0;
	}

}


int Connection::openReceive() {
	if (isOpen()) {
		return 0;
	} else {
		char *port;
		sprintf(port, "%d", (portNoReceive + 1));

		struct addrinfo prepInfo;//Used to store information on the criteria for selecting the socket address structures returned in the listOfServerAddr.
		struct addrinfo *listOfServerSocketAddr;//The list of appropriate socket address structures available
		struct addrinfo *iteratorForListOfServerSocketAddr;//Used to iterate through listOfServerSocketAddr
		int frontDoor;//listen for incoming connection request on frontDoor
		int returnValueFromgetaddrinfo;//Used to store a return value from a call to getaddrinfo
		int yes = 1;//Used to set socket options
		struct sockaddr_storage clientAddress;//Client's address information
		socklen_t sin_size;//Used for accepting new connections
		char clientAddressString[INET6_ADDRSTRLEN];//Used to store a calling client's IP address as a string

		//Set up prepInfo to say what kind of socket we want (prepInfo specifies the criteria for selecting the socket address structures returned in the listOfServerAddr)
		memset(&prepInfo, 0, sizeof prepInfo);
		prepInfo.ai_family = AF_UNSPEC; // *either* IPv4 or IPv6
		prepInfo.ai_socktype = SOCK_STREAM;//only use TCP

		//Set up the list of appropriate socket address structures available
		if ((returnValueFromgetaddrinfo = getaddrinfo(IPAddress, port,
				&prepInfo, &listOfServerSocketAddr)) != 0) {//If it fails...
			fprintf(stderr, "Connection::openReceive(): getaddrinfo: %s\n",
					gai_strerror(returnValueFromgetaddrinfo));
			return 1;
		}

		//Loop through all the results in listOfServerSocketAddr, create a socket and connect to the first address we can
		for (iteratorForListOfServerSocketAddr = listOfServerSocketAddr; iteratorForListOfServerSocketAddr
		!= NULL; iteratorForListOfServerSocketAddr
		= iteratorForListOfServerSocketAddr->ai_next) {

			//Try to create the socket
			if ((frontDoor = socket(
					iteratorForListOfServerSocketAddr->ai_family,
					iteratorForListOfServerSocketAddr->ai_socktype,
					iteratorForListOfServerSocketAddr->ai_protocol)) == -1) {//If it fails...
				perror("Connection::openReceive(): socket()");
				continue;
			}

			//Try to set the socket options
			if (setsockopt(frontDoor, SOL_SOCKET, SO_REUSEADDR, &yes,
					sizeof(int)) == -1) {//If it fails...
				perror("Connection::openReceive(): setsockopt()");
				exit(1);
			}

			//Try to bind the socket (assign the address contained in the addrinfo structure pointed to by iteratorForListOfServerSocketAddr to the socket)
			if (bind(frontDoor, iteratorForListOfServerSocketAddr->ai_addr,
					iteratorForListOfServerSocketAddr->ai_addrlen) == -1) {//If it fails...
				close(frontDoor);//close the socket
				perror("Connection::openReceive(): bind()");
				continue;
			}

			//Socket successfully bound
			break;
		}

		if (iteratorForListOfServerSocketAddr == NULL) {///If we have iterated over the list of addrinfo ("listOfServerSocketAddr") and failed to successfully bind a socket to any of them...
			fprintf(stderr, "Connection::openReceive(): failed to bind\n");
			return 1;
		}

		freeaddrinfo(listOfServerSocketAddr);//We're all done with this structure, so free the memory

		//Try to listen on "frontdoor" for incoming connection requests. These requests are queued in a queue of max length "MAXWAITINGCLIENTS"
		if (listen(frontDoor, 2) == -1) {//If it fails...
			perror("Connection::openReceive(): listen()");
			close(sockfdReceive);
			return 1;
		}

		// Try to reap all dead processes
		sa.sa_handler = sigchld_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		if (sigaction(SIGCHLD, &sa, NULL) == -1) {//If it fails...
			perror("Connection::openReceive(): sigaction()");
			close(sockfdReceive);
			return 1;
		}

		//Print a message to the console indicating a successful set up
		printf("Connection::openReceive(): now waiting for connections...\n  \n"); //TODO: remove this debug code

		// Main loop used to accept incoming connections
		while (1) {

			//Open a new socket ("sockfdReceive") for the first connection request on the listen queue
			sin_size = sizeof clientAddress;
			sockfdReceive = accept(frontDoor,
					(struct sockaddr *) &clientAddress, &sin_size);
			if (sockfdReceive == -1) {//If failure accept()ing:
				perror("Connection::openReceive(): accept()");
				close(frontDoor);
				close(sockfdReceive);
				return 1;
			}

			//Find and store the calling client's IP address to a string
			inet_ntop(clientAddress.ss_family,
					getAddrFromSockaddr((struct sockaddr *) &clientAddress),
					clientAddressString, sizeof clientAddressString);//TODO: remove this debug code

			//Print a message to the console to indicate a connection has been received
			printf("Connection::openReceive(): got connection from %s\n", clientAddressString);//TODO: remove this debug code

			//Fork off a new process to handle the newly accepted connection
			if (!fork()) {//TODO: modify for threads

				close(frontDoor); //The child process doesn't need the listener


				//TODO: put this in closeReceive
				//				close(sockfdReceive);//This child process is done, so we don't need this anymore
				//				exit(0);//Close this child process
			}

		}

	}
}

//TODO: modify for threads
int Connection::closeSend() {
	if (isOpen()) {
		close(sockfdSend);
	}
	return 0;
}

//TODO: modify for threads
int Connection::closeReceive() {
	if (isOpen()) {
		close(sockfdReceive);
	}
	return 0;
}

// TODO: Finish these
//int Connection::send(Message toSend) {
//
//}
//
//Message Connection::receive() {
//
//}
//
//Connection newConnection(int portNoSendToSet, int portNoRecieveToSet,
//		char* IPAddressToSet) {
//
//}
//
//void printConnection(Connection c) {
//
//}
//
//bool operator ==(const Connection &p1, const Connection &p2) {
//
//}

