FLAGS = -std=c++14 -Wall -Wextra -pedantic -g

all: my_function

.PHONY: my_function

my_function: Function_test.cpp Function.hpp
	g++ Function_test.cpp -o function_test $(FLAGS)

clean:
	rm -rf *.dSYM *.o function_test
