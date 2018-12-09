FLAGS = -std=c++17 -Wall -Wextra -pedantic -g

all: make_array

.PHONY: make_array

make_array: Array.hpp test_Array.cpp
	g++ test_Array.cpp -o array $(FLAGS)

clean:
	rm -rf *.dSYM *.o array
