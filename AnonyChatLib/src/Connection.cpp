/*
 * Connection.cpp
 *
 *	Note: Currently needs some work on threading, comments
 *
 *  Created on: Feb 15, 2014
 *      Author: Paula Rudy (paular@wpi.edu)
 */
#include "../include/Connection.h"

/**
 * This constructor creates and opens a new Connection.
 * The send and receive TCP connections are open separately
 * through the port numbers passed in as portNoSendToSet and
 * portNoRecieveToSet respectively. Both the send and receive
 * connections are made to the IP address passed in as
 * portNoSendToSet.
 * See Connection::openConnection(), Connection::openSend(),
 * and Connection::openReceive() for more details
 *
 * IMPORTANT NOTES:
 *
 * -> The receive connection will be opened by
 * listening on port number (portNoRecieveToSet + 1) for
 * incoming connection requests, but the finished receive
 * TCP connection will be through the port number passed in
 * as portNoSendToSet.
 * See Connection::openReceive() defined in this file for
 * more details.
 *
 * ->Opening the receive connection also spawns a thread to
 * buffer all incoming messages to a file (of the filename
 * = the IP address of the connection).
 * See Connection::openReceive() and receiveThreadFunction()
 * defined in this file for more details.
 *
 * See Connection::openConnection(), Connection::openSend(),
 * and Connection::openReceive() for more details
 */
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

/**
 * This constructor creates and opens a new Connection.
 * The details of the execution are the same as
 * Connection::Connection(int portNoSendToSet, int portNoRecieveToSet, char* IPAddressToSet)
 * as defined in this file, except portNoReceive is set to (portNoSendToSet +1).
 *
 * See Connection::Connection(int portNoSendToSet, int portNoRecieveToSet, char* IPAddressToSet)
 * as defined in this file for more detail.
 */
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

/**
 * This function opens both the send and receive
 * TCP connections for this Connection.
 * See  Connection::openSend(), and Connection::openReceive()
 * as defined in this file for more details.
 * This function will return 0 if both send and receive connections
 * are open, or 1 if either the send or receive connection
 * encounters an error during the opening process.
 *
 * IMPORTANT NOTES:
 *
 * -> The receive connection will be opened by
 * listening on port number (portNoReceive + 1) for
 * incoming connection requests, but the finished receive
 * TCP connection will be through the port number passed in
 * as portNoReceive.
 * See Connection::openReceive() defined in this file for
 * more details.
 *
 * ->Opening the receive connection also spawns a thread to
 * buffer all incoming messages to a file (of the filename
 * = the IP address of the connection).
 * See Connection::openReceive() and receiveThreadFunction()
 * defined in this file for more details.
 *
 */
int Connection::openConnection() {
	if (!(isOpen())) {//Only if we are not fully connected should we try to open any new connections.

		if (!openBoolReceive)//Only if the receive TCP connection is not open should we attempt to open it
			int rvReceive = openReceive(); //Open the receive TCP connection. openReceive must be called first to accept incoming connections.

		if (!openBoolSend)//Only if the send TCP connection is not open should we attempt to open it
			int rvSend = openSend();//Open the send TCP connection.

		if ((rvSend == 1) || (rvReceive == 1))//If either the call to openReceive() or openSend() ends with errors (indicated by a return value of 1)...
			return 1;
		else
			return 0;
	}
	return 0;
}

/**
 * This function closes both the send and receive
 * TCP connections for this Connection.
 * See  Connection::closeSend(), and Connection::ocloseReceive()
 * as defined in this file for more details.
 * This function will return 0 if both send and receive connections
 * are closed successfully, or 1 if either the send or receive connection
 * encounters an error during the closing process.
 *
 * IMPORTANT NOTES:
 *
 * ->The receive connection has a thread to
 * buffer all incoming messages to a file (of the filename
 * = the IP address of the connection), so it must wait for
 * the receive thread to exit gracefully.
 * See Connection::closeReceive() and receiveThreadFunction()
 * defined in this file for more details.
 *
 */
int Connection::closeConnection() {

	if (openBoolReceive)//Only if the receive connection is currently open should we attempt to close it
		int rvReceive = closeReceive();
	if (openBoolSend)//Only if the send connection is currently open should we attempt to close it
		int rvSend = closeSend();

	if ((rvSend == 1) || (rvReceive == 1))//If either of the calls to close a connection return with errors....
		return 1;

	openBool = false;//Set the openBool to indicate a successful closure
	return 0;
}

/**
 * Opens (creates and connects) the send tcp socket for this Connection if it isn't already open.
 * The connection opened is a TCP socket made to the IP address stored in the calling Connection's IPAddress variable.
 * The connection is made via the port number stored in the calling Connection's portNoSend variable.
 * Returns 1 if it encounters an error (it will print an error message to stderr if this happens),
 * 0 if everything works.
 */
int Connection::openSend() {

	//Only if the send connection isn't open should you try to create and connect the send socket
	if (!openBoolSend) {

		char *port;//This will hold the port number to open the send connection from (set from the connection's portNoSend variable)
		sprintf(port, "%d", portNoSend); //This is a work around to allow a dynamic port number. This sets port to the value in the connection's portNoSend variable.

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

			break;//Connection was successful, so we don't have to try another address
		}

		if (iteratorForListOfServerSocketAddr == NULL) {//If we have iterated over the list of addrinfo ("listOfServerSocketAddr") and failed to successfully connect to any of the addresses...
			fprintf(stderr, "Connection::openSend(): failed to connect\n");
			return 1;
		}

		freeaddrinfo(listOfServerSocketAddr);//We're all done with this structure, so free the memory
	}

	openBoolSend = true;//Set the openBoolSend variable to indicate the send connection is now open
	return 0;
}

/**
 * Opens (creates and connects) the receive tcp socket for this Connection if it isn't already open.
 * The connection opened is a TCP socket made to the IP address stored in the calling Connection's IPAddress variable.
 * The connection is made via listening on 1+ the port number stored in the calling Connection's portNoReceive variable.
 * The accepted incoming connection will be a TCP socket opened on the port number stored in the calling Connection's
 * portNoReceive variable.
 * A thread is spawned to handle all incoming messages- messages will be written to a file with the filename of the
 * IP address it is connected to.
 * Returns 1 if it encounters an error (it will print an error message to stderr if this happens),
 * 0 if everything works.
 */
int Connection::openReceive() {
	//Only if the receive connection isn't open should you try to create and connect the receive socket
	if (!openBoolReceive) {
		char *port;//This will hold the port number to listen to incoming connection requests from (set from the connection's portNoReceive variable + 1)
		sprintf(port, "%d", (portNoReceive + 1)); //This is a work around to allow a dynamic port number. This sets port to the value in the connection's portNoReceive variable + 1.

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
				continue;
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

		//Fork off a new process to handle the newly accepted connection
		receiveThread = std::thread(receiveThreadFunction,
				&flagToReceiveThread, sockfdReceive, &flagToReceiveThreadMutex,
				&mutexForBufferFile, IPAddress);

	}

	openBoolReceive = true;//Indicate the connection is now open
	return 0;
}

/**
 * This is the code executed by the thread spawned to handle all incoming messages
 * on the Connection's receive connection.
 * The thread (stored in the Connection's receiveThread variable) tries to pull as much data
 * through the socket passed in as sockfdReceive to fill a maximum size message
 * (see message.h). When it has received as much data as it can (up to max message size),
 * it appends that data to the end of a file with the filename of the IP address of the connection
 * (passed in as IPAddress). Before each new message is received, it checks to see if it should
 * end by checking the flagToReceiveThread boolean. This ensures that it is not suspended
 * in the middle of a function call. When the flagToReceiveThread boolean is seen to =
 * true, it closes the socket and exits.
 * The thread will print a message to stderr, close the socket, and exit with value 1 if it
 * encounters an error with the recv() function.
 */
void receiveThreadFunction(bool *flagToReceiveThread, int sockfdReceive,
		std::mutex *flagToReceiveThreadMutex, std::mutex *mutexForBufferFile,
		char* IPAddress) {

	std::ofstream bufferFileStream;
	(*flagToReceiveThreadMutex).lock();//Make sure to lock the flagToReceiveThreadMutex to ensure we see the right value

	while (!(*flagToReceiveThread)) {//Check the flagToReceiveThread boolean to see if we should clean up and exit
		(*flagToReceiveThreadMutex).unlock();//Unlock the flagToReceiveThreadMutex to allow others to use it

		int numBytesRecieved;//This will hold the number of bytes successfully received through the socket (up to the maximum message size in bytes)
		char buffer[999999];//This is a buffer to hold the incoming message//TODO: put real value as max data size

		//Try to pull no more than the maximum message size in bytes through the socket into the buffer to grab the message
		if ((numBytesRecieved = recv(sockfdReceive, buffer, 999999, 0)) == -1) {//If it fails...//TODO: put real value as max data size
			perror("receiveThreadFunction: recv()");
			close(sockfdReceive);//This child process is done, so we don't need this anymore
			exit(1);
		}

		(*mutexForBufferFile).lock();//Make sure to lock the mutexForBufferFile so others can't read it while we're writing to it
		bufferFileStream.open(IPAddress, std::ios::out | std::ios::app);//Open the file with the filename = IPAddress (the IP address of the connection). If it does not exist yet, create it.

		bufferFileStream << std::string(buffer, (size_t) numBytesRecieved)
				<< std::endl;//Write the received message to the file

		bufferFileStream.close(); //Close the file so others can use it

		(*mutexForBufferFile).unlock();//Unlock the mutexForBufferFile so that others know the file is availible

	}

	close(sockfdReceive);//This child process is done, so we don't need this anymore
	exit(0);

}

//TODO: modify for threads?
int Connection::closeSend() {
	if (openBoolSend) {
		close(sockfdSend);
	}
	return 0;
}

/**
 * This closes the Receive TCP connection by signaling the receive thread
 * to stop by setting the flagToReceiveThread variable to true.
 */
int Connection::closeReceive() {
	if (openBoolReceive) { //No need to try to close the receive connection if it's not open
		flagToReceiveThreadMutex.lock();//Make sure to lock the flagToReceiveThreadMutex to ensure others will see our changes to flagToReceiveThread
		flagToReceiveThread = true;//Set the flagToReceiveThread boolean to tell the receive thread to clean up and stop execution
		flagToReceiveThreadMutex.unlock();//Unlock the flagToReceiveThreadMutex so others can view the new value of the flagToReceiveThread boolean
		receiveThread.join();//Wait for the receiveThread to finish cleaning up and stop execution
		openBoolReceive = false; //Indicate the receive connection has been closed
		openBool = false;//Indicate the Connection is no longer fully open
	}
	return 0;
}

/**
 * This function is used to update the value of the openBool variable.
 * It checks the values of openBoolSend and openBoolReceive,
 * and sets openBool to true only if both the send and receive connections
 * are open (indicated by openBoolSend & openBoolReceive both being 'true').
 * openBool is set to false if either the send and receive connections are
 * closed (indicated by openBoolSend or openBoolReceive being 'false').
 */
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

