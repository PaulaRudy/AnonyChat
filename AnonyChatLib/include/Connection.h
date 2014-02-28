/*
 * Connection.h
 *
 * Header for the Connection.cpp file
 *
 *  Created on: Feb 15, 2014
 *      Author: Paula Rudy (paular@wpi.edu)
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <mutex>
#include "message.h"


class Connection {
private:
	int portNoReceive;//The port number of the TCP socket to recieve data from
	int portNoSend;//The port number of the TCP socket to send data through
	char* IPAddress;//The IP address of the connection
	int sockfdReceive;//The socket descriptor of the Receive connection
	int sockfdSend;//The socket descriptor of the Send connection
	bool openBool;//Whether both the receive and send connections are open (true) or closed (false)
	bool openBoolReceive;//Whether the receive connection is open or not
	bool openBoolSend;//Whether the send connection is open or not
	bool flagToReceiveThread;//Used to signal to receive thread that it should stop and join();
	std::mutex flagToReceiveThreadMutex;//Mutex lock for flagToReceiveThread
	std::mutex mutexForBufferFile;//Mutex lock for the file used to buffer messages received through this connection
	struct sigaction sa;//Used to reap dead processes
	std::thread receiveThread;//Used to buffer incoming messages to a file (see Connection::openReceive() and receiveThreadFunction() in Connection.cpp for more details)
	int closeSend();//Close the send TCP connection (see Connection.cpp for more details)
	int openReceive();//Open the receive TCP connection (see Connection.cpp for more details)
	int closeReceive();//Open the receive TCP connection (see Connection.cpp for more details)
	void checkState();//Used to update openBool (see Connection.cpp for more details)


public:
	Connection(int portNoSendToSet, int portNoRecieveToSet, char* IPAddressToSet);
	Connection(int portNoSendToSet, char* IPAddressToSet);
	Connection(char* IPAddressToSet);
	bool isOpen(){return openBool;};
	int openSend();//Open the send TCP connection (see Connection.cpp for more details)
	int openConnection();//Used to open both the send and receive TCP connections (see Connection.cpp for more details)
	int closeConnection();//Used to close both the send and receive TCP connections (see Connection.cpp for more details)
	char* getIPAddress(){return IPAddress;};
	int* getSockReceive(){return &sockfdReceive;};
	int* getSockSend(){return &sockfdSend;};
	int getPortNoReceive(){return portNoReceive;};
	int getPortNoSend(){return portNoSend;};
	int send(Message toSend);//Used to send a message over an open send TCP connection (see Connection.cpp for more details)
	Message receive();//Used to extract a message from the file buffer used by the receive thread  (see Connection.cpp for more details)

};
//The function executed by the receive thread. (see Connection.cpp for more details)
void receiveThreadFunction(bool *flagToReceiveThread,int sockfdReceive, std::mutex *flagToReceiveThreadMutex, std::mutex *mutexForBufferFile, char* IPAddress);

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

#endif /* CONNECTION_H_ */
