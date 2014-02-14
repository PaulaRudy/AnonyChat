#include <iostream>
#include <stdlib>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


// this is completely arbitrary right now
#define PORT 9191
#define NEIGHBORVALUE1 5
#define NEIGHBORVALUE2 10
#define NEIGHBORVALUE3 15
#define UTILBENCHMARK1 80
#define UTILBENCHMARK2 160

struct NeighborRequestPDU {
	unsigned opcode;
	long long unsigned source;
	long long unsigned dest;
	int utilCounters;
	unsigned neighbors;
	struct in_addr n_addr;
};

// above should be in header

//CONNECTTOHOST – Connects to a remote host
bool ConnectToHost(int PortNo, char* IPAddress, int* sockfd)
{
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection(int sockfd)
{
}

// above should go in a DIFFERENT header

/* int invite_neighbor(struct in_addr sin_addr) -- invites a target to become our neighbor
 *  creates socket with target and sends NeighborRequestPDU with our own
 *  requires:
		ConnectToHost() function capable of connecting to a host using a tcp socket
			takes as argument an int for a port number and an IP address as a char*
			returns a bool indicating if it succeeded in establishing a connection
		CloseConnection() function to clean up after and close a socket
			takes a SOCKET fd as an argument
			returns nothing
		NumberOfNeighbors() function that returns the number of neighbors this node has
			takes no arguments
		GetOwnIP() function that returns this node's IP address
			takes no arguments, returns IP address as a char*
		AddToNeighbors() function that takes a NeighborRequestPDU and decides whether or not
			to add the specified neighbor to our list of neighbors

			takes as an argument the NeighborRequestPDU
			returns a bool indicating if the neighbor was or was not added
 */

int invite_neighbor(char *ipaddr, int sockfd) {
	int rv, recbytes;
	struct NeighborRequestPDU thisNode, newNeighbor;

	thisNode.opcode = 0;
	thisNode.source = 0;
	thisNode.dest = 0;
	thisNode.neighbors = NumberOfNeighbors();
	thisNode.utilCounters = -1; // will denote an invitation
	thisNode.n_addr = inet_addr(GetOwnIP());

	rv = send(sockfd, (char *)&thisNode, sizeof(struct NeighborRequestPDU), 0);

	if (rv == -1) {
		// there was an error, not sure what we should do here
	}

	recbytes = recv(sockfd, (char *)&newNeighbor, sizeof(struct NeighborRequestPDU), 0);

	AddToNeighbors(newNeighbor);

	return 0; // for success
}

/* broadcastNeighborRequest() broadcasts a neighbor request to all nodes on the network
	takes nothing as an argument
	returns bool indicating success or failure of broadcast

	requires:
		SendToAddress() -- function that takes as an argument a virtual address, a string, and the length of the string
			and returns a boolean indicating if the send was successful or not
			this should be fine to do over TCP sockets but will require
			we figure out which direction each virtual address is in
		NumberOfNeighbors() -- as above
		GetOwnIP() -- as above

*/


int broadcastNeighborRequest() {
	struct NeighborRequestPDU newRequest;
	bool retval;

	newRequest.opcode = 0;
	newRequest.source = 0;
	newRequest.dest = 0;
	newRequest.neighbors = NumberOfNeighbors();
	newRequest.utilCounters = 0; // starts at 0 and NOT an invitation
	newRequest.n_addr = inet_addr(GetOwnIP());

	retval = SendToAddress(0xffffffff, newRequest, sizeof(struct NeighborRequestPDU));

	if (retval)
		return 0; // indicate success

	else
		return 1; // indicate failure

}



/* bool AddToNeighbors(struct NeighborRequestPDU newNeighbor) -- function to evaluate a neighbor request
	takes a NeighborRequestPDU as an argument
	returns a bool indicating whether or not the neighbor request was accepted
	requres:
		NumberOfNeighbors() -- as above
		UserTrustLevel -- a global indicating the minimum required number of neighbors a user has
			specified a node must have before accepting it as a neighbor, 0 by default
		functions to add to and remove from the neighbor list

*/

bool AddToNeighbors(struct NeighborRequestPDU newNeighbor) {
	if (newNeighbor.neighbors < UserTrustLevel)
		return false;

	// if the number of neighbors is less than or equal to our low benchmark
	// then we want to accept any new neighbors until we have grown our neighbor list
	if (NumberOfNeighbors() <= NEIGHBORVALUE1) {
		// TODO: code to add neighbor to list
		return true;
	}
	// otherwise if the number is below our second benchmark then we want to accept only
	// those neighbors with a sufficiently large utility counter field
	else if (NumberOfNeighbors() <= NEIGHBORVALUE2) {
		if (newNeighbor.utilCounters >= UTILBENCHMARK1) {
			// TODO: code to add neighbor to list
			return true;
		} else {
			return false;
		}
	}
	// otherwise if the number is below our third benchmark then we want to accept
	// relativley few neighbors
	else if (NumberOfNeighbors() <= NEIGHBORVALUE3) {
		if (newNeighbor.utilCounters >= UTILBENCHMARK2) {
			// TODO: code to add neighbor to list
			return true;
		} else {
			return false;
		}
	}
	// otherwise if the number is above our last benchmark then we need to REMOVE
	// an old neighbor from the list and replace them with this one if the
	// utility counter is sufficiently high that we would want this new node
	// as a neighbor
	else {
		if (newNeighbor.utilCounters >= UTILBENCHMARK2) {
			// TODO: code to add neighbor to list
			// TODO: code to remove a neighbor from our list
			//		IMPORTANT will likely require mutex protection to ensure nothing
			//		weird happens like trying to talk to a neighbor while we're deleting it
			return true;
		}

		// otherwise we don't want it
		return false;
	}
}
