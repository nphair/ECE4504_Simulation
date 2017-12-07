#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "destination.hpp"
#include <map>
#include "globals.hpp"
#include <vector>

class LoadBalancer: public Destination {
private:
    //If we switch this over to having main allocate them then we could just make these pointers to the Destination objects
    std::map<int,std::pair<int, Destination*>> roundLoadDict;
    bool containsBlob(int);

public:
    LoadBalancer(Destination*);
    void update() override;
    //Destination * slave[MAX_NUM_SLAVES];
    std::vector<Destination *> slave;
    int totalRoundsTakenByReqs;
    int totalReqsCompleted;
    void setShowBlobs(int);
};


#endif /* LOAD_BALANCER_HPP */
