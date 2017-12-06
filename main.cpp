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
int initialize();



int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr
            << "usage: " << argv[0] << " server_number servers_per_rack storage_per_server\n"
            << "\tserver_number:      the amount of servers to run the simulation with.\n"
            << "\tservers_per_rack:   the amount of servers each rack contains.\n"
            << "\tstorage_per_server: the number of 'blobs' each server will hold."
            << std::endl;
        exit(0);
    }

    int serverNum = atoi(argv[1]); //TODO actually use this number... TODO
    int serversPerRack = atoi(argv[2]);
    int serverStorage = atoi(argv[3]); //In Blobs

    int loadBalanceWork = 10;
    int serverWork = 10;
    //How do we determine the number of clusters?
    int numClusters = 64;
    int numRacksPerCluster = 64;
    //How do we distribute the ShowBlobs across the system?
    //How many showBlobs do we have? <-- requires knowing how much data in shows there are and then the minimum data in a server at its smallest storage size
    const int showBlobs = 100;//100 is the total number of show blobs we have in the service TODO
    int blobIter = 0;

    std::cout << "# of Servers: " << serverNum << "\n";
    std::cout << "# of Racks: " << numRacksPerCluster * numClusters << "\n";
    std::cout << "# of Load Balancers: " << 1+numClusters << "\n";
    std::cout << "# of Clusters: " << numClusters << "\n";


    std::cout << "WSC Initialization Started...\n";
    LoadBalancer * masterLoadBalancer = new LoadBalancer(NULL, loadBalanceWork);

    for (int i = 0; i < numClusters; i++)
    {
        //Cluster Level LoadBalancers
        masterLoadBalancer->slave[i] = new LoadBalancer(masterLoadBalancer, loadBalanceWork);
        LoadBalancer * currCluster = ((LoadBalancer*)(masterLoadBalancer->slave[i]));
        for (int j = 0; j < numRacksPerCluster; j++)
        {
            //Rack Level Load Balancers
            currCluster->slave[j] = new LoadBalancer(currCluster, loadBalanceWork);
            //std::cout << "Rack " << j << "\n";
            LoadBalancer * currRack = ((LoadBalancer*)(currCluster->slave[j]));

            for (int k = 0; k < serversPerRack; k++)
            {
                //std::cout << "Server" << k << "\n";
                currRack->slave[k] = new Server(currRack, serverWork, serverStorage);
                ((Server*)currRack->slave[k])->setShowBlobs(showBlobs, (blobIter) % showBlobs);
                blobIter += serverStorage;
            }
            currRack->setShowBlobs(5);
        }
        currCluster->setShowBlobs(64);
    }
    std::cout << "WSC Initialization Completed.\n";

    //Add our generated requests for the round to the back of the requestQueue of the masterLoadBalancerd
    //Keep master list of all requests in the system (actual objects)
    //Iterate every request roundCount in the master list by 1
    //Call masterLoadBalancer.update()
    //Loop again

    return 0;
}
