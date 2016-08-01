#ifndef _coord_map_h
#define _coord_map_h

#include "math_data.h"

enum Square {
	RED,
	ORANGE,
	BLUE,
	YELLOW,
	PINK,
	TURQUOISE,
	GREEN,
	EMPTY,
	DESTROYED
};

enum Orientation {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NONE
};

/* Initializes the tetromino map. Returns 0 on success and a negative integer on error. */
int CoordMap_init();

/* Gets the coordinates of a single tetromino square. Returns NULL on error. */
const ICoord* CoordMap_getSquare(int color, int orientation, int num);

/* Destroys the tetromino map. */
void CoordMap_destroy();

/* Gets the coordinates of a tetromino's spawn position. Returns NULL on error. */
const ICoord* CoordMap_getSpawn(int color);

#endif
