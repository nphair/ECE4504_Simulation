#ifndef SERVER_HPP
#define SERVER_HPP
#include "destination.hpp"

class Server : public Destination {
public:
    Server(Destination*, int);
    int showBlobs[100];
    void update() override;
};



#endif /* SERVER_HPP */
