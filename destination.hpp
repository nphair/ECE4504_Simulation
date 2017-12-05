#ifndef DESTINATION_HPP
#define DESTINATION_HPP
#include <deque>
#include "request.hpp"

class Destination {
protected:
    //The Destination that is directly above the current Destination in the hierarchy
    Destination * master;
    //The amount of work units that can be done
    int workUnits;
    static const int SHOWS_PER_BLOB = 100;

public:
    Destination();
    virtual void update() {};
    std::deque<Request*> requestQueue;
    int currLoad;
    int showBlobs[SHOWS_PER_BLOB];

};

#endif /* DESTINATION_HPP */
