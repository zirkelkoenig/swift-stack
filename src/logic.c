#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "error.h"
#include "logic.h"

/* Checks whether shifting the current piece to a new position results in a collision. If so, 1 is returned, otherwise
0 is returned. Returns -1 on error. */
int checkCollision(State* state, ICoord newPosition, int newOrientation);

State* newGame()
{
	int i = 0;
	int j = 0;
	int index = 0;

	// create a new game state
	State* state = malloc(sizeof(State));
	alloc_check(state);
	state->phase = P_LOAD;
	state->activePiece.color = C_EMPTY;

	// setup playfield
	state->playfield = calloc(FIELD_WIDTH * FIELD_HEIGHT, sizeof(int));
	alloc_check(state->playfield);

	for(i = 0; i < FIELD_WIDTH; i++) {
		for(j = 0; j < FIELD_HEIGHT; j++) {
			index = getFieldIndex(i, j);
			rc_check(index, "getFieldIndex");

			state->playfield[index] = C_EMPTY;
		}
	}

	// setup randomizer
	srand(time(NULL));

	state->history = calloc(RAND_HISTORY, sizeof(int));
	alloc_check(state->history);

	for(i = 0; i < RAND_HISTORY; i++) {
		state->history[i] = C_EMPTY;
	}

	rc_check(GameData_init(), "GameData_init");
	return state;

error:
	if(state) {
		if(state->playfield) {
			free(state->playfield);
		}
		if(state->history) {
			free(state->history);
		}
		free(state);
	}
	return NULL;
}

int getFieldIndex(int x, int y)
{
	cond_check((x >= 0) && (x < FIELD_WIDTH), "argument \"x\" out of bounds");
	cond_check((y >= 0) && (y < FIELD_HEIGHT), "argument \"y\" out of bounds");

	return x + (y * FIELD_WIDTH);
error:
	return -1;
}

int nextPiece(State* state)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->history, "argument \"state\" invalid");
	cond_check(state->activePiece.color == C_EMPTY, "argument \"state\" already has an active piece");

	int result = C_EMPTY;
	int triesLeft = RAND_TRIES;
	int valid = 0;

	while((triesLeft-- > 0) && (!valid)) {
		result = rand() % C_EMPTY;
		valid = 1;

		int i = 0;
		for(i = 0; i < RAND_HISTORY; i++) {
			if(result == state->history[i]) {
				valid = 0;
				break;
			}
		}
	}
	int* rp = memmove(&state->history[1], &state->history[0], (RAND_HISTORY - 1) * sizeof(int));
	cond_check(rp == &state->history[1], "memory move failed");
	state->history[0] = result;

	state->activePiece.color = result;
	state->activePiece.orientation = O_NORTH;
	state->activePiece.position = *GameData_getSpawn(result);

	int coll = checkCollision(state, state->activePiece.position, state->activePiece.orientation);
	rc_check(coll, "checkCollision");
	if(coll) {
		return 0;
	} else {
		return 1;
	}

error:
	return -1;
}

int lock(State* state)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->playfield, "argument \"playfield\" invalid");
	cond_check(state->activePiece.color != C_EMPTY, "argument \"state\" has no active piece");

	int i = 0;
	for(i = 0; i < 4; i++) {
		const ICoord *square= GameData_getSquare(state->activePiece.color, state->activePiece.orientation, i);
		cond_check(square, "function \"GameData_getSquare\" returned an error");

		ICoord current = ICoord_shift(*square, state->activePiece.position.x, state->activePiece.position.y);
		int index = getFieldIndex(current.x, current.y);
		rc_check(index, "getFieldIndex");

		state->playfield[index] = state->activePiece.color;
	}

	state->activePiece.color = C_EMPTY;
	return 0;

error:
	return -1;
}

int moveHorizontal(State* state, int x)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->activePiece.color != C_EMPTY, "argument \"state\" has no active piece");

	int direction = x >= 0 ? 1 : -1;
	int i = 0;

	for(i = 0; i != x; i += direction) {
		ICoord newPos = ICoord_shift(state->activePiece.position, direction, 0);
		int coll = checkCollision(state, newPos, state->activePiece.orientation);
		rc_check(coll, "checkCollision");
		if(coll) {
			return 1;
		} else {
			state->activePiece.position = newPos;
		}
	}
	return 0;

error:
	return -1;
}

int moveDown(State* state, int y)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->activePiece.color != C_EMPTY, "argument \"state\" has no active piece");

	int i = 0;
	for(i = y <= 0 ? y : 0; i != 0; i++) {
		ICoord newPos = ICoord_shift(state->activePiece.position, 0, -1);
		int coll = checkCollision(state, newPos, state->activePiece.orientation);
		rc_check(coll, "checkCollision");
		if(coll) {
			return 1;
		} else {
			state->activePiece.position = newPos;
		}
	}
	return 0;

error:
	return -1;
}

int checkCollision(State* state, ICoord newPosition, int newOrientation)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->activePiece.color != C_EMPTY, "argument \"state\" has no active piece");

	int i = 0;
	for(i = 0; i < 4; i++) {
		const ICoord* base = GameData_getSquare(state->activePiece.color, newOrientation, i);
		cond_check(base, "function \"GameData_getSquare\" returned an error");

		ICoord checkSquare = ICoord_shift(*base, newPosition.x, newPosition.y);
		if((checkSquare.x < 0) || (checkSquare.x >= FIELD_WIDTH) || (checkSquare.y < 0)) {
			return 1;
		}

		int index = getFieldIndex(checkSquare.x, checkSquare.y);
		rc_check(index, "getFieldIndex");
		if(state->playfield[index] != C_EMPTY) {
			return 1;
		}

		int maxOffset = 0;
		switch(state->activePiece.color) {
		case C_RED:
			maxOffset = 3;
			break;

		case C_YELLOW:
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

int rotateRight(State* state)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->activePiece.color != C_EMPTY, "argument \"state\" has no active piece");

	int newOrientation = (state->activePiece.orientation + 1) % O_NONE;
	int coll = checkCollision(state, state->activePiece.position, newOrientation);
	rc_check(coll, "checkCollision");
	if(!coll) {
		state->activePiece.orientation = newOrientation;
	}
	return 0;

error:
	return -1;
}

int rotateLeft(State* state)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->activePiece.color != C_EMPTY, "argument \"state\" has no active piece");

	int newOrientation = state->activePiece.orientation - 1;
	if(newOrientation < 0) {
		newOrientation += O_NONE;
	}

	int coll = checkCollision(state, state->activePiece.position, newOrientation);
	rc_check(coll, "checkCollision");
	if(!coll) {
		state->activePiece.orientation = newOrientation;
	}
	return 0;

error:
	return -1;
}

void destroyGame(State* state)
{
	if(state) {
		if(state->playfield) {
			free(state->playfield);
		}
		if(state->history) {
			free(state->history);
		}
		free(state);
	}
	GameData_destroy();
}

int markLines(State* state)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->playfield, "argument \"state\" invalid");

	int i = 0;
	int j = 0;
	int counter = 0;

	for(i = 0; i < FIELD_HEIGHT; i++) {
		int complete = 1;

		for(j = 0; j < FIELD_WIDTH; j++) {
			int index = getFieldIndex(j, i);
			rc_check(index, "getFieldIndex");

			if((state->playfield[index] < C_RED) || (state->playfield[index] > C_GREEN)) {
				complete = 0;
				break;
			}
		}

		if(complete) {
			for(j = 0; j < FIELD_WIDTH; j++) {
				int index = getFieldIndex(j, i);
				rc_check(index, "getFieldWidth");
				state->playfield[index] = C_DESTROYED;
			}
			counter++;
		}
	}
	return counter;

error:
	return -1;
}

int clearLines(State* state)
{
	cond_check(state, "argument \"state\" uninitialized");
	cond_check(state->playfield, "argument \"state\" invalid");

	int i = 0;
	int j = 0;
	int counter = 0;

	for(i = 0; i < FIELD_HEIGHT; i++) {
		int dstIndex = getFieldIndex(0, i);
		rc_check(dstIndex, "getFieldIndex");

		if(state->playfield[dstIndex] == C_DESTROYED) {
			int srcIndex = getFieldIndex(0, i + 1);
			rc_check(srcIndex, "getFieldIndex");
			int moveLength = getFieldIndex(FIELD_WIDTH - 1, FIELD_HEIGHT - 1);
			rc_check(moveLength, "getFieldIndex");
			moveLength -= srcIndex;

			int* rp = memmove(&state->playfield[dstIndex], &state->playfield[srcIndex],
					moveLength * sizeof(int));
			cond_check(rp == &state->playfield[dstIndex], "memory move failed");

			for(j = 0; j < FIELD_WIDTH; j++) {
				int index = getFieldIndex(j, FIELD_HEIGHT - 1);
				rc_check(index, "getFieldIndex");
				state->playfield[index] = C_EMPTY;
			}

			counter++;
			i--;
		}
	}
	return counter;

error:
	return -1;
}
