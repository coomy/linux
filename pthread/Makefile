# author: Coomy

CXX = g++

CXXFLAGS += -std=c++11 -Wall
CXXFLAGS += -lpthread

source := $(wildcard *.cc)

main: $(source)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm main

.PHONY: clean

# http://blog.csdn.net/shui1025701856/article/details/7460276
