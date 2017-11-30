# author: Coomy

CXX = g++

CXXFLAGS += -Wall -std=c++11

TARGET = server server_aio client

all: $(TARGET)

server_aio: server_aio.cc
	$(CXX) $< -o $@ $(CXXFLAGS) -lrt

%: %.cc
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm $(TARGET)

.PHONY: clean all

# http://blog.csdn.net/shui1025701856/article/details/7460276
