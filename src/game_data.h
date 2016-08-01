#ifndef _game_data_h
#define _game_data_h

#include <stdint.h>
#include "math_data.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define RAND_TRIES 4
#define RAND_HISTORY 4

#define GRAVITY 10.0
#define LOAD_DELAY 30
#define	LOCK_DELAY 30
#define CLEAR_DELAY 30

enum Color {
	C_RED,
	C_ORANGE,
	C_BLUE,
	C_YELLOW,
	C_PINK,
	C_TURQUOISE,
	C_GREEN,
	C_EMPTY,
	C_DESTROYED
};

enum Orientation {
	O_NORTH,
	O_EAST,
	O_SOUTH,
	O_WEST,
	O_NONE
};

enum Phase {
	P_LOAD,
	P_DROP,
	P_LOCK,
	P_CLEAR
};

typedef struct Piece {
	int color;
	int orientation;
	ICoord position;
} Piece;

/* Initializes the tetromino map. Returns 0 on success and a negative integer on error. */
int GameData_init();

/* Gets the coordinates of a single tetromino square. Returns NULL on error. */
const ICoord* GameData_getSquare(int color, int orientation, int num);

/* Destroys the tetromino map. */
void GameData_destroy();

/* Gets the coordinates of a tetromino's spawn position. Returns NULL on error. */
const ICoord* GameData_getSpawn(int color);

/* Gets the color of a tetromino. Returns NULL on error. */
const uint8_t* GameData_getColor(int phase, int color, int num);

#endif
