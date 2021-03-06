#ifndef DESTINATION_HPP
#define DESTINATION_HPP
#include <deque>
#include "request.hpp"
#include "globals.hpp"

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
    int showBlobs[NUM_SHOW_BLOBS];

};

#endif /* DESTINATION_HPP */
