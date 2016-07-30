#ifndef _coord_map_h
#define _coord_map_h

#include "logic.h"
#include "math_data.h"

/* Initializes the tetromino map. Returns 0 on success and a negative integer on error. */
int CoordMap_init();

/* Gets the coordinates of a single tetromino square. returns NULL on error. */
ICoord* CoordMap_get(int color, int orientation, int num);

/* Destroys the tetromino map. */
void CoordMap_destroy();

#endif
