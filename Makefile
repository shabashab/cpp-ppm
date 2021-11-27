build: example.cpp ppm.hpp
	g++ -o example example.cpp

run: build
	./example

default: run
