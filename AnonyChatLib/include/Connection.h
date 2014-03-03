/*
 * Connection.h
 *
 * Header for the Connection class. See Connection.cpp for more details.
 *
 * The Connection class is used to create, mantain, and use a TCP
 * connection to a neighbor. Two separate TCP connections are maintained:
 * one devoted to sending messages, one devoted to receiving.
 *
 * The receiving TCP connection is managed by a thread that
 * buffers all input to a file named by the IP address of the
 * connection.
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
	std::mutex *flagToReceiveThreadMutex;//Mutex lock for flagToReceiveThread
	std::mutex *mutexForBufferFile;//Mutex lock for the file used to buffer messages received through this connection
	struct sigaction sa;//Used to reap dead processes
	std::thread receiveThread;//Used to buffer incoming messages to a file (see Connection::openReceive() and receiveThreadFunction() in Connection.cpp for more details)
	int closeSend();//Close the send TCP connection (see Connection.cpp for more details)
	int openReceive();//Open the receive TCP connection (see Connection.cpp for more details)
	int closeReceive();//Open the receive TCP connection (see Connection.cpp for more details)
	void checkState();//Used to update openBool (see Connection.cpp for more details)


public:
	Connection(int portNoSendToSet, int portNoRecieveToSet, char* IPAddressToSet, std::mutex *receiveThreadMutex, std::mutex *bufferFileMutex );
	Connection(int portNoSendToSet, char* IPAddressToSet, std::mutex *receiveThreadMutex, std::mutex *bufferFileMutex );
	Connection(char* IPAddressToSet, std::mutex *receiveThreadMutex, std::mutex *bufferFileMutex );
	bool isOpen(){return openBool;};
	int openSend();//Open the send TCP connection (see Connection.cpp for more details)
	int openConnection();//Used to open both the send and receive TCP connections (see Connection.cpp for more details)
	int closeConnection();//Used to close both the send and receive TCP connections (see Connection.cpp for more details)
	char* getIPAddress(){return IPAddress;};
	int* getSockReceive(){return &sockfdReceive;};
	int* getSockSend(){return &sockfdSend;};
	int getPortNoReceive(){return portNoReceive;};
	int getPortNoSend(){return portNoSend;};
	int sendMessage(Message toSend);//Used to send a message over an open send TCP connection (see Connection.cpp for more details)
	Message receiveMessage();//Used to extract a message from the file buffer used by the receive thread  (see Connection.cpp for more details)


};
//The function executed by the receive thread. (see Connection.cpp for more details)
void receiveThreadFunction(bool *flagToReceiveThread,int &sockfdReceive, std::mutex *flagToReceiveThreadMutex, std::mutex *mutexForBufferFile, char* IPAddress);

//Helper function: grabs the address stored in the sockaddr pointed to by sa, IPv4 *or* IPv6
void *getAddrFromSockaddr(struct sockaddr *sa);

//Helper function: handler used to reap dead processes
void sigchld_handler(int s);

#endif /* CONNECTION_H_ */
