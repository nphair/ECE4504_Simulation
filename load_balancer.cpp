#include "load_balancer.hpp"
#include <algorithm>
#include <iostream>

LoadBalancer::LoadBalancer(Destination * myMaster, int myWorkUnits)
{
    workUnits = myWorkUnits;
    master = myMaster;
    totalRoundsTakenByReqs = 0;
    totalReqsCompleted = 0;
}

void LoadBalancer::setShowBlobs(int numSlaves)
{

    //loop thru every slave
    for (int slv = 0; slv < numSlaves; slv++)
    {
        //loop thru every element in ShowBlobs in each slave
        for (int slvB = 0; slvB < 100; slvB++)
        {
            bool found = false;
            //Compare each element in ShowBlobs of the slaves to every element of show Blobs in the loadBalancer
            for (int lbB = 0; lbB < 100; lbB++)
            {
                (slave[slv])->showBlobs[slvB] == showBlobs[lbB];

                //Found showBlobs of the LoadBalancer
                if ((slave[slv])->showBlobs[slvB] == showBlobs[lbB])
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                for (int v = 0; v < 100; v++)
                {
                    if (showBlobs[v] == -1)
                    {
                        showBlobs[v] = (slave[slv])->showBlobs[slvB];
                    }
                }
            }

        }
    }

}

void LoadBalancer::update()
{
    //Call the updates of all its slaves
    for (int b = 0; b < 64; b++)
    {
        (slave[b])->update();
    }

    int currRoundWorkUnits = workUnits;

    //Formulate the LoadRoundDict-ionary
    bool found = false;
    //Looping thru every slave
    for (int g = 0; g < 64; g++)
    {
        //Looping thru every element in service blob library
        for (int f = 0; f < 100; f++)
        {
            //Looping thry every element in the showBlobs[] in the server
            for(int i = 0; i < 100; i++)
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



    while (currRoundWorkUnits > 0)
    {
        //Reroute requests to appropriate location, outgoing respones and incoming requests
        Request * currReq = requestQueue.front();
        if (!currReq->outgoing)
        {
            for (int k = 0; k < 10; k++) //10 is the max number of showblobs we have in a single request! TODO
            {
                //Determine which slave to give it to based on Round Dict TODO

            }
        }
        else
        {
            if (master != NULL) //No work cost, YET TODO
            {
                master->requestQueue.push_front(currReq);
            }
            else //The current load balancer is the masterLoadBalancer
            {
                totalRoundsTakenByReqs += currReq->roundCount;
                totalReqsCompleted++;
            }
        }
        requestQueue.pop_front();
    }

    //Calculate currLoad
    for (int m = 0; m < 64; m++)
    {
        currLoad += slave[m]->requestQueue.size();
    }
    currLoad += requestQueue.size();
}
