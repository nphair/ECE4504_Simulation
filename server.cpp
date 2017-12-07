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
    std::cout << "enter update" << std::endl;
    int currRoundWorkUnits = workUnits;
    while (currRoundWorkUnits > 0 && !requestQueue.empty()) {
        std::cout << "enter loop" << std::endl;
        Request * request = requestQueue.front();
        std::cout << "after front" << std::endl;
        request->outgoing = process_request(request);
        master->requestQueue.push_front(request);
        std::cout << "b4" << std::endl;
        requestQueue.pop_front();
        std::cout << "after" << std::endl;
        currRoundWorkUnits--;
    }
    std::cout << "done update" << std::endl;

    currLoad = requestQueue.size();
}

void Server::setShowBlobs(const int totalShows, int startPos)
{
    for (int i = 0; i < storageAmt; i++)
    {
        showBlobs[i] = i+startPos;
    }
}
