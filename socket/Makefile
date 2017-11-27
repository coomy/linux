# author: Coomy

CXX = g++

CXXFLAGS += -std=c++11 -Wall

TARGET = server client

all: $(TARGET)

%: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm $(TARGET)

.PHONY: clean all

# http://blog.csdn.net/shui1025701856/article/details/7460276
