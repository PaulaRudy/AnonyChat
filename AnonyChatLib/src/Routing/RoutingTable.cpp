//TODO: Put a header comment here
#include "RoutingTable.h"
#include "NeighborList.h"
#include "Connection.h"
#include "message.h"

//TODO: Put a function header comment here
RoutingTable::RoutingTable(void) {
//TODO: Put code here to initalize the routing table from a file or make a new table if that file is not found
}

/**
 * Function to add or update an entry in the routing table.
 *
 * If an entry with the given VID (virtualAddress) does not exist, it will
 * create one with the VID and the utility count/IP address pair (toAdd)
 * and add it to the routing table.
 *
 * If an entry with the given VID does exist in the routing table,
 * but no entry with that given utility count/IP address pair (toAdd)
 * is associated with that VID, it will create one with the VID and
 * the utility count/IP address pair (toAdd) and add it to the table.
 *
 * If an entry with the given VID does exist in the routing table,
 * and there exists an utility count/IP address pair associated with
 * that VID, but the utility count of that NeighborUtilCountPair
 * is greater than the  utility count of the NeighborUtilCountPair
 * passed in, it will set the existing pair's utility count to match
 * the one passed in as toAdd.
 *
 * If an entry with the given VID does exist in the routing table,
 * and there exists an utility count/IP address pair associated with
 * that VID, but the utility count of that NeighborUtilCountPair
 * is NOT greater than the  utility count of the NeighborUtilCountPair
 * passed in, it will not modify the table.
 *
 * Takes:
 * 		virtualAddress
 * 			A long long unsigned representing the virtual address of the destination
 * 			whose entry in the routing table is to be updated
 * 		toAdd
 * 			A utility count/IP address pair (a "NeighborUtilCountPair"- see
 * 			RoutingTable.h for more information). The IP address of this
 * 			pair represents the routing path through the neighbor with the
 * 			given IP address. The utility count represents the lowest observed
 * 			utility count of messages recieved through that neighbor with the
 * 			VID "virtualAddress".
 */
void RoutingTable::UpdateTableEntry(long long unsigned virtualAddress, NeighborUtilCountPair toAdd){

	std::multimap<long long unsigned, NeighborUtilCountPair>::iterator it = table.find(virtualAddress);//Find the first entry in the routing table with the given virtual address

	// If there was no entry in the routing table associated with the specified destination address
	if (it == table.end())
		table.insert(std::pair<long long unsigned, NeighborUtilCountPair>(virtualAddress,toAdd));//Make an entry with the given values and put it in the table
	else{//We found an entry associated with this destination virtual address in the table

		//Look for entries for the given virtual address and the given IP address (passed in as toAdd's IPAddress variable)
		std::pair<
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator,
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator>
				itPair = table.equal_range(virtualAddress); //Fill a pair of iterators with pointers to the first and last entry in the table with the given virtual address

		std::multimap<long long unsigned, NeighborUtilCountPair>::iterator it; //Iterator to use to iterate over these entries

		bool found = false;//Flag to signal if we should stop searching because an entry with the virtual address/IP address has been found
		for (it = itPair.first; ((it != itPair.second) && (!found)); ++it){//Check each entry in the table with the given virtual address. Stop if we find one (shown by the found flag = true) or we've checked all entries with the given VID.
			if (it->second.IPAddress == toAdd.IPAddress){//If the IP address of this entry matches the one passed in with toAdd...

				if (it->second.utilityCount > toAdd.utilityCount)//...and the utility count of this entry is less than the one passed in with toAdd...
					it->second.utilityCount = toAdd.utilityCount;//...update this entry to reflect the new lowest utility count value

				found = true;//Tell the for loop to stop looking because we found it
			}
		}

		if (!found)//If we've checked all entries with the given VID and none of them have an associated IP address that matches the one passed in with toAdd...
			table.insert(std::pair<long long unsigned, NeighborUtilCountPair>(virtualAddress,toAdd));// Then we need to insert an entry that maps this VID to this IP address/utility count

		//If we found an entry for the given virtual address and IP address, but the utility count paired to that IP address is <= to the one passed in as toAdd's utility count,
		//...then we don't need to do anything to the table
	}
}


/**
 * This function removes all entries from the routing table
 * with the given virtual address (passed in as "virtualAddress").
 *
 * Returns:
 * 		0
 * 			If 1 or more entries for the given virtual address were
 * 			found and removed.
 * 		1
 * 			If no entries with the given virtual address were found
 * 			in the table.
 */
int RoutingTable::RemoveTableEntry(long long unsigned virtualAddress){

	std::multimap<long long unsigned, NeighborUtilCountPair>::iterator it =
			table.find(virtualAddress);//Find the first entry in the routing table with the given virtual address

	if (it == table.end())// If there was no entry in the routing table associated with the specified destination address ...
		return 1;//Return 1 to indicate failure, as we have checked all entries in the table and none match the virtualAddress
	else {

		std::pair<
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator,
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator>
				itPair = table.equal_range(virtualAddress);//Fill a pair of iterators with pointers to the first and last entry in the table with the given virtual address

		it = itPair.first; //Set the iterator to the first entry pointed to by the pair itPair

		for (; it != itPair.second; ++it)//For each entry in the table with the given virtual address.
			table.erase(it);//Remove this entry from the table

		return 0;//Return 0 to indicate sucess
	}
}

/**
 * This function removes only the entry in the routing table
 * with the given virtual address (passed in as "virtualAddress",
 *  IP address and utility count (both passed in as "toRemove"-
 *  see RoutingTable.h for more details on the NeighborUtilCountPair
 *  class). If such an entry is not found, the table is not altered.
 *
 *  Returns:
 *  	0
 *  		If the entry with the given virtual address and
 *  		IP address/Utility count was found and removed.
 *  	1
 *  		If no entry with the given virtual address and
 *  		IP address/Utility count was found.
 */
int RoutingTable::RemoveTableEntry(long long unsigned virtualAddress,
		NeighborUtilCountPair toRemove) {

	std::multimap<long long unsigned, NeighborUtilCountPair>::iterator it =
			table.find(virtualAddress);//Find the first entry in the routing table with the given virtual address


	if (it == table.end())// If there was no entry in the routing table associated with the specified destination address ...
		return 1;//Return 1 to indicate failure, as we have checked all entries in the table and none match the virtualAddress
	else {

		std::pair<
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator,
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator>
				itPair = table.equal_range(virtualAddress);//Fill a pair of iterators with pointers to the first and last entry in the table with the given virtual address

		it = itPair.first; //Set the iterator to the first entry pointed to by the pair itPair

		for (; it != itPair.second; ++it) {//Check each entry in the table with the given virtual address.
			if (it->second == toRemove) {//If we find a match to toRemove
				table.erase(it);//Remove this entry from the table
				return 0;//Return 0 to indicate sucess
			}
		}

		return 1;//Return 1 to indicate failure, as we have checked all entries in the table with the given virtual address and have not found a match to toRemove
	}

}

/**
 * This function fills the list of NeighborUtilCountPair passed in as
 * toFill with all the entries in the routing table matching the virtual
 * address passed in as virtualAddress.
 * Before returning, it sorts the list in ascending utility count order.
 *
 * Takes:
 * 		virtualAddress
 * 			A long long unsigned representing the virtual address to search
 * 			the routing table for.
 * 		toFill
 * 			A pointer to an empty std::list<NeighborUtilCountPair> to fill
 * 			with all entries in the routing table for the given virtual
 * 			address (in ascending utility count order). See RoutingTable.h
 * 			for more details on the NeighborUtilCountPair class.
 *
 * Returns:
 * 		0
 * 			If one or more entries for the given virtual address were found
 * 			in the routing table, and so the list was filled successfully
 * 		1
 * 			If no entries were found in the routing table for the given
 * 			virtual address.
 */
int RoutingTable::fillAndSortListOfRoutesForVirtualAddress(long long unsigned virtualAddress,
		std::list<NeighborUtilCountPair> *toFill) {

	std::multimap<long long unsigned, NeighborUtilCountPair>::iterator it =
			table.find(virtualAddress);//Find the first entry in the routing table with the given virtual address

	if (it == table.end())// If there was no entry in the routing table associated with the specified destination address ...
		return 1;//Return 1 to indicate failure, as we have checked all entries in the table and none match the virtualAddress
	else {//There is at least one entry in the routing table for the specified virtual address

		std::pair<
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator,
				std::multimap<long long unsigned, NeighborUtilCountPair>::iterator>
				itPair = table.equal_range(virtualAddress);//Fill a pair of iterators with pointers to the first and last entry in the table with the given virtual address

		for (it = itPair.first; it != itPair.second; ++it) {//For each entry in the table with the given virtual address...
			toFill->push_back(NeighborUtilCountPair(it->second));//...add it to the list passed by reference as "toFill"
		}

		toFill->sort();//Sort the list
		return 0;//Indicate success
	}

}


//TODO: finish this.
int RoutingTable::RouteMessage(Message toSend, char* srcNeighborIP) {
	Connection *sendConnection;
	std::multimap<long long unsigned, NeighborUtilCountPair>::iterator it;
	std::list<NeighborUtilCountPair> neighborList;
	std::vector<std::string>::iterator neighborIT;
	NeighborUtilCountPair *neighborPair;
	std::string neighborIP;
	int retval;

	// if the number of utility counters has become too large
	if (toSend.getUCounters() > UC_Limit) {
		// then we no longer want to route this message and will simply drop it
		// without doing anything else
		return 2;
	}

	// extract this message's neighbor pair information
	neighborPair->IPAddress = srcNeighborIP;
	neighborPair->utilityCount = toSend.getUCounters();

	// if we aren't over the UC_Limit we want to try adding this message's VID,
	// neighbor ip, and UC value to the RoutingTable
	UpdateTableEntry(toSend.getVID(true), *neighborPair);

	// if the utility counters associated with the message are acceptable
	// then we want to check if we know how to get to the destination VID
	it = table.find(toSend.getVID(false));

	// if the entry wasn't in the routing table then we want to broadcast it
	if (it == table.end()) {

		// for each neighbor in the neighbor list that isn't the srcNeighborIP
		for (neighborIT = TheNeighbors.list.begin();
				neighborIT != TheNeighbors.list.end(); neighborIT++) {

			// get the ip address of this neighbor
			neighborIP = *neighborIT;

			// if the neighborIP is our source neighbor skip this one
			if (neighborIP == srcNeighborIP) continue;

			// set up the connection to this neighbor
			sendConnection = new Connection(MESSAGEPORT, (char *) &neighborIP);

			// open a send connection
			retval = sendConnection->openSend();

			// if the connection was successfully opened send the message
			if (retval) sendConnection->send(toSend);

			// close the connection
			sendConnection->closeConnection();

		} // go to the next neighbor in the list

	// otherwise if the dest VID HAD an entry in the routing table
	} else { // we just choose the lowest UC neighbor associated in the table

		// sort the list of neighbors and utility counters for this virtual address
		this->fillAndSortListOfRoutesForVirtualAddress(toSend.getVID(false), &neighborList);

		// once we have the sorted neighbor list we open a connection to the first neighbor IP
		neighborIP = neighborList.front().IPAddress;

		// now that we know the IP address of the neighbor we're sending to
		// we can set up a connection with that neighbor
		sendConnection = new Connection(MESSAGEPORT, (char *) &neighborIP);

		// now that the connection is ready, we want to open a send connection
		retval = sendConnection->openSend();

		// as long as we haven't successfully opened a connection
		while (!!retval) {

			// we want to remove the unsuccessful connection from the list and table
			RemoveTableEntry(toSend.getVID(false), neighborList.front());
			neighborList.remove(neighborList.front());

			// if the neighbor list is now empty then we failed to find any working neighbors
			if (neighborList.empty()) return 1;

			// otherwise we need the new optimal neighbor
			neighborIP = neighborList.front().IPAddress;

			// we want to set up a connection again
			sendConnection = new Connection(MESSAGEPORT, (char *) &neighborIP);

			// we then want to attempt to open a connection to the next front
			retval = sendConnection->openSend();

		} // once we have successfully opened a connection to a neighbor

		// we want to actually send the message
		sendConnection->send(toSend);

		// then we want to close the connection and return
		sendConnection->closeConnection();
	}

	return 0;//Indicate success
}

//TODO: Put a function header comment here
RoutingTable::~RoutingTable(void) {
//TODO: Put code here to store the routing table to a file and clean up memory before exit
}
