/*
 * Connection.h
 *
 * Note: Currently needs some work on threading (see Connection.cpp), comments
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
#include "message.h"


class Connection {
private:
	int portNoReceive;//The port number of the TCP socket to recieve data from
	int portNoSend;//The port number of the TCP socket to send data through
	char* IPAddress;//The IP address of the connection
	int sockfdReceive;//The socket descriptor of the Receive connection
	int sockfdSend;//The socket descriptor of the Send connection
	bool openBool;//Whether the connections are open or not
	struct sigaction sa;//Used to reap dead processes
	int openSend();
	int closeSend();
	int openReceive();
	int closeReceive();

public:
	Connection(int portNoSendToSet, int portNoRecieveToSet, char* IPAddressToSet);
	Connection(int portNoSendToSet, char* IPAddressToSet);
	Connection(char* IPAddressToSet);
	bool isOpen(){return openBool;};
	int openConnection();
	int closeConnection();
	char* getIPAddress(){return IPAddress;};
	int* getSockReceive(){return &sockfdReceive;};
	int* getSockSend(){return &sockfdSend;};
	int getPortNoReceive(){return portNoReceive;};
	int getPortNoSend(){return portNoSend;};
	int send(Message toSend);
	Message receive();

};

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

Connection newConnection(int portNoSendToSet, int portNoRecieveToSet, char* IPAddressToSet);

void printConnection(Connection c);

bool operator == (const Connection &p1, const Connection &p2);

#endif /* CONNECTION_H_ */
