#include "load_balancer.hpp"
#include "globals.hpp"
#include <algorithm>
#include <iostream>

LoadBalancer::LoadBalancer(Destination * myMaster)
{
    master = myMaster;
    totalRoundsTakenByReqs = 0;
    totalReqsCompleted = 0;
}

bool LoadBalancer::containsBlob(int potentialNum)
{
    for (int k = 0; k < NUM_SHOW_BLOBS; k++)
    {
        if (showBlobs[k] == potentialNum)
        {
            return true;
        }
    }
    return false;
}

void LoadBalancer::setShowBlobs(int numSlaves)
{
    //loop thru every slave
    for (int slv = 0; slv < numSlaves; slv++)
    {
        for (int slavNum = 0; slavNum < MAX_SERVER_STORAGE; slavNum++)
        {
            if (slave[slv]->showBlobs[slavNum] != -1 && !this->containsBlob(slave[slv]->showBlobs[slavNum]))
            {
                for (int i = 0; i < MAX_SERVER_STORAGE; i++)
                {
                    if (showBlobs[i] == -1)
                    {
                        showBlobs[i] = slave[slv]->showBlobs[slavNum];
                        break;
                    }
                }
            }
        }
    }
}

void LoadBalancer::update()
{
    //Call the updates of all its slaves
    for (int b = 0; b < slave.size(); b++)
    {
        (slave[b])->update();
    }

    int currRoundWorkUnits = LOAD_BALANCE_WORK;

    for (int s = 0; s < slave.size(); s++)
    {
        for (int j = 0; j < NUM_SHOW_BLOBS; j++)
        {
            if (slave[s]->currLoad <= roundLoadDict[slave[s]->showBlobs[j]].first)
            {
                roundLoadDict[slave[s]->showBlobs[j]].first = slave[s]->currLoad;
                roundLoadDict[slave[s]->showBlobs[j]].second = slave[s];
            }
        }
    }

    for (int x = 0; x < NUM_SHOW_BLOBS; x++)
    {
        std::cout << x << " " << roundLoadDict[x].second << "\n";
    }


    while (currRoundWorkUnits > 0 && !requestQueue.empty())
    {
        //Reroute requests to appropriate location, outgoing respones and incoming requests
        Request * currReq = requestQueue.front();
        if (!currReq->outgoing)
        {
            for (int k = 0; k < MAX_BLOB_ACCESSES_PER_REQUEST; k++)
            {
                if (currReq->shows[k] != -1)
                {
                    requestQueue.pop_front();
                    auto foo = currReq->shows[k];
                    auto bar = roundLoadDict[foo];
                    auto fred = bar.second;
                    auto lafs = fred->requestQueue;
                    lafs.push_back(currReq);
                    //roundLoadDict[currReq->shows[k]].second->requestQueue.push_back(currReq);
                    currRoundWorkUnits--;
                    break;
                }
            }
        }
        else
        {
            if (master != NULL) //No work cost, YET TODO
            {
                master->requestQueue.push_front(currReq);
            }
            else //The current load balancer is the masterLoadBalancer, Maybe useful if we want MasterLoadBalancer to track average time per request
            {
                totalRoundsTakenByReqs += currReq->roundCount;
                totalReqsCompleted++;
            }
        }
        requestQueue.pop_front();
    }

    //Calculate currLoad
    //for (int m = 0; m < MAX_NUM_SLAVES; m++)
    for (int m = 0; m < slave.size(); m++)
    {
        currLoad += slave[m]->requestQueue.size();
    }
    currLoad += requestQueue.size();
}
