#ifndef SERVER_HPP
#define SERVER_HPP
#include "destination.hpp"

class Server : public Destination {
public:
    Server(Destination*, int, int);
    int showBlobs[100];
    int storageAmt;
    void update() override;
    void setShowBlobs(const int, int);
};



#endif /* SERVER_HPP */
