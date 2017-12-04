#include "load_balancer.hpp"
#include "destination.hpp"
#include "server.hpp"
#include "request.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Not all proper variables included:\nExpected Input: SERVER_NUM SERVER_PER_RACK STORAGE_PER_SERVER";
        exit(0);
    }
    int serverNum = atoi(argv[1]);
    int seversPerRack = atoi(argv[2]);
    int serverStorage = atoi(argv[3]);

    int loadBalanceWork = 10;
    int serverWork = 10;
    LoadBalancer * masterLoadBalancer = new LoadBalancer(NULL, loadBalanceWork);
}
