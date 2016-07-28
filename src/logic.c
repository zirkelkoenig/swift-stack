#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logic.h"

int init()
{
	int i = 0;

	// setup playfield
	playfield = calloc(FIELD_WIDTH * FIELD_HEIGHT, sizeof(Square));

	for(i = 0; i < FIELD_WIDTH; i++) {
		int j = 0;
		for(j = 0; j < FIELD_HEIGHT; j++) {
			ICoord current = {i, j};
			playfield[getFieldIndex(current)] = EMPTY;
		}
	}

	// setup randomizer
	srand(time(NULL));
	pieceHistory = calloc(HISTORY, sizeof(Square));
	for(i = 0; i < HISTORY; i++) {
		pieceHistory[i] = EMPTY;
	}

	// setup prototypes
	prototypes = calloc(EMPTY, sizeof(Piece));

	Square color[EMPTY] = {
		RED,
		ORANGE,
		BLUE,
		YELLOW,
		PINK,
		TURQUOISE,
		GREEN
	};
	ICoord position[EMPTY] = {
		{3, 19},
		{4, 19},
		{3, 19},
		{4, 19},
		{3, 19},
		{3, 19},
		{4, 19}
	};
	int orientation[EMPTY] = {
		NORTH,
		NORTH,
		NORTH,
		NORTH,
		NORTH,
		NORTH,
		NORTH
	};
	ICoord squares[EMPTY][4] = {
		{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
		{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
		{{0, 1}, {1, 1}, {2, 0}, {2, 1}},
		{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
		{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
		{{0, 0}, {1, 0}, {1, 1}, {2, 0}},
		{{0, 1}, {1, 0}, {1, 1}, {2, 0}}
	};

	for(i = 0; i < EMPTY; i++) {
		prototypes[i].color = color[i];
		prototypes[i].position = position[i];
		prototypes[i].orientation = orientation[i];

		memcpy(&prototypes[i].squares, &squares[i], 4 * sizeof(ICoord));
	}

	activePiece = NULL;
	return 0;
}

int getFieldIndex(ICoord square)
{
	return square.x + (square.y * FIELD_WIDTH);
}

void nextPiece()
{
	int result = EMPTY;
	int triesLeft = RAND_TRIES;
	int valid = 0;

	while((triesLeft-- > 0) && (!valid)) {
		result = rand() % EMPTY;
		valid = 1;

		int i = 0;
		for(i = 0; i < HISTORY; i++) {
			if(result == pieceHistory[i]) {
				valid = 0;
				break;
			}
		}
	}

	int i = 0;
	for(i = 0; i < (HISTORY - 1); i++) {
		pieceHistory[i + 1] = pieceHistory[i];
	}
	pieceHistory[0] = result;

	activePiece = malloc(sizeof(Piece));
	memcpy(activePiece, &prototypes[result], sizeof(Piece));
}

void move(int x, int y)
{
	ICoord newPos = ICoord_shift(activePiece->position, x, y);

	if(newPos.x < 0) {
		newPos.x = 0;
	} else {
		int i = 0;
		int offset = 0;
		for(i = 0; i < 4; i++) {
			if(activePiece->squares[i].x > offset) {
				offset = activePiece->squares[i].x;
			}
		}
		if((newPos.x + offset) >= FIELD_WIDTH) {
			newPos.x = FIELD_WIDTH - 1 - offset;
		}
	}

	if(newPos.y < 0) {
		newPos.y = 0;
	} else if(newPos.y >= FIELD_HEIGHT) {
		newPos.y = FIELD_HEIGHT - 1;
	}

	activePiece->position = newPos;
}

void lock()
{
	int i = 0;
	for(i = 0; i < 4; i++) {
		ICoord square = ICoord_shift(activePiece->squares[i], activePiece->position.x, activePiece->position.y);
		int index = getFieldIndex(square);
		playfield[index] = activePiece->color;
	}

	free(activePiece);
	activePiece = NULL;
}
