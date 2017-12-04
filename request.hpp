#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <array>

class Request {
public:
    Request(std::array<int, 10>);
    //10 is arbitrarily chosen
    //A request whose shows array is empty, is a response that has been satisfied
    std::array<int, 10> shows;
    //Will have to be iterated in main, so we will need a reference to all the requests currently in the system
    int roundCount;
};

#endif /* REQUEST_HPP */
