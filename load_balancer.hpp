#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "destination.hpp"
#include <map>

class LoadBalancer: public Destination {
private:
    //If we switch this over to having main allocate them then we could just make these pointers to the Destination objects
    std::map<int,std::pair<int, Destination*>> roundLoadDict;

public:
    LoadBalancer(Destination*, int);
    void update() override;
    Destination * slave[64];
    int totalRoundsTakenByReqs;
    int totalReqsCompleted;
    void setShowBlobs(int);
};


#endif /* LOAD_BALANCER_HPP */
