#include <stdlib.h>
#include "movement.h"
#include "control.h"
#include "dbg.h"

Input_Map *last_input;

State *init_state()
{
	State *state = malloc(sizeof(State));
	check_mem(state);

	state->level = 0;
	state->phase = LOADING;
	state->phase_counter = 0;
	state->shift_counter = 0;

	int rc = init_field(state);
	check(rc >= 0, "\"init_field\" returned an error");

	// temporary fixed values
	state->timing.gravity = 4;
	state->timing.load = 25;
	state->timing.load_clear = 25;
	state->timing.shift = 14;
	state->timing.lock = 30;
	state->timing.clear = 40;

	last_input = malloc(sizeof(Input_Map));
	check_mem(last_input);

	last_input->left = 0;
	last_input->right = 0;
	last_input->up = 0;
	last_input->down = 0;
	last_input->rot_left_a = 0;
	last_input->rot_left_b = 0;
	last_input->rot_right = 0;

	return state;

error:
	if (state) {
		free(state);
	}
	if (last_input) {
		free(last_input);
	}
	return NULL;
}

void destroy_state(State *state)
{
	check_debug(state, "argument \"state\" uninitialized");
	check_debug(last_input, "global \"last_input\" uninitialized");

	free(state);
	free(last_input);

error:
	;// do nothing
}

int process(State *state, Input_Map *input)
{
	check(state, "argument \"state\" uninitialized");
	check(state, "argument \"input\" uninitialized");
	check(last_input, "global \"last_input\" uninitialized");

	if (input->left && input->right) {
		input->left = 0;
		input->right = 0;
	}
	if (input->up && input->down) {
		input->up = 0;
		input->down = 0;
	}

	int rc = 0;

	switch (state->phase) {
	case LOADING:
		if (input->left) {
			if (last_input->left) {
				state->shift_counter++;
			} else {
				state->shift_counter = 1;
			}
		} else if (input->right) {
			if (last_input->right) {
				state->shift_counter++;
			} else {
				state->shift_counter = 1;
			}
		} else {
			state->shift_counter = 0;
		}

		state->phase_counter++;
		if (state->phase_counter == state->timing.load) {
			rc = spawn(state);
			check(rc >= 0, "\"spawn\" returned an error");

			if (input->rot_left_a) {
				rc = rotate(state, -1);
				check(rc >= 0, "\"rotate\" returned an error");
			}
			if (input->rot_left_b) {
				rc = rotate(state, -1);
				check(rc >= 0, "\"rotate\" returned an error");
			}
			if (input->rot_right) {
				rc = rotate(state, 1);
				check(rc >= 0, "\"rotate\" returned an error");
			}

			if (rc == 0) {
				return 0;
			}
			state->phase = DROPPING;
			state->phase_counter = 0;
		}
		break;

	case DROPPING:
		if (input->rot_left_a) {
			rc = rotate(state, -1);
			check(rc >= 0, "\"rotate\" returned an error");
		}
		if (input->rot_left_b) {
			rc = rotate(state, -1);
			check(rc >= 0, "\"rotate\" returned an error");
		}
		if (input->rot_right) {
			rc = rotate(state, 1);
			check(rc >= 0, "\"rotate\" returned an error");
		}

		if (input->left) {
			if (last_input->left) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, -1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, -1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, -1);
					check(rc >= 0, "\"shift\" returned an error");
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, -1);
				check(rc >= 0, "\"shift\" returned an error");
				state->shift_counter = 1;
			}
		} else if (input->right) {
			if (last_input->right) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, 1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, 1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, 1);
					check(rc >= 0, "\"shift\" returned an error");
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, 1);
				state->shift_counter = 1;
				check(rc >= 0, "\"shift\" returned an error");
			}
		} else {
			state->shift_counter = 0;
		}

		double pull = 1 / (state->timing.gravity / 256);
		rc = drop(state, state->phase_counter / pull);
		check(rc >= 0, "\"drop\" returned an error");
		if (rc < 2) {
			state->phase = LOCKING;
			state->phase_counter = 0;
			break;
		}

		if (input->down) {
			rc = drop(state, 1);
			check(rc >= 0, "\"drop\" returned an error");
			if (rc < 2) {
				rc = lock(state);
				check(rc >= 0, "\"lock\" returned an error");
				if (state->level % 100 != 99) {
					state->level++;
				}

				rc = clear(state);
				check(rc >= 0, "\"clear\" returned an error");
				if (rc > 0) {
					state->phase = CLEARING;
				} else {
					state->phase = LOADING;
				}
				state->phase_counter = 0;
				break;
			}
		} else if (input->up) {
			rc = drop(state, 20);
			check(rc >= 0, "\"drop\" returned an error");
			state->phase = LOCKING;
			state->phase_counter = 0;
			break;
		}

		state->phase_counter++;
		break;

	case LOCKING:
		if (input->rot_left_a) {
			rc = rotate(state, -1);
			check(rc >= 0, "\"rotate\" returned an error");
		}
		if (input->rot_left_b) {
			rc = rotate(state, -1);
			check(rc >= 0, "\"rotate\" returned an error");
		}
		if (input->rot_right) {
			rc = rotate(state, 1);
			check(rc >= 0, "\"rotate\" returned an error");
		}

		if (input->left) {
			if (last_input->left) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, -1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, -1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, -1);
					check(rc >= 0, "\"shift\" returned an error");
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, -1);
				check(rc >= 0, "\"shift\" returned an error");
				state->shift_counter = 1;
			}
		} else if (input->right) {
			if (last_input->right) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, 1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, 1);
					check(rc >= 0, "\"shift\" returned an error");
					rc = shift(state, 1);
					check(rc >= 0, "\"shift\" returned an error");
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, 1);
				check(rc >= 0, "\"shift\" returned an error");
				state->shift_counter = 1;
			}
		} else {
			state->shift_counter = 0;
		}

		rc = drop(state, 20);
		check(rc >= 0, "\"drop\" returned an error");
		if (rc > 0) {
			state->phase_counter = 0;
		}

		if (input->down) {
			rc = lock(state);
			check(rc >= 0, "\"lock\" returned an error");
			if (state->level % 100 != 99) {
				state->level++;
			}

			rc = clear(state);
			check(rc >= 0, "\"clear\" returned an error");
			if (rc > 0) {
				state->phase = CLEARING;
			} else {
				state->phase = LOADING;
			}
			state->phase_counter = 0;
			break;
		}

		state->phase_counter++;
		if (state->phase_counter == state->timing.lock) {
			rc = lock(state);
			check(rc >= 0, "\"lock\" returned an error");
			if (state->level % 100 != 99) {
				state->level++;
			}

			rc = clear(state);
			check(rc >= 0, "\"clear\" returned an error");
			if (rc > 0) {
				state->phase = CLEARING;
			} else {
				state->phase = LOADING;
			}
			state->phase_counter = 0;
			break;
		}

		break;

	case CLEARING:
		state->phase_counter++;
		if (state->phase_counter == state->timing.clear) {
			rc = destroy(state);
			check(rc >= 0, "\"destroy\" returned an error");
			state->level += rc;

			state->phase = LOADING;
			state->phase_counter = 0;
		}
		break;
	}

	*last_input = *input;
	return 1;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}
