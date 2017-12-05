#include "load_balancer.hpp"

LoadBalancer::LoadBalancer(Destination * myMaster, int myWorkUnits)
{
    workUnits = myWorkUnits;
    master = myMaster;
    totalRoundsTakenByReqs = 0;
    totalReqsCompleted = 0;
}

void LoadBalancer::update()
{
    //Call the updates of all its slaves
    for (int b = 0; b < 64; b++)
    {
        (slave[b])->update();
    }

    int currRoundWorkUnits = workUnits;

    //formulate the LoadRoundDict-ionary
    for (int g = 0; g < 64; g++)
    {
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
    for (int m = 0; m < 64; m++)
    {
        currLoad += slave[m]->requestQueue.size();
    }
}
