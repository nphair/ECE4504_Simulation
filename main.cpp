#include "main.hpp"
#include "globals.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr
            << "usage: " << argv[0] << " server_number servers_per_rack storage_per_server\n"
            << "\tserver_number:      the amount of servers to run the simulation with.\n"
            << "\tservers_per_rack:   the amount of servers each rack contains.\n"
            << "\tstorage_per_server: the number of 'blobs' each server will hold. Each Blob is 2 TB"
            << std::endl;
        exit(0);
    }
    else if (atoi(argv[3]) != 1 && atoi(argv[3]) != 2 && atoi(argv[3]) != 4 && atoi(argv[3]) != 8)
    {
        std::cerr << "Number of Blobs must be of the set {1, 2, 4, 8}";
        exit(0);
    }
    else if (atoi(argv[2]) > 46)
    {
        std::cerr << "A Rack can hold at most 46 (48 slots, 2 network devices)";
        exit(0);
    }
    const int serverNum = atoi(argv[1]);
    const int serversPerRack = atoi(argv[2]);
    const int serverStorage = atoi(argv[3]); //In Blobs

    //We are keeping number of racks per cluster always the same at 64
    const int numClusters = 1 + serverNum / (serversPerRack * MAX_NUM_SLAVES);
    int blobIter = 0;

    std::cout << "# of Servers: " << serverNum << "\n";
    std::cout << "# of Racks: " << MAX_NUM_SLAVES * numClusters << "\n";
    std::cout << "# of Load Balancers: " << 1+numClusters << "\n";
    std::cout << "# of Clusters: " << numClusters << "\n";


    std::cout << "WSC Initialization Started...\n";
    LoadBalancer * masterLoadBalancer = new LoadBalancer(NULL);

    for (int i = 0; i < numClusters; i++)
    {
        //Cluster Level LoadBalancers
        masterLoadBalancer->slave[i] = new LoadBalancer(masterLoadBalancer);
        LoadBalancer * currCluster = ((LoadBalancer*)(masterLoadBalancer->slave[i]));
        for (int j = 0; j < MAX_NUM_SLAVES; j++)
        {
            //Rack Level Load Balancers
            currCluster->slave[j] = new LoadBalancer(currCluster);
            LoadBalancer * currRack = ((LoadBalancer*)(currCluster->slave[j]));

            for (int k = 0; k < serversPerRack; k++)
            {
                currRack->slave[k] = new Server(currRack, SERVER_WORK, serverStorage);
                ((Server*)currRack->slave[k])->setShowBlobs(NUM_SHOW_BLOBS, (blobIter) % NUM_SHOW_BLOBS);
                blobIter += serverStorage;
            }
            currRack->setShowBlobs(serversPerRack);
        }
        currCluster->setShowBlobs(MAX_NUM_SLAVES);
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

    double roundsPerRequest = masterLoadBalancer->totalRoundsTakenByReqs / masterLoadBalancer->totalReqsCompleted;
    std::cout << "Average Time per Request: " << roundsPerRequest * 0.001 << "\n";
    double totalCost = (serverNum * SERVER_COST) + ((MAX_NUM_SLAVES * numClusters) * RACK_COST) + (serverNum * serverStorage * STORAGE_BLOB_COST) + (LB_COST * (1 + numClusters));
    std::cout << "Total Cost: " << totalCost << "\n";

    return 0;
}

Request * requestGen(LoadBalancer * master, std::vector<Request*> requestList, const int numReqs)
{
    Request * newRequests[numReqs];
    for (int currReq = 0; currReq < numReqs; currReq++)
    {
        std::array<int, MAX_BLOB_ACCESSES_PER_REQUEST> requestedBlobs;
        //Broswer Request
        if (rand() % 100 <= 7)
        {
            int numBlobs = rand() % (MAX_BLOB_ACCESSES_PER_REQUEST + 1);
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
