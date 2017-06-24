#include <stdlib.h>
#include "movement.h"
#include "control.h"
#include "dbg.h"

int last_direction = 0;

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

	last_direction = 0;
	return state;

error:
	if (state) {
		free(state);
	}
	return NULL;
}

void destroy_state(State *state)
{
	check_debug(state, "argument \"state\" uninitialized");

	free(state);

error:
	;// do nothing
}

int process_rotations(State *state, Input_Map *input)
{
	check(state, "argument \"state\" uninitialized");
	check(input, "argument \"input\" uninitialized");

	int rc = 0;
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

	return rc;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}

int process_shift(State *state, Input_Map *input, int direction)
{
	check(state, "argument \"state\" uninitialized");
	check(input, "argument \"input\" uninitialized");

	int rc = 0;

	if (direction) {
		if (direction == last_direction) {
			if (state->shift_counter >= state->timing.shift) {
				rc = shift(state, direction);
				check(rc >= 0, "\"shift\" returned an error");
				rc = shift(state, direction);
				check(rc >= 0, "\"shift\" returned an error");
				rc = shift(state, direction);
				check(rc >= 0, "\"shift\" returned an error");
			} else {
				state->shift_counter++;
			}
		} else {
			rc = shift(state, direction);
			check(rc >= 0, "\"shift\" returned an error");
			state->shift_counter = 1;
		}
	} else {
		state->shift_counter = 0;
	}

	return rc;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}

int process_lock(State *state)
{
	check(state, "argument \"state\" uninitialized");

	int rc = lock(state);
	check(rc >= 0, "\"lock\" returned an error");

	if (state->level % 100 != 99) {
		state->level++;
	}

	rc = clear(state);
	check(rc >= 0, "\"clear\" returned an error");

	state->phase = (rc > 0) ? CLEARING : LOADING;
	state->phase_counter = 0;

	return rc;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}

int process(State *state, Input_Map *input)
{
	check(state, "argument \"state\" uninitialized");
	check(state, "argument \"input\" uninitialized");

	if (input->left && input->right) {
		input->left = 0;
		input->right = 0;
	}
	if (input->up && input->down) {
		input->up = 0;
		input->down = 0;
	}

	int rc = 0;
	int cur_direction = 0;
	if (input->left) {
		cur_direction = -1;
	} else if (input->right) {
		cur_direction = 1;
	}

	switch (state->phase) {
	case LOADING:
		if (cur_direction) {
			if (cur_direction == last_direction) {
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

			rc = process_rotations(state, input);
			check(rc >= 0, "\"process_rotations\" returned an error");
			if (rc == 0) {
				return 0;
			}

			state->phase = DROPPING;
			state->phase_counter = 0;
		}
		break;

	case DROPPING:
		rc = process_rotations(state, input);
		check(rc >= 0, "\"process_rotations\" returned an error");

		rc = process_shift(state, input, cur_direction);
		check(rc >= 0, "\"process_shift\" returned an error");

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
				rc = process_lock(state);
				check(rc >= 0, "\"process_lock\" returned an error");
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
		rc = process_rotations(state, input);
		check(rc >= 0, "\"process_rotations\" returned an error");

		rc = process_shift(state, input, cur_direction);
		check(rc >= 0, "\"process_shift\" returned an error");

		rc = drop(state, 20);
		check(rc >= 0, "\"drop\" returned an error");
		if (rc > 0) {
			state->phase_counter = 0;
		}

		state->phase_counter++;
		if (input->down || state->phase_counter == state->timing.lock) {
			rc = process_lock(state);
			check(rc >= 0, "\"process_lock\" returned an error");
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

	last_direction = cur_direction;
	return 1;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}
