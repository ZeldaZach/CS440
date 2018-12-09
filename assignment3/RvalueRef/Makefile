FLAGS = -std=c++14 -Wall -Wextra -pedantic -g

all: my_array

.PHONY: my_int my_array

my_int: MyInt.cpp MyInt.hpp
	g++ MyInt.cpp -c -o MyInt.o $(FLAGS)

my_array: Array.hpp MyInt.o
	g++ Array_test.cpp -o array_test $(FLAGS)

clean:
	rm -rf *.dSYM *.o array_test
