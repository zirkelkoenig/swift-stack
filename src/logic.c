#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "coord_map.h"
#include "error.h"
#include "logic.h"

/* Checks whether shifting the current piece to a new position results in a collision. If so, 1 is returned, otherwise
0 is returned. Returns -1 on error. */
int checkCollision(ICoord newPosition, int newOrientation);

int* history;

ICoord* proto;

int init()
{
	int i = 0;
	int j = 0;
	int index = 0;
	ICoord current = {0, 0};

	// setup playfield
	playfield = calloc(FIELD_WIDTH * FIELD_HEIGHT, sizeof(int));
	alloc_check(playfield);

	for(i = 0; i < FIELD_WIDTH; i++) {
		for(j = 0; j < FIELD_HEIGHT; j++) {
			current.x = i;
			current.y = j;

			index = getFieldIndex(current);
			rc_check(index, "getFieldIndex");

			playfield[index] = EMPTY;
		}
	}

	// setup randomizer
	srand(time(NULL));

	history = calloc(HISTORY, sizeof(int));
	alloc_check(history);

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
	alloc_check(proto);

	ICoord* rp = memcpy(proto, &protoInit, EMPTY * sizeof(ICoord));
	cond_check(rp == proto, "memory copy failed");

	activePiece = NULL;
	rc_check(CoordMap_init(), "CoordMap_init");
	return 0;

error:
	if(playfield) {
		free(playfield);
		playfield = NULL;
	}
	if(history) {
		free(history);
		history = NULL;
	}
	if(proto) {
		free(proto);
		proto = NULL;
	}
	return -1;
}

int getFieldIndex(ICoord square)
{
	cond_check(square.x >= 0 &&
			square.x < FIELD_WIDTH &&
			square.y >= 0 &&
			square.y < FIELD_HEIGHT,
			"argument \"square\" out of bounds");
	return square.x + (square.y * FIELD_WIDTH);

error:
	return -1;
}

int nextPiece()
{
	global_check(history, "history");

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
	int* rp = memmove(&history[1], &history[0], (HISTORY - 1) * sizeof(int));
	cond_check(rp == &history[1], "memory move failed");
	history[0] = result;

	if(!activePiece) {
		activePiece = malloc(sizeof(Piece));
		alloc_check(activePiece);
	}
	activePiece->color = result;
	activePiece->orientation = NORTH;
	activePiece->position = proto[result];

	int coll = checkCollision(activePiece->position, activePiece->orientation);
	rc_check(coll, "checkCollision");
	if(coll) {
		return 0;
	} else {
		return 1;
	}

error:
	if(activePiece) {
		free(activePiece);
		activePiece = NULL;
	}
	return -1;
}

int lock()
{
	global_check(activePiece, "activePiece");

	int i = 0;
	for(i = 0; i < 4; i++) {
		ICoord *current = CoordMap_get(activePiece->color, activePiece->orientation, i);
		cond_check(current, "function \"CoordMap_get\" returned an error");

		int index = getFieldIndex(ICoord_shift(*current, activePiece->position.x, activePiece->position.y));
		rc_check(index, "getFieldIndex");

		free(current);
		playfield[index] = activePiece->color;
	}

	free(activePiece);
	activePiece = NULL;

	return 0;

error:
	return -1;
}

int moveHorizontal(int x)
{
	global_check(activePiece, "activePiece");

	int direction = x >= 0 ? 1 : -1;
	int i = 0;

	for(i = 0; i != x; i += direction) {
		ICoord newPos = ICoord_shift(activePiece->position, direction, 0);
		int coll = checkCollision(newPos, activePiece->orientation);
		rc_check(coll, "checkCollision");
		if(coll) {
			return 1;
		} else {
			activePiece->position = newPos;
		}
	}
	return 0;

error:
	return -1;
}

int moveDown(int y)
{
	global_check(activePiece, "activePiece");

	int i = 0;
	for(i = y <= 0 ? y : 0; i != 0; i++) {
		ICoord newPos = ICoord_shift(activePiece->position, 0, -1);
		int coll = checkCollision(newPos, activePiece->orientation);
		rc_check(coll, "checkCollision");
		if(coll) {
			return 1;
		} else {
			activePiece->position = newPos;
		}
	}
	return 0;

error:
	return -1;
}

int checkCollision(ICoord newPosition, int newOrientation)
{
	global_check(activePiece, "activePiece");

	int i = 0;
	for(i = 0; i < 4; i++) {
		ICoord* base = CoordMap_get(activePiece->color, newOrientation, i);
		cond_check(base, "function \"CoordMap_get\" returned an error");

		ICoord checkSquare = ICoord_shift(*base, newPosition.x, newPosition.y);
		free(base);
		if((checkSquare.x < 0) || (checkSquare.x >= FIELD_WIDTH) || (checkSquare.y < 0)) {
			return 1;
		}

		int index = getFieldIndex(checkSquare);
		rc_check(index, "getFieldIndex");
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

error:
	return -1;
}

int rotateRight()
{
	global_check(activePiece, "activePiece");

	int newOrientation = (activePiece->orientation + 1) % NONE;
	int coll = checkCollision(activePiece->position, newOrientation);
	rc_check(coll, "checkCollision");
	if(!coll) {
		activePiece->orientation = newOrientation;
	}
	return 0;

error:
	return -1;
}

int rotateLeft()
{
	global_check(activePiece, "activePiece");

	int newOrientation = activePiece->orientation - 1;
	if(newOrientation < 0) {
		newOrientation += NONE;
	}

	int coll = checkCollision(activePiece->position, newOrientation);
	rc_check(coll, "checkCollision");
	if(!coll) {
		activePiece->orientation = newOrientation;
	}
	return 0;

error:
	return -1;
}

void destroy()
{
	if(activePiece) {
		free(activePiece);
		activePiece = NULL;
	}
	if(playfield) {
		free(playfield);
		playfield = NULL;
	}
	if(history) {
		free(history);
		history = NULL;
	}
	if(proto) {
		free(proto);
		proto = NULL;
	}

	CoordMap_destroy();
}

int markLines()
{
	global_check(playfield, "playfield");

	int i = 0;
	int j = 0;
	int index = 0;
	int complete = 1;
	int counter = 0;

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
			counter++;
		}
	}
	return counter;

error:
	return -1;
}

int clearLines()
{
	global_check(playfield, "playfield");

	int i = 0;
	int j = 0;
	int lineIndex = 0;
	int counter = 0;

	for(i = 0; i < FIELD_HEIGHT; i++) {
		lineIndex = i * FIELD_WIDTH;
		if(playfield[lineIndex] == DESTROYED) {
			int* rp = memmove(&playfield[lineIndex], &playfield[lineIndex + FIELD_WIDTH],
					(FIELD_WIDTH * FIELD_HEIGHT) - lineIndex);
			cond_check(rp == &playfield[lineIndex], "memory move failed");
			for(j = 0; j < FIELD_WIDTH; j++) {
				playfield[(FIELD_WIDTH * (FIELD_HEIGHT - 1)) + j] = EMPTY;
			}
			counter++;
		}
	}
	return counter;

error:
	return -1;
}
