#include "load_balancer.hpp"

LoadBalancer::LoadBalancer(Destination * myMaster, int myWorkUnits)
{
    workUnits = myWorkUnits;
    master = myMaster;
}

void LoadBalancer::update()
{
    int currRoundWorkUnits = workUnits;
    while (currRoundWorkUnits > 0)
    {

    }
}
