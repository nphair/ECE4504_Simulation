CC = g++
CFLAGS =-Werror -Wall -std=c++11
DEPS = destination.hpp load_balancer.hpp request.hpp server.hpp
OBJ = destination.o load_balancer.o request.o server.o

# write makefile. In the meantime, run with...
# $ g++ destination.hpp destination.cpp load_balancer.cpp load_balancer.hpp request.hpp request.cpp server.hpp server.cpp main.cpp

all: simulation

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main.o: main.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

simulation: $(OBJ)
	$(CC) -o $@ $^ main.o $(CFLAGS)

clean:
	rm -f *.o ./simulation
