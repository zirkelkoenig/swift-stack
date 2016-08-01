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
	ICoord current = {0, 0};

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
			current.x = i;
			current.y = j;

			index = getFieldIndex(current);
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
		const ICoord *current = GameData_getSquare(state->activePiece.color, state->activePiece.orientation, i);
		cond_check(current, "function \"GameData_getSquare\" returned an error");

		int index = getFieldIndex(ICoord_shift(*current, state->activePiece.position.x,
				state->activePiece.position.y));
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

		int index = getFieldIndex(checkSquare);
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
	int index = 0;
	int complete = 1;
	int counter = 0;
	ICoord square = {0, 0};

	for(i = 0; i < FIELD_HEIGHT; i++) {
		complete = 1;

		for(j = 0; j < FIELD_WIDTH; j++) {
			square.x = j;
			square.y = i;
			index = getFieldIndex(square);
			rc_check(index, "getFieldIndex");
			if((state->playfield[index] == C_EMPTY) || (state->playfield[index] == C_DESTROYED)) {
				complete = 0;
				break;
			}
		}

		if(complete) {
			for(j = 0; j < FIELD_WIDTH; j++) {
				square.x = j;
				square.y = i;
				index = getFieldIndex(square);
				rc_check(index, "getFieldIndex");
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
	int lineIndex = 0;
	int counter = 0;

	for(i = 0; i < FIELD_HEIGHT; i++) {
		lineIndex = i * FIELD_WIDTH;
		if(state->playfield[lineIndex] == C_DESTROYED) {
			int* rp = memmove(&state->playfield[lineIndex], &state->playfield[lineIndex + FIELD_WIDTH],
					(FIELD_WIDTH * FIELD_HEIGHT) - lineIndex);
			cond_check(rp == &state->playfield[lineIndex], "memory move failed");
			for(j = 0; j < FIELD_WIDTH; j++) {
				state->playfield[(FIELD_WIDTH * (FIELD_HEIGHT - 1)) + j] = C_EMPTY;
			}
			counter++;
		}
	}
	return counter;

error:
	return -1;
}
