#include <stdlib.h>
#include "movement.h"
#include "control.h"

Input_Map *last_input;

State *init_state()
{
	State *state = malloc(sizeof(State));
	state->level = 0;
	state->phase = LOADING;
	state->phase_counter = 0;
	state->shift_counter = 0;

	int rc = init_field(state);

	// temporary fixed values
	state->timing.gravity = 4;
	state->timing.load = 25;
	state->timing.load_clear = 25;
	state->timing.shift = 14;
	state->timing.lock = 30;
	state->timing.clear = 40;

	last_input = malloc(sizeof(Input_Map));
	last_input->left = 0;
	last_input->right = 0;
	last_input->up = 0;
	last_input->down = 0;
	last_input->rot_left_a = 0;
	last_input->rot_left_b = 0;
	last_input->rot_right = 0;

	return state;
}

void destroy_state(State *state)
{
	free(state);
	free(last_input);
}

int process(State *state, Input_Map *input)
{
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

			if (input->rot_left_a) {
				rc = rotate(state, -1);
			}
			if (input->rot_left_b) {
				rc = rotate(state, -1);
			}
			if (input->rot_right) {
				rc = rotate(state, 1);
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
		}
		if (input->rot_left_b) {
			rc = rotate(state, -1);
		}
		if (input->rot_right) {
			rc = rotate(state, 1);
		}

		if (input->left) {
			if (last_input->left) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, -1);
					rc = shift(state, -1);
					rc = shift(state, -1);
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, -1);
				state->shift_counter = 1;
			}
		} else if (input->right) {
			if (last_input->right) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, 1);
					rc = shift(state, 1);
					rc = shift(state, 1);
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, 1);
				state->shift_counter = 1;
			}
		} else {
			state->shift_counter = 0;
		}

		double pull = 1 / (state->timing.gravity / 256);
		rc = drop(state, state->phase_counter / pull);
		if (rc < 2) {
			state->phase = LOCKING;
			state->phase_counter = 0;
			break;
		}

		if (input->down) {
			rc = drop(state, 1);
			if (rc < 2) {
				rc = lock(state);
				if (state->level % 100 != 99) {
					state->level++;
				}

				rc = clear(state);
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
			state->phase = LOCKING;
			state->phase_counter = 0;
			break;
		}

		state->phase_counter++;
		break;

	case LOCKING:
		if (input->rot_left_a) {
			rc = rotate(state, -1);
		}
		if (input->rot_left_b) {
			rc = rotate(state, -1);
		}
		if (input->rot_right) {
			rc = rotate(state, 1);
		}

		if (input->left) {
			if (last_input->left) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, -1);
					rc = shift(state, -1);
					rc = shift(state, -1);
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, -1);
				state->shift_counter = 1;
			}
		} else if (input->right) {
			if (last_input->right) {
				if (state->shift_counter >= state->timing.shift) {
					rc = shift(state, 1);
					rc = shift(state, 1);
					rc = shift(state, 1);
				} else {
					state->shift_counter++;
				}
			} else {
				rc = shift(state, 1);
				state->shift_counter = 1;
			}
		} else {
			state->shift_counter = 0;
		}

		rc = drop(state, 20);
		if (rc > 0) {
			state->phase_counter = 0;
		}

		if (input->down) {
			rc = lock(state);
			if (state->level % 100 != 99) {
				state->level++;
			}

			rc = clear(state);
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
			if (state->level % 100 != 99) {
				state->level++;
			}

			rc = clear(state);
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
			state->phase = LOADING;
			state->phase_counter = 0;
		}
		break;
	}

	*last_input = *input;
	return 1;
}
