/*
 * NeighborRequestConnection.cpp
 *
 *	This defines the class functions for the NeighborRequestConnection class,
 *	which is used to manage neighbor requests - to both listen on an open
 *	TCP connection for incoming neighbor requests (see openFrontDoor and
 *	listenThreadFunction both defined in this file), and to send a
 *	neighbor request (as a NeighborRequestPDU -see messagePDU.h) to a
 *	specific IP address.
 *	Please note that there are multiple threads being used here, and use
 *	appropriate caution.
 *
 *
 *	NOTE: This file not yet finished. Please see the TODO comments.
 *
 *  Created on: Feb 22, 2014
 *      Author: Paula Rudy (paular@wpi.edu)
 */
#include "NeighborRequestConnection.h"

/**
 * This constructor creates and opens a new TCP connection
 * (socket descriptor is frontDoorSockfd - see NeighborRequestConnection.h).
 * This connection will function as both a send and recieve connection for
 * NeighborRequestPDUs (see messagePDU.h).
 * This connection will be opened on port NEIGHBORREQUESTCONNECTIONLISTENPORT
 * (see NeighborRequestConnection.h).
 * This constructor also spawns a thread ("frontDoorThread" -
 * see NeighborRequestConnection.h) to receive and process NeighborRequestPDUs
 * (see messagePDU.h).
 * See the "listenThreadFunction" defined in this file for more details
 * on this thread.
 *
 * IMPORTANT NOTE:
 * ->The same TCP socket is used to both send and receive NeighborRequestPDUs
 * (see messagePDU.h), so the frontDoorThread must be signaled to clean up
 * and exit before it is used. This means you should never be attempting to
 * use the open TCP connection outside of the
 * NeighborRequestConnection::useFrontDoor() function defined in this file.
 *
 * See  NeighborRequestConnection.h for more details.
 */
NeighborRequestConnection::NeighborRequestConnection(){
	flagToFrontDoorThread = false;
	openFrontDoor(NULL);

	//Fork off a new thread to continuously listen for and handle new NeighborRequestConnections
	frontDoorThread = std::thread(listenThreadFunction, &flagToFrontDoorThread,
			frontDoorSockfd, &flagToFrontDoorThreadMutex);
}

/**
 * This function is used to create and open the "frontDoor";
 *  a TCP connection used to both a send and receive NeighborRequestPDUs
 *  (see messagePDU.h).
 * The socket descriptor for this connection is frontDoorSockfd -
 * see NeighborRequestConnection.h.
 * This connection will be opened on port NEIGHBORREQUESTCONNECTIONLISTENPORT
 * (see NeighborRequestConnection.h).
 *
 * When called with "NULL" passed in as the "IPAddress" agrument, this
 * function opens the connection to listen for and receive all incoming
 * connection requests.
 *
 * When called with the "IPAddress" agrument containing an IP address, this
 * function opens the connection to a specific IP address, to be used to send
 * a NeighborRequestPDU (see messagePDU.h) to a specific IP address using the
 * useFrontDoor method defined in this file.
 *
 * This function returns 1 upon error, 0 for sucess.
 */
int NeighborRequestConnection::openFrontDoor(char* IPAddress) {

	struct addrinfo prepInfo;//Used to store information on the criteria for selecting the socket address structures returned in the listOfServerAddr.
	struct addrinfo *listOfServerSocketAddr;//The list of appropriate socket address structures available
	struct addrinfo *iteratorForListOfServerSocketAddr;//Used to iterate through listOfServerSocketAddr
	int yes = 1;//Used to set socket options
	int returnValueFromgetaddrinfo;//Used to store a return value from a call to getaddrinfo

	char *port;//This will hold the port number to listen to incoming connection requests from (set from the connection's portNoReceive variable + 1)
	sprintf(port, "%d", NEIGHBORREQUESTCONNECTIONLISTENPORT); //This is a work around to allow a dynamic port number. This sets port to the value in the connection's portNoReceive variable + 1.

	//Set up prepInfo to say what kind of socket we want (prepInfo specifies the criteria for selecting the socket address structures returned in the listOfServerAddr)
	memset(&prepInfo, 0, sizeof prepInfo);
	prepInfo.ai_family = AF_UNSPEC; // *either* IPv4 or IPv6
	prepInfo.ai_socktype = SOCK_STREAM;//only use TCP
	prepInfo.ai_flags = AI_PASSIVE; //use my IP a

	//Set up the list of appropriate socket address structures available
	if ((returnValueFromgetaddrinfo = getaddrinfo(IPAddress, port, &prepInfo,
			&listOfServerSocketAddr)) != 0) {//If it fails...
		fprintf(stderr, "NeighborRequestConnection::openFrontDoor: getaddrinfo: %s\n",
				gai_strerror(returnValueFromgetaddrinfo));//error setting up listOfServerSocketAddr
		return 1;
	}

	//Loop through all the results in listOfServerSocketAddr, create a socket and bind to the first address we can
	for (iteratorForListOfServerSocketAddr = listOfServerSocketAddr; iteratorForListOfServerSocketAddr
			!= NULL; iteratorForListOfServerSocketAddr
			= iteratorForListOfServerSocketAddr->ai_next) {

		//Try to create the socket
		if ((frontDoorSockfd = socket(
				iteratorForListOfServerSocketAddr->ai_family,
				iteratorForListOfServerSocketAddr->ai_socktype,
				iteratorForListOfServerSocketAddr->ai_protocol)) == -1) { //If it fails...
			perror("NeighborRequestConnection::openFrontDoor: socket()");//print error to error stream
			continue;
		}

		if (IPAddress == NULL) {//If we are opening this socket to receive incoming connections...
			//Try to set the socket options
			if (setsockopt(frontDoorSockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
					sizeof(int)) == -1) {//If it fails...
				perror("NeighborRequestConnection::openFrontDoor: setsockopt()");
				return 1;
			}

			//Try to bind the socket (assign the address contained in the addrinfo structure pointed to by iteratorForListOfServerSocketAddr to the socket)
			if (bind(frontDoorSockfd,
					iteratorForListOfServerSocketAddr->ai_addr,
					iteratorForListOfServerSocketAddr->ai_addrlen) == -1) {//If it fails...
				close(frontDoorSockfd);//close the socket
				perror("NeighborRequestConnection::openFrontDoor: bind()");
				continue;
			}

		} else {//We are initiating a TCP connection to a specific IP address to send a NeighborRequestPDU
			//Try to connect to the address stored in this addrinfo, over the newly created socket
			if (connect(frontDoorSockfd,
					iteratorForListOfServerSocketAddr->ai_addr,
					iteratorForListOfServerSocketAddr->ai_addrlen) == -1) {//If it fails...
				close(frontDoorSockfd);
				perror("NeighborRequestConnection::openFrontDoor: connect()");
				continue;
			}
		}


		//Socket successfully bound or connected
		break;
	}

	if (iteratorForListOfServerSocketAddr == NULL) {//If we have iterated over the list of addrinfo ("listOfServerSocketAddr") and failed to successfully bind a socket or open a connection to any of them...
		if (IPAddress == NULL) //If we were opening this socket to receive incoming connections...
			fprintf(stderr, "NeighborRequestConnection::openFrontDoor: failed to bind\n");
		else//We were initiating a TCP connection to a specific IP address to send a NeighborRequestPDU
			fprintf(stderr, "NeighborRequestConnection::openFrontDoor: failed to connect\n");
		return 1;
	}

	freeaddrinfo(listOfServerSocketAddr);//We're all done with this structure, so free the memory

	return 0;
}

/**
 * This is the function executed by the frontDoorThread.
 * It continuously listens on the open TCP connection
 * (passed in as the "frontDoorSockfd" socket descriptor),
 * forking off new processes to recieve and process incoming
 * NeighborRequestPDUs (see message.h).
 * This thread is signaled to clean up and exit by setting the
 * flagToFrontDoorThread boolean to true.
 */
void listenThreadFunction(bool *flagToFrontDoorThread, int frontDoorSockfd,
		std::mutex *flagToFrontDoorThreadMutex) {

	//Try to listen on "frontDoorSockfd" for incoming connection requests. These requests are queued in a queue of max length "MAXWAITINGCLIENTS"
	if (listen(frontDoorSockfd, 15) == -1) {//If it fails...
		perror("server: listen()");
		close(frontDoorSockfd);
		exit(1);
	}

	struct sigaction sa;//Used to reap dead processes
	// Try to reap all dead processes
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {//If it fails...
		perror("server: sigaction()");
		close(frontDoorSockfd);
		exit(1);
	}

	//Print a message to the console indicating a successful set up
	printf("Server is now waiting for connections...\n  \n");

	struct sockaddr_storage clientAddress;//Client's address information
	char clientAddressString[INET6_ADDRSTRLEN];//Used to store a calling client's IP address as a string

	(*flagToFrontDoorThreadMutex).lock();//Make sure to lock the flagToFrontDoorThreadMutex to ensure we see the right value

	while (!(*flagToFrontDoorThread)) {//Check the flagToFrontDoorThread boolean to see if we should clean up and exit
		(*flagToFrontDoorThreadMutex).unlock();//Unlock the flagToFrontDoorThreadMutex to allow others to use it

		//Open a new socket ("sockfd") for the first connection request on the listen queue
		socklen_t sin_size = sizeof clientAddress;
		int sockfd = accept(frontDoorSockfd,
				(struct sockaddr *) &clientAddress, &sin_size);
		if (sockfd == -1) {//If failure accept()ing:
			perror("server: accept()");
			close(frontDoorSockfd);
			close(sockfd);
			exit(1);
		}

		//Find and store the calling client's IP address to a string
		inet_ntop(clientAddress.ss_family,
				getAddrFromSockaddr((struct sockaddr *) &clientAddress),
				clientAddressString, sizeof clientAddressString);

		//Print a message to the console to indicate a connection has been received
		printf("Server: got connection from %s\n", clientAddressString);

		//Fork off a new process to handle the newly accepted connection
		if (!fork()) {

			close(frontDoorSockfd); //The child process doesn't need the listener


			int numBytesRecieved;//Used to store the number of bytes received from the client

			char buffer[sizeof(NeighborRequestPDU)];

			if ((numBytesRecieved = recv(sockfd, buffer, sizeof(struct NeighborRequestPDU), 0)) == -1) {//If it fails...
				perror("NeighborRequestConnection::useFrontDoor: receive NeighborRequestPDU");
				exit(1);
			}

			NeighborRequestPDU toRecieve;
			//TODO: way to convert char buffer to NeighborRequestPDU
			//TODO: code to process this NeighborRequestPDU

			close(sockfd);//This child process is done, so we don't need this anymore

			exit(0);//Close this child process
		}

		close(sockfd);//We're all done, so the parent process doesn't need this anymore

	}

	exit(0);
}

/**
 * This function is used to send a NeighborRequestPDU (see message.h) to
 * a specific IP address (passed in as "IPAddress"). Because the same port
 * is used to send and receive a NeighborRequestPDU, this function first
 * signals the frontDoorThread to clean up and exit before closing the
 * existing TCP connection and reopening it to a specific IP address
 * (using the openFrontDoor method defined in this file).
 * After both sending a NeighborRequestPDU and receiving a response
 * (in the form of a NeighborRequestPDU), this function closes the
 * TCP connection, reopens it to receive incoming connections, and
 * restarts the frontDoorThread.
 *
 * Returns 1 if an error is encountered, 0 upon sucessful compleation.
 */
int NeighborRequestConnection::useFrontDoor(NeighborRequestPDU toSend,
		NeighborRequestPDU *toReceive,char* IPAddress ) {

	flagToFrontDoorThreadMutex.lock();//Make sure to lock the flagToFrontDoorThreadMutex to ensure others see the right value
	flagToFrontDoorThread = true; //set the flagToFrontDoorThread boolean to signal the frontDoorThread to clean up and exit
	flagToFrontDoorThreadMutex.unlock();//Unlock the flagToFrontDoorThreadMutex to allow others to see it

	frontDoorThread.join(); //Wait for the frontDoorThread to clean up and exit

	close(frontDoorSockfd); //Close the front door TCP connection so we can reopen it to a specific IP address

	openFrontDoor(IPAddress);//Open the front door TCP connection to the IP adress passed in as "IPAddress"

	char *toSendCharArray;
	//sprintf(toSendCharArray, "%d", toSend);//TODO: need a way to print a NeighborRequestPDU

	//Try to send the NeighborRequestPDU "toSend", now stored in the toSendCharArray
	if (send(frontDoorSockfd, toSendCharArray, sizeof(NeighborRequestPDU), 0) == -1) { //If it fails...
		perror("NeighborRequestConnection::useFrontDoor: send NeighborRequestPDU");
		return(1);
	}

	int numBytesRecieved;//Used to store the number of bytes received from the client

	char buffer[sizeof(NeighborRequestPDU)];//Used to buffer the incoming NeighborRequestPDU

	//Try to receive the incoming NeighborRequestPDU
	if ((numBytesRecieved = recv(frontDoorSockfd, buffer,sizeof(NeighborRequestPDU), 0)) == -1) {//If it fails...
		perror("NeighborRequestConnection::useFrontDoor: receive NeighborRequestPDU");
		return(1);
	}

	//TODO: way to convert char buffer to NeighborRequestPDU

	close(frontDoorSockfd); //Close the front door TCP connection so we can reopen it to allow incoming connections

	openFrontDoor(NULL);//Open the front door TCP connection to allow incoming connection requests

	flagToFrontDoorThread = false;//Reset this to false to allow the frontDoorThread to listen. We don't need to use the mutex because the frontDoorThread is not running.

	//Fork off a new thread to continuously listen for and handle new NeighborRequestConnections (re-spawn the frontDoorThread)
	frontDoorThread = std::thread(listenThreadFunction, &flagToFrontDoorThread,
			frontDoorSockfd, &flagToFrontDoorThreadMutex);

	return 0;
}

/**
 * This function is used to create and send a NeighborRequestPDU (see message.h) to
 * a specific IP address (passed in as "IPAddress") and receive and process a reply
 * NeighborRequestPDU using the private function "useFrontDoor" (defined in this file-
 * see definition for more details)
 *
 * Returns 1 if an error is encountered, 0 upon sucessful compleation.
 */
int NeighborRequestConnection::invite_neighbor(char * ipaddr, NeighborList list){
	int rv;
	struct NeighborRequestPDU requestToSend, requestToReceive;

	requestToSend.opcode = 0;
	requestToSend.src = 0;
	requestToSend.dest = 0;
	requestToSend.numNeighbors = list.NumberOfNeighbors();
	requestToSend.utilCounter = -1; // will denote an invitation
	requestToSend.n_addr = "127.0.0.1";//TODO: put code here to get node's IP address and put it here

	rv = useFrontDoor(requestToSend, &requestToReceive,requestToSend.n_addr);

	//TODO: put code here to handle neighbor request message received (in requestToReceive)
	return rv;
}

//TODO: Needs a real function header
//Used to broadcast a neighbor request on all active connections.
int NeighborRequestConnection::broadcastNeighborRequest() {
//TODO: Finish this function
}

//TODO: Needs a real function header
//Used to decide to accept a neighbor, ask the user, or discard a neighbor request passed in as newNeighbor.
bool NeighborRequestConnection::EvaluateNeighborRequest(NeighborRequestPDU newNeighbor) {
	// TODO: code for if we have already seen a neighbor request for this ipaddress in the last few minutes
/*
	// if our user has set the user trust level and we're below that threshold
	if (newNeighbor.numNeighbors < UserTrustLevel) {
		return false;
	} // otherwise we want to evaluate this request on its merits

	// if we do not have the desired minimum number of neighbors
	if (TheNeighbors.NumberOfNeighbors() < MINNEIGHBORS) {
		// we want to add this neighbor no matter what
		TheNeighbors.list.push_back(newNeighbor.n_addr);
		// then we want to return
		return true;
	}

	// otherwise if this node has less than the maximum number of neighbors
	else if (TheNeighbors.NumberOfNeighbors() < MAXNEIGHBORS) {
		// and if the Utility Counter value is over the threshold value
		if (newNeighbor.utilCounter > UTILBENCHMARK) {
			// then we want to add this neighbor
			TheNeighbors.list.push_back(newNeighbor.n_addr);
			// then we want to return
			return true;
		// otherwise we don't want to accept this neighbor
		} else return false;
	}

	// finally, if this node already has the maximum number of neighbors
	// and the Utility Counter value is above the limit
	else if (newNeighbor.utilCounter > MAX_UCS) {

		// then we want to dump our first neighbor
		TheNeighbors.list.pop_back();
		TheNeighbors.list.push_back(newNeighbor.n_addr);
		return true;
	}
	*/

	return false;
}

/**
 * This destructor is needed to signal the frontDoorThread to clean up and exit
 * before the NeighborRequestConnection is destroyed.
 */
NeighborRequestConnection::~NeighborRequestConnection() {
	flagToFrontDoorThreadMutex.lock();//Make sure to lock the flagToFrontDoorThreadMutex to ensure others see the right value
	flagToFrontDoorThread = true; //set the flagToFrontDoorThread boolean to signal the frontDoorThread to clean up and exit
	flagToFrontDoorThreadMutex.unlock();//Unlock the flagToFrontDoorThreadMutex to allow others to see it
	frontDoorThread.join(); //Wait for the frontDoorThread to clean up and exit
	close(frontDoorSockfd);
}

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
