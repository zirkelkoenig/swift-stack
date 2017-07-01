CFLAGS=-Wall -g

all: static_data.o movement.o video_data.o control.o
	rm -f main
	gcc main.c static_data.o movement.o video_data.o control.o -Wall -g -lSDL2 -lSDL2_ttf -o main

control.o: static_data.o movement.o

movement.o: static_data.o

clean:
	rm -f *.o
	rm -f main
