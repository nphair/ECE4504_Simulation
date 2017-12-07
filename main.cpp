#include "load_balancer.hpp"
#include "destination.hpp"
#include "server.hpp"
#include "request.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <list>
#include <numeric>
#include <random>

#define NUM_SHOW_BLOBS 100
#define LOAD_BALANCE_WORK 10
#define SERVER_WORK 10
#define TOTAL_ROUNDS 999
#define NUM_REQUESTS_GENERATED 125
#define NUM_RACKS_PER_CLUSTER 64

Request * requestGen(LoadBalancer*, std::vector<Request*>, const int);

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr
            << "usage: " << argv[0] << " server_number servers_per_rack storage_per_server\n"
            << "\tserver_number:      the number of servers to run the simulation with.\n"
            << "\tservers_per_rack:   the number of servers each rack contains.\n"
            << "\tstorage_per_server: the number of 'blobs' each server will hold."
            << std::endl;
        exit(0);
    }

    const int serverNum = atoi(argv[1]);
    const int serversPerRack = atoi(argv[2]);
    const int serverStorage = atoi(argv[3]); //In Blobs

    //We are keeping number of racks per cluster always the same at 64
    const int numClusters = 1 + serverNum / (serversPerRack * NUM_RACKS_PER_CLUSTER);
    int blobIter = 0;

    std::cout << "# of Servers: " << serverNum << "\n";
    std::cout << "# of Racks: " << NUM_RACKS_PER_CLUSTER * numClusters << "\n";
    std::cout << "# of Load Balancers: " << 1+numClusters << "\n";
    std::cout << "# of Clusters: " << numClusters << "\n";


    std::cout << "WSC Initialization Started...\n";
    LoadBalancer * masterLoadBalancer = new LoadBalancer(NULL, LOAD_BALANCE_WORK);

    for (int i = 0; i < numClusters; i++)
    {
        //Cluster Level LoadBalancers
        masterLoadBalancer->slave[i] = new LoadBalancer(masterLoadBalancer, LOAD_BALANCE_WORK);
        LoadBalancer * currCluster = ((LoadBalancer*)(masterLoadBalancer->slave[i]));
        for (int j = 0; j < NUM_RACKS_PER_CLUSTER; j++)
        {
            //Rack Level Load Balancers
            currCluster->slave[j] = new LoadBalancer(currCluster, LOAD_BALANCE_WORK);
            LoadBalancer * currRack = ((LoadBalancer*)(currCluster->slave[j]));

            for (int k = 0; k < serversPerRack; k++)
            {
                currRack->slave[k] = new Server(currRack, SERVER_WORK, serverStorage);
                ((Server*)currRack->slave[k])->setShowBlobs(NUM_SHOW_BLOBS, (blobIter) % NUM_SHOW_BLOBS);
                blobIter += serverStorage;
            }
            currRack->setShowBlobs(serversPerRack);

        }
        currCluster->setShowBlobs(NUM_RACKS_PER_CLUSTER);
    }
    std::cout << "WSC Initialization Completed.\n";

    std::vector<Request*> requestMasterList;
    for (int currRnds = 0; currRnds < TOTAL_ROUNDS; currRnds++)
    {
        requestGen(masterLoadBalancer, requestMasterList, NUM_REQUESTS_GENERATED);
        for (int t = 0; t < requestMasterList.size(); t++)
        {
            requestMasterList[t]->roundCount++;
        }
        masterLoadBalancer->update();
    }
    //Add our generated requests for the round to the back of the requestQueue of the masterLoadBalancerd
    //Keep master list of all requests in the system (actual objects)
    //Iterate every request roundCount in the master list by 1
    //Call masterLoadBalancer.update()
    //Loop again

    return 0;
}

Request * requestGen(LoadBalancer * master, std::vector<Request*> requestList, const int numReqs)
{
    Request * newRequests[numReqs];
    for (int currReq = 0; currReq < numReqs; currReq++)
    {
        std::array<int, 10> requestedBlobs;
        //Broswer Request
        if (rand() % 100 <= 7)
        {
            int numBlobs = rand() % (10 + 1);
            for (int n = 0; n < numBlobs; n++)
            {
                requestedBlobs[n] = rand() % NUM_SHOW_BLOBS;
            }
        }
        else //Show Request
        {
            requestedBlobs[0] = rand() % NUM_SHOW_BLOBS;
        }
        newRequests[currReq] = new Request(requestedBlobs);
        master->requestQueue.push_back(newRequests[currReq]);
        requestList.push_back(newRequests[currReq]);
    }
    return *newRequests;
}
