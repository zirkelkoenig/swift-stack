#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logic.h"

int checkCollision(ICoord newPosition);

Square* history;

ICoord* proto;

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
	history = calloc(HISTORY, sizeof(Square));
	for(i = 0; i < HISTORY; i++) {
		history[i] = EMPTY;
	}

	// setup prototypes
	ICoord protoInit[EMPTY] = {
		{3, 17},
		{3, 18},
		{3, 18},
		{4, 18},
		{3, 18},
		{3, 18},
		{3, 18}
	};
	proto = calloc(EMPTY, sizeof(ICoord));
	memcpy(proto, &protoInit, EMPTY * sizeof(ICoord));

	activePiece = NULL;
	fieldState = INIT;
	CoordMap_init();
	return 0;
}

int getFieldIndex(ICoord square)
{
	return square.x + (square.y * FIELD_WIDTH);
}

int nextPiece()
{
	int result = EMPTY;
	int triesLeft = RAND_TRIES;
	int valid = 0;

	while((triesLeft-- > 0) && (!valid)) {
		result = rand() % EMPTY;
		valid = 1;

		int i = 0;
		for(i = 0; i < HISTORY; i++) {
			if(result == history[i]) {
				valid = 0;
				break;
			}
		}
	}
	memmove(&history[1], &history[0], (HISTORY - 1) * sizeof(Square));
	history[0] = result;

	activePiece = malloc(sizeof(Piece));
	activePiece->color = result;
	activePiece->orientation = NORTH;
	activePiece->position = proto[result];

	if(checkCollision(activePiece->position)) {
		return 0;
	} else {
		return 1;
	}
}

void lock()
{
	int i = 0;
	for(i = 0; i < 4; i++) {
		ICoord current = CoordMap_get(activePiece->color, activePiece->orientation, i);
		int index = getFieldIndex(ICoord_shift(current, activePiece->position.x, activePiece->position.y));
		playfield[index] = current->color;
	}

	free(activePiece);
	activePiece = NULL;
}

int moveHorizontal(int x)
{
	int direction = x >= 0 ? 1 : -1;
	int i = 0;

	for(i = 0; i != x; i += direction) {
		ICoord newPos = ICoord_shift(activePiece->position, direction, 0);
		if(checkCollision(newPos)) {
			return 1;
		} else {
			activePiece->position = newPos;
		}
	}
	return 0;
}

int moveDown(int y)
{
	int i = 0;
	for(i = y <= 0 ? y : 0; i != 0; i++) {
		ICoord newPos = ICoord_shift(activePiece->position, 0, -1);
		if(checkCollision(newPos)) {
			return 1;
		} else {
			activePiece->position = newPos;
		}
	}
	return 0;
}

int checkCollision(ICoord newPosition)
{
	int i = 0;
	for(i = 0; i < 4; i++) {
		ICoord base = CoordMap_get(activePiece->color, activePiece->orientation, i);
		ICoord checkSquare = ICoord_shift(base, newPosition.x, newPosition.y);

		if((checkSquare.x < 0) || (checkSquare.x >= FIELD_WIDTH) || (checkSquare.y < 0)) {
			return 1;
		}

		int index = getFieldIndex(checkSquare);
		if(playfield[index] != EMPTY) {
			return 1;
		}

		int maxOffset = 0;
		switch(activePiece->color) {
		case RED:
			maxOffset = 3;
			break;

		case YELLOW:
			maxOffset = 1;
			break;

		default:
			maxOffset = 2;
			break;
		}

		if(checkSquare.y >= (FIELD_HEIGHT + maxOffset)) {
			return 1;
		}
	}
	return 0;
}

void rotateRight()
{
	Orientation newOrientation = (activePiece->orientation + 1) % NONE;
	if(!checkCollision(activePiece->position)) {
		activePiece->orientation = newOrientation;
	}
}

void rotateLeft()
{
	Orientation newOrientation = activePiece->orientation - 1;
	if(newOrientation < 0) {
		newOrientation += NONE;
	}

	if(!checkCollision(activePiece->position)) {
		activePiece->orientation = newOrientation;
	}
}

void destroy()
{
	if(activePiece) {
		free(activePiece);
	}

	free(playfield);
	free(history);
	free(proto);

	CoordMap_destroy();
}

void markLines()
{
	int i = 0;
	int j = 0;
	int index = 0;
	int complete = 1;

	for(i = 0; i < FIELD_HEIGHT; i++) {
		complete = 1;

		for(j = 0; j < FIELD_WIDTH; j++) {
			index = (i * FIELD_WIDTH) + j;
			if(playfield[j] == EMPTY) {
				complete = 0;
				break;
			}
		}

		if(complete) {
			for(j = 0; j < FIELD_WIDTH; j++) {
				index = (i * FIELD_WIDTH) + j;
				playfield[index] = DESTROYED;
			}
		}
	}
}

int clearLines()
{
	int i = 0;
	int j = 0;
	int lineIndex = 0;
	int counter = 0;

	for(i = 0; i < FIELD_HEIGHT; i++) {
		lineIndex = i * FIELD_WIDTH;
		if(playfield[lineIndex] == DESTROYED) {
			memmove(&playfield[lineIndex], &playfield[lineIndex + FIELD_WIDTH],
					(FIELD_WIDTH * FIELD_HEIGHT) - lineIndex);
			for(j = 0; j < FIELD_WIDTH; j++) {
				playField[(FIELD_WIDTH * (FIELD_HEIGHT - 1)) + j] = EMPTY;
			}
			counter++;
		}
	}
	return counter;
}
