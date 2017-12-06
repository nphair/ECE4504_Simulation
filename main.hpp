#include "load_balancer.hpp"
#include "destination.hpp"
#include "server.hpp"
#include "request.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <list>
#include <numeric>
#include <random>

Request * requestGen(LoadBalancer*, std::vector<Request*>, const int);
