#include "request.hpp"
#include <array>

Request::Request(std::array<int, 10> requestedBlobs)
{
    shows = requestedBlobs;
}
