#include "server.hpp"

Server::Server(Destination * myMaster, int myWorkUnits)
{
    workUnits = myWorkUnits;
    master = myMaster;
}

void Server::update()
{
    int currRoundWorkUnits = workUnits;
    while (currRoundWorkUnits > 0)
    {
        //Satisfy a request currently in the queue
        //If finish or doesn't contain the requested Blob, then add to master queue (Front <-- but this may change)
    }
}
