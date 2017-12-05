#ifndef DESTINATION_HPP
#define DESTINATION_HPP
#include<deque>
#include"Request.hpp"

class Destination {
protected:
    //The Destination that is directly above the current Destination in the hierarchy
    Destination * master;
    //The amount of work units that can be done
    int workUnits;

public:
    Destination();
    virtual void update() {};
    std::deque<Request*> requestQueue;
    int currLoad;
    int showBlobs[100]; //100 is the total number of show blobs we have in the service TODO
};

#endif /* DESTINATION_HPP */
