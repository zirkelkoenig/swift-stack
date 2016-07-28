#ifndef _coord_map_h
#define _coord_map_h

#include "logic.h"
#include "math_data.h"

/* Initializes the tetromino map. */
void CoordMap_init();

/* Gets the coordinates of a single tetromino square. */
ICoord CoordMap_get(Square color, Orientation orientation, int num);

/* Destroys the tetromino map. */
void CoordMap_destroy();

#endif
