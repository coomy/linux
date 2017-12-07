# author: Coomy

CXX = g++

CXXFLAGS += -Wall -std=c++11 -lcrypto

TARGET = rsa

all: $(TARGET)

%: %.cc
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm $(TARGET)

.PHONY: clean all

