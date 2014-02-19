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
	openBool = false;
	openBoolReceive = false;
	openBoolSend = false;
	flagToReceiveThread = false;
	openConnection();
	checkState();
}

Connection::Connection(int portNoSendToSet, char* IPAddressToSet) {
	portNoSend = portNoSendToSet;
	portNoReceive = (portNoSendToSet + 1);
	IPAddress = IPAddressToSet;
	openBool = false;
	openBoolReceive = false;
	openBoolSend = false;
	flagToReceiveThread = false;
	openConnection();
	checkState();
}

Connection::Connection(char* IPAddressToSet) {
	portNoSend = 2020;
	portNoReceive = 2020;
	IPAddress = IPAddressToSet;
	openBool = false;
	openBoolReceive = false;
	openBoolSend = false;
	flagToReceiveThread = false;
	openConnection();
	checkState();
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

		openBoolSend = true;

		return 0;
	}

}

int Connection::openReceive() {
	if (!(isOpen())) {
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
		printf(
				"Connection::openReceive(): now waiting for connections...\n  \n"); //TODO: remove this debug code


		//Open a new socket ("sockfdReceive") for the first connection request on the listen queue
		sin_size = sizeof clientAddress;
		sockfdReceive = accept(frontDoor, (struct sockaddr *) &clientAddress,
				&sin_size);
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
		printf("Connection::openReceive(): got connection from %s\n",
				clientAddressString);//TODO: remove this debug code

		close(frontDoor); //Done with this

		openBoolReceive = true;

		//Fork off a new process to handle the newly accepted connection
		receiveThread = std::thread(receiveThreadFunction, &flagToReceiveThread, sockfdReceive, &flagToReceiveThreadMutex, &mutexForBufferFile, IPAddress);

	}
	return 0;
}

void receiveThreadFunction(bool *flagToReceiveThread, int sockfdReceive,
		std::mutex *flagToReceiveThreadMutex, std::mutex *mutexForBufferFile,
		char* IPAddress) {

	(*flagToReceiveThreadMutex).lock();

	std::ofstream bufferFileStream;

	while (!(*flagToReceiveThread)) {
		(*flagToReceiveThreadMutex).unlock();

		int numBytesRecieved;
		char buffer[999999];

		//Try to pull no more than MAXDATASIZE-1 bytes through the socket into the buffer to grab the client's reply
		if ((numBytesRecieved = recv(sockfdReceive, buffer, 999999, 0)) == -1) {//If it fails...//TODO: put real value as max data size
			perror("receiveThreadFunction: recv()");
			close(sockfdReceive);//This child process is done, so we don't need this anymore
			exit(1);
		}

		(*mutexForBufferFile).lock();
		bufferFileStream.open(IPAddress, std::ios::out | std::ios::app);

		bufferFileStream << std::string(buffer, (size_t) numBytesRecieved)
				<< std::endl;

		bufferFileStream.close();

		(*mutexForBufferFile).unlock();

	}

	close(sockfdReceive);//This child process is done, so we don't need this anymore
	exit(0);

}

//TODO: modify for threads
int Connection::closeSend() {
	if (openBoolSend) {
		close(sockfdSend);
	}
	return 0;
}

int Connection::closeReceive() {
	if (openBoolReceive) {

		flagToReceiveThreadMutex.lock();
		flagToReceiveThread = true;
		flagToReceiveThreadMutex.unlock();
		receiveThread.join();
		openBoolReceive = false;
		openBool = false;

	}
	return 0;
}

//used to update openBool
void Connection::checkState() {
	if (openBoolSend && openBoolReceive)
		openBool = true;
	else
		openBool = false;
}


// TODO: Finish these

//int Connection::send(Message toSend) {
//	if (!openBoolSend){
//		perror("Connection::send: send socket not connected!");
//		return 1;
//	}
//
//}

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

