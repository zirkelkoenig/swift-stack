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
