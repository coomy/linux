# author: Coomy

CXX = g++

CXXFLAGS += -Wall -std=c++11 -levent

TARGET = timer socket_server

all: $(TARGET)

%: %.cc
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm $(TARGET)

.PHONY: clean all

