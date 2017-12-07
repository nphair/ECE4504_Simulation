CC = g++
CFLAGS =-Werror -Wall -std=c++11 -g
DEPS = destination.hpp load_balancer.hpp request.hpp server.hpp
OBJ = destination.o load_balancer.o request.o server.o main.o

all: simulation

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simulation: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o ./simulation
