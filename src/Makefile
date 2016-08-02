CFLAGS=-Wall -g

all: game_data.o logic.o math_data.o
	rm -f main
	gcc main.c game_data.o math_data.o logic.o -Wall -g -lSDL2 -o main

test: game_data.o logic.o math_data.o

game_data.o: math_data.o

logic.o: game_data.o math_data.o

clean:
	rm -f *.o
	rm -f main
	rm -f test
