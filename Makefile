run: bin main.o universe.o
	g++ -o bin/main bin/main.o bin/universe.o -lcurses

	rm -rf bin/*.o

main.o: main.cpp
	g++ -std=c++17 -O2 -Wall -c main.cpp -o bin/main.o

universe.o: src/universe.cpp
	g++ -std=c++17 -O2 -Wall -I./include -c src/universe.cpp -o bin/universe.o

bin:
	mkdir -p ./bin