build:
	g++ main.cpp -lSDL3 -o ./main

run:
	./main

all: build run
