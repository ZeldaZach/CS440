FLAGS = -std=c++17 -Wall -Wextra -pedantic -g

all: make_ostream

.PHONY: make_ostream

make_ostream: Interpolate.hpp test_Interpolate.cpp
	g++ test_Interpolate.cpp -o ostream $(FLAGS)

clean:
	rm -rf *.dSYM *.o ostream
