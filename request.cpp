#include "request.hpp"
#include <array>

Request::Request(std::array<int, 10> requestedBlobs)//10 is the max number of showblobs we have in a single request! TODO
{
    shows.fill(-1);
    shows = requestedBlobs;
    outgoing = false;
}
