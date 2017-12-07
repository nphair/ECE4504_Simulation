#include "request.hpp"
#include <array>

Request::Request(std::array<int, MAX_BLOB_ACCESSES_PER_REQUEST> requestedBlobs)
{
    shows.fill(-1);
    shows = requestedBlobs;
    outgoing = false;
    roundCount = 0;
}
