all: pre
	g++ -o test -Wall -Wextra -pedantic -g -ldl src/test.cpp -std=c++14 -O

pre:
	g++ -E -o test.pre -Wall -Wextra -pedantic src/test.cpp -std=c++14 -O

.PHONY: clean
clean:
	rm -f test test.pre
