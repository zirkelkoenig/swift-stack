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

/*
Move the currently active piece down the field by the number of lines specified by speed. Stops when encountering a
collision. Return codes:
	2 - success
	1 - success with collision
	0 - fail with collision
*/
int drop(State *state, int speed);

/* Marks the currently active piece's position on the field. The piece is invalid after. */
void lock(State *state);

/* Scans the field for complete lines and marks them for clearing. Returns the number of marked lines. */
int clear(State *state);

/*
Removes all lines marked by clear() from the field and consolidates the stack. Returns the number of destroyed lines.
*/
int destroy(State *state);

/* Takes the next piece in the spawn history, places it at the initial position and updates the history. Return codes:
	1 - success
	0 - fail (collision)
*/
int spawn(State *state);

#endif
