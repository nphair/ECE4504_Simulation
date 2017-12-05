#include "request.hpp"
#include <array>

Request::Request(std::array<int, SHOWS_PER_REQUEST> requestedBlobs)
{
    shows.fill(-1);
    shows = requestedBlobs;
    outgoing = false;
}
