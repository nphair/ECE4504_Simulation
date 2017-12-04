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

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Not all proper variables included:\nExpected Input: SERVER_NUM SERVER_PER_RACK STORAGE_PER_SERVER(In Blobs)";
        exit(0);
    }

    int serverNum = atoi(argv[1]);
    int serversPerRack = atoi(argv[2]);
    int serverStorage = atoi(argv[3]); //In Blobs

    int loadBalanceWork = 10;
    int serverWork = 10;
    //How do we determine the number of clusters?
    int numClusters = 64;
    int numRacksPerCluster = 64;
    //How do we distribute the ShowBlobs across the system?
    //How many showBlobs do we have? <-- requires knowing how much data in shows there are and then the minimum data in a server at its smallest storage size
    const int showBlobs = 100;
    int * showBlobsArray = (int*) calloc(100, sizeof(int));

    LoadBalancer * masterLoadBalancer = new LoadBalancer(NULL, loadBalanceWork);
    std::cout << "Master Init\n";

    for (int i = 0; i < numClusters; i++)
    {
        std::cout << "Cluster " << i << "\n";

        //Cluster Level LoadBalancers
        masterLoadBalancer->slave[i] = new LoadBalancer(masterLoadBalancer, loadBalanceWork);
        LoadBalancer * currCluster = ((LoadBalancer*)(masterLoadBalancer->slave[i]));

        for (int j = 0; j < numRacksPerCluster; j++)
        {
            //Rack Level Load Balancers
            currCluster->slave[j] = new LoadBalancer(currCluster, loadBalanceWork);
            std::cout << "Rack " << j << "\n";
            LoadBalancer * currRack = ((LoadBalancer*)(currCluster->slave[i]));
            int currShowBlobArr[serverStorage];

            for (int k = 0; k < serversPerRack; k++)
            {
                std::cout << "Server" << k << "\n";
                currRack->slave[k] = new Server(currRack, serverWork, serverStorage);

                /*((Server*)currRack->slave[j])->setShowBlobs(showBlobsArray);*/
            }
        }
    }
    return 0;
}
