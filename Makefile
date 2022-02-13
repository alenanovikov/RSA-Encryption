CXX = g++
CPPFLAGS = -Wall -g 

all: rsa

rsa: rsa.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@

clean: 
	rm rsa