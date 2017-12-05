#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <array>

class Request {
public:
    // 10 is the max number of shows we have in a single request!
    static const int SHOWS_PER_REQUEST = 10;
    Request(std::array<int, SHOWS_PER_REQUEST>);
    //A request whose shows array is empty, is a response that has been satisfied.
    std::array<int, SHOWS_PER_REQUEST> shows;
    int roundCount;
    bool outgoing;

};

#endif /* REQUEST_HPP */
