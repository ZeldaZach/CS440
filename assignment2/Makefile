FLAGS = -std=c++14 -Wall -Wextra -pedantic -g -O

all: set1 set2 set3 set4 set5

.PHONY: set1 set2 set3 set4 set5

set1:
	g++ src/test-kec.cpp -o test-kec $(FLAGS)

set2:
	g++ src/test.cpp -o test $(FLAGS)

set3:
	g++ src/minimal.cpp -o minimal $(FLAGS)

set4:
	g++ src/morseex.cpp -o morseex $(FLAGS)

set5:
	g++ src/test-scaling.cpp -o test-scaling $(FLAGS)

clean:
	rm -rf Map test minimal test-kec test-scaling morseex *.dSYM outs/*
