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

void LoadBalancer::setShowBlobs(int numSlaves, bool test)
{
    //loop thru every slave
    for (int slv = 0; slv < numSlaves; slv++)
    {

        for (int slavNum = 0; slavNum < NUM_SHOW_BLOBS; slavNum++)
        {
            if (test){
                slave[slv]->showBlobs[slavNum];

                this->containsBlob(slave[slv]->showBlobs[slavNum]);
            }
            if (slave[slv]->showBlobs[slavNum] != -1 && !this->containsBlob(slave[slv]->showBlobs[slavNum]))
            {
                    showBlobs[slave[slv]->showBlobs[slavNum]] = slave[slv]->showBlobs[slavNum];
            }
        }
    }
}

void LoadBalancer::update()
{
        //roundLoadDict.clear();



    //Call the updates of all its slaves
    for (int b = 0; b < slave.size(); b++)
    {
        (slave[b])->update();
    }

    int currRoundWorkUnits = LOAD_BALANCE_WORK;
    if (master == NULL)
    {
        currRoundWorkUnits = 1500;
    }

    for (int s = 0; s < slave.size(); s++ && requestQueue.size() > 0)
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

    for (int g = 0; g < requestQueue.size(); g++)
    {
        requestQueue[g]->roundCount++;
    }

    while (currRoundWorkUnits > 0 && !requestQueue.empty())
    {
        //Reroute requests to appropriate location, outgoing respones and incoming requests
        Request * currReq = requestQueue.front();
        if (!currReq->outgoing)
        {
            int misses = 0;

            for (int k = 0; k < MAX_BLOB_ACCESSES_PER_REQUEST; k++)
            {
                if (currReq->shows[k] != -1 && this->containsBlob(currReq->shows[k]))
                {
                    roundLoadDict[currReq->shows[k]].second->requestQueue.push_back(currReq);
                    requestQueue.pop_front();
                    currRoundWorkUnits--;
                    break;
                }
                else
                    misses++;
            }
            if (misses == 10)
            {
                requestQueue.pop_front();
                master->requestQueue.push_front(currReq);
            }
        }
        else
        {
            requestQueue.pop_front();

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

    }

    //Calculate currLoad
    for (int m = 0; m < slave.size(); m++)
    {
        currLoad += slave[m]->requestQueue.size();
    }
    currLoad += requestQueue.size();
}
