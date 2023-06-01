CXX = g++
CXXFLAGS = -std=c++2a

OBJS = 

.PHONY: clean

all: main

main: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

run:
	./main

clean:
	rm -f $(OBJS) main
