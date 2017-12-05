#include "server.hpp"
#include <iostream>

Server::Server(Destination * myMaster, int myWorkUnits, int myStorageAmt)
{
    workUnits = myWorkUnits;
    master = myMaster;
    storageAmt = myStorageAmt;
}
//how many racks per Cluster VS how many clusters in the system???? TODO
void Server::update()
{
    int currRoundWorkUnits = workUnits;
    while (currRoundWorkUnits > 0)
    {
        //Satisfy a request currently in the queue
        //If finish or doesn't contain the requested Blob, then add to master queue (Front <-- but this may change)
    }
    currLoad = requestQueue.size();
}

void Server::setShowBlobs(const int totalShows, int startPos)
{
    for (int i = 0; i < storageAmt; i++)
    {
        showBlobs[i] = i+startPos;
    }
}
