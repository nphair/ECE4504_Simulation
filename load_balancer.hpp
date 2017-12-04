#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "destination.hpp"
#include <map>

class LoadBalancer: public Destination {
private:
    //If we switch this over to having main allocate them then we could just make these pointers to the Destination objects
    std::map<int, Destination*> RoundLoadDict;

public:
    LoadBalancer(Destination*, int);
    void update() override;
    Destination * slave[64];

};


#endif /* LOAD_BALANCER_HPP */
