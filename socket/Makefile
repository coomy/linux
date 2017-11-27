# author: Coomy

CXX = g++

CXXFLAGS +=  -Wall -std=c++11

TARGET = server client

all: $(TARGET)

%: %.cc
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm $(TARGET)

.PHONY: clean all

# http://blog.csdn.net/shui1025701856/article/details/7460276
