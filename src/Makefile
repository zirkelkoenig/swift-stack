CFLAGS=-Wall -g

all:
	rm -f main
	gcc main.c coord_map.o math_data.o logic.o -Wall -g -lSDL2 -o main

test: coord_map.o math_data.o logic.o

coord_map.o: math_data.o

logic.o: math_data.o coord_map.o
