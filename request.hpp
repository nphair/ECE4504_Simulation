#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <array>
#include "globals.hpp"

class Request {
public:
    Request(std::array<int, MAX_BLOB_ACCESSES_PER_REQUEST>);
    //A request whose shows array is empty, is a response that has been satisfied.
    std::array<int, MAX_BLOB_ACCESSES_PER_REQUEST> shows;
    int roundCount;
    bool outgoing;

};

#endif /* REQUEST_HPP */
