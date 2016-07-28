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
			playfield[getFieldNum(current)] = EMPTY;
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

int getFieldNum(ICoord square)
{
	return square.x + (square.y * FIELD_WIDTH);
}

Piece* nextPiece()
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

	Piece* piece = malloc(sizeof(Piece));
	memcpy(piece, &prototypes[result], sizeof(Piece));
	return piece;
}

void move(Piece* piece, int x, int y)
{
	ICoord newPos = ICoord_shift(piece->position, x, y);

	if(newPos.x < 0) {
		newPos.x = 0;
	} else {
		int i = 0;
		int offset = 0;
		for(i = 0; i < 4; i++) {
			if(piece->squares[i].x > offset) {
				offset = piece->squares[i].x;
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

	piece->position = newPos;
}
