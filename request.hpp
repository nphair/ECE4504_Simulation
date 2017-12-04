#ifndef REQUEST_HPP
#define REQUEST_HPP

class Request {
public:
    Request();
    //10 is arbitrarily chosen
    int shows[10];
    //Will have to be iterated in main, so we will need a reference to all the requests currently in the system
    int roundCount;
};

#endif /* REQUEST_HPP */
