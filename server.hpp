#ifndef SERVER_HPP
#define SERVER_HPP
#include "destination.hpp"

class Server : public Destination {
public:
    Server(Destination*, int, int);
    int storageAmt;
    void update() override;
    void setShowBlobs(const int, int);

private:
    bool process_request(Request * request);

};

#endif /* SERVER_HPP */
