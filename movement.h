#ifndef _movement_h
#define _movement_h

#include "static_data.h"

/*
Shift the currently active piece left (direction < 0) or right (direction > 0). Return codes:
	1 - success
	0 - fail (collision)
*/
int shift(State *state, int direction);

/*
Rotate the currently active piece counter-clockwise (direction < 0) or clockwise (direction > 0) and perform wall kicks
if necessary/possible. Return codes:
	1 - success
	0 - fail (collision)
*/
int rotate(State *state, int direction);

#endif
