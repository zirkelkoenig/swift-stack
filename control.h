/*
Copyright (C) 2017 Marvin Kipping

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; version 2 only.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef _control_h
#define _control_h

#include "static_data.h"

/* Creates a new game */
State *init_state();

/* Destroys a game and deallocates its resources. */
void destroy_state(State *state);

/*
To be called once per frame, this function controls the game flow, processing player input and modifying the game state
accordingly. Return codes:
	2 - win (level 999 reached)
	1 - success
	0 - block out/death (new piece couldn't be spawned)
*/
int process(State *state, Input_Map *input);

#endif
