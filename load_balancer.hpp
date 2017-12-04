#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "destination.hpp"
#include <map>

class LoadBalancer: public Destination {
private:
    //If we switch this over to having main allocate them then we could just make these pointers to the Destination objects
    Destination slave[64];
    std::map<int, Destination*> RoundLoadDict;

public:
    LoadBalancer(Destination*, int);
    void update() override;

};


#endif /* LOAD_BALANCER_HPP */
