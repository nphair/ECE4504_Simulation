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
        for (int slavNum = 0; slavNum < 8; slavNum++)
        {
            if (slave[slv]->showBlobs[slavNum] != -1 && !this->containsBlob(slave[slv]->showBlobs[slavNum]))
            {
                for (int i = 0; i < 8; i++)
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
    for (int b = 0; b < MAX_NUM_SLAVES; b++)
    {
        (slave[b])->update();
    }

    int currRoundWorkUnits = LOAD_BALANCE_WORK;

    //Formulate the LoadRoundDict-ionary
    //Looping thru every slave
    for (int g = 0; g < MAX_NUM_SLAVES; g++)
    {
        //Looping thru every element in service blob library
        for (int f = 0; f < NUM_SHOW_BLOBS; f++)
        {
            //Looping thru every element in the showBlobs[] in the server
            for(int i = 0; i < 8; i++)
            {
                //If server has this blob in its storage and its load is less than the current value stored in the dictionary
                if(slave[g]->showBlobs[i] == f && slave[g]->currLoad < roundLoadDict[f].first)
                {
                    roundLoadDict[f].first = slave[g]->currLoad;
                    roundLoadDict[f].second = slave[g];
                }
            }
        }
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
                    roundLoadDict[currReq->shows[k]].second->requestQueue.push_back(currReq);
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
    for (int m = 0; m < MAX_NUM_SLAVES; m++)
    {
        currLoad += slave[m]->requestQueue.size();
    }
    currLoad += requestQueue.size();
}
