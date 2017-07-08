/*
Copyright (C) 2017 Marvin Kipping

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
License version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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

/* Marks the currently active piece's position on the field. The piece is invalid after. Return is for error codes. */
int lock(State *state);

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

/*
Initializes the field, piece and randomizer of the given state.
*/
int init_field(State *state);

#endif
