#include "server.hpp"
#include <iostream>

Server::Server(Destination * myMaster, int myWorkUnits, int myStorageAmt)
{
    workUnits = myWorkUnits;
    master = myMaster;
    storageAmt = myStorageAmt;
}

/**
 * Given a request determine if our server has all the shows in it.
 * This operation costs ~1 work unit.
 */
bool Server::process_request(Request * request)
{
    bool all_shows_in_request_found = true;
    auto sit = request->shows.begin();
    while(sit != request->shows.end()) {
        /* For each show in the request... */
        for(int i = 0; i < NUM_SHOW_BLOBS; i++) {
            /* For each show the server has... */
            if(showBlobs[i] == *sit) {
                *sit = -1;
                break;
            }
        }

        // Server does not have 1 or more of the shows in the request.
        if(*sit != -1) {
            all_shows_in_request_found = false;
        }
        sit++;
    }

    return all_shows_in_request_found;
}

/**
 *  Do as much work as possible in our allotted work units. Each
 *  iteration costs 1 work unit with the bulk of that cost coming
 *  from processing the request (process_request).
 */
void Server::update()
{

    int currRoundWorkUnits = workUnits;
    /*if (requestQueue.size() > 0)
        std::cout << "Request Queue length: " << requestQueue.size() << "\n";*/
    while (currRoundWorkUnits > 0 && !requestQueue.empty()) {
        Request * request = requestQueue.front();
        request->outgoing = process_request(request);
        //std::cout << "Outgoing " << request->outgoing << "\n";
        request->roundCount++;
        master->requestQueue.push_front(request);
        requestQueue.pop_front();
        currRoundWorkUnits--;
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
