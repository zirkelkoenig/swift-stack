/*
Copyright (C) 2017 Marvin Kipping

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
License version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdlib.h>
#include "movement.h"
#include "control.h"
#include "dbg.h"

int last_direction = 0;
int last_rot_left_a = 0;
int last_rot_left_b = 0;
int last_rot_right = 0;
int last_clear = 0;

int process_rotate(State *state, int left_a, int left_b, int right);
int process_shift(State *state, int direction, int dummy);
int process_lock(State *state);

State *init_state()
{
	State *state = malloc(sizeof(State));
	check_mem(state);

	state->level = 0;
	state->phase = LOADING;
	state->phase_counter = 0;
	state->shift_counter = 0;
	state->drop_counter = 0;

	int rc = init_field(state);
	check(rc >= 0, "\"init_field\" returned an error");

	Timing *new_timing = get_timings(0);
	if (new_timing) {
		state->timing = *new_timing;
		free(new_timing);
	} else {
		sentinel("\"get_timings\" returned an error");
	}

	last_direction = 0;
	return state;

error:
	if (state) {
		free(state);
	}
	if (new_timing) {
		free(new_timing);
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

int process(State *state, Input_Map *input)
{
	int rc = 0;
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

	int cur_direction = 0;
	if (input->left) {
		cur_direction = -1;
	} else if (input->right) {
		cur_direction = 1;
	}

	switch (state->phase) {
	case LOADING:
		state->phase_counter++;

		// shift
		rc = process_shift(state, cur_direction, 1);
		check(rc >= 0, "\"process_shift\" returned an error");

		// spawn & initial rotation
		int phase_end = last_clear ? state->timing.load_clear : state->timing.load;
		if (state->phase_counter == phase_end) {
			int spawn_success = 0;
			rc = spawn(state);
			check(rc >= 0, "\"spawn\" returned an error");
			spawn_success += rc;

			rc = process_rotate(state, input->rot_left_a, input->rot_left_b, input->rot_right);
			check(rc >= 0, "\"process_rotate\" returned an error");
			spawn_success += rc;

			if (spawn_success) {
				state->phase = DROPPING;
				state->phase_counter = 0;
				state->drop_counter = 0;
			} else {
				return 0;
			}
		}
		break;

	case DROPPING:
		state->phase_counter++;

		// rotation
		rc = process_rotate(state,
				input->rot_left_a && !last_rot_left_a,
				input->rot_left_b && !last_rot_left_b,
				input->rot_right && !last_rot_right);
		check(rc >= 0, "\"process_rotate\" returned an error");

		// shift
		rc = process_shift(state, cur_direction, state->phase_counter == 1);
		check(rc >= 0, "\"process_shift\" returned an error");

		// gravity
		double pull = 1.0 / (state->timing.gravity / 256.0);
		int down = state->phase_counter / pull - state->drop_counter;
		rc = drop(state, down);
		check(rc >= 0, "\"drop\" returned an error");
		state->drop_counter += down;
		if (rc < 2) {
			state->phase = LOCKING;
			state->phase_counter = 0;
			break;
		}

		// soft drop & sonic drop
		if (input->down) {
			rc = drop(state, 1);
			check(rc >= 0, "\"drop\" returned an error");
			if (rc < 2) {
				rc = process_lock(state);
				check(rc >= 0, "\"process_lock\" returned an error");
			}
		} else if (input->up) {
			rc = drop(state, 20);
			check(rc >= 0, "\"drop\" returned an error");
			state->phase = LOCKING;
			state->phase_counter = 0;
		}
		break;

	case LOCKING:
		state->phase_counter++;

		// rotation
		rc = process_rotate(state,
				input->rot_left_a && !last_rot_left_a,
				input->rot_left_b && !last_rot_left_b,
				input->rot_right && !last_rot_right);
		check(rc >= 0, "\"process_rotate\" returned an error");

		// shift
		rc = process_shift(state, cur_direction, 0);
		check(rc >= 0, "\"process_shift\" returned an error");

		// gravity
		rc = drop(state, 20);
		check(rc >= 0, "\"drop\" returned an error");
		if (rc > 0) {
			state->phase_counter = 0;
		}

		// soft (locking) drop & delay locking
		if (input->down || state->phase_counter == state->timing.lock) {
			rc = process_lock(state);
			check(rc >= 0, "\"process_lock\" returned an error");
		}
		break;

	case CLEARING:
		state->phase_counter++;

		if (state->phase_counter == state->timing.clear) {
			rc = destroy(state);
			check(rc >= 0, "\"destroy\" returned an error");

			int i = 0;
			for (i = 0; i != rc; i++) {
				state->level++;
				Timing *new_timing = get_timings(state->level);
				if (new_timing) {
					state->timing = *new_timing;
					free(new_timing);
				}
			}

			if (state->level >= 999) {
				state->level = 999;
				return 2;
			}

			state->phase = LOADING;
			state->phase_counter = 0;
		}
		break;
	}

	last_direction = cur_direction;
	last_rot_left_a = input->rot_left_a;
	last_rot_left_b = input->rot_left_b;
	last_rot_right = input->rot_right;
	return 1;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}

/*
> 0 - success
0 - fail (collision)
*/
int process_rotate(State *state, int left_a, int left_b, int right)
{
	int rc = 0;
	check(state, "argument \"state\" uninitialized");

	int result = 0;
	if (left_a) {
		rc = rotate(state, -1);
		check(rc >= 0, "\"rotate\" returned an error");
		result += rc;
	}
	if (left_b) {
		rc = rotate(state, -1);
		check(rc >= 0, "\"rotate\" returned an error");
		result += rc;
	}
	if (right) {
		rc = rotate(state, 1);
		check(rc >= 0, "\"rotate\" returned an error");
		result += rc;
	}

	return result;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}

/*
dummy - only handle shift counter, do not actually shift
*/
int process_shift(State *state, int direction, int dummy)
{
	int rc = 0;
	check(state, "argument \"state\" uninitialized");

	if (direction) {
		if (direction == last_direction) {
			if (!dummy && state->shift_counter >= state->timing.shift) {
				rc = shift(state, direction);
				check(rc >= 0, "\"shift\" returned an error");
			}
			state->shift_counter++;
		} else {
			if (!dummy) {
				rc = shift(state, direction);
				check(rc >= 0, "\"shift\" returned an error");
			}
			state->shift_counter = 1;
		}
	} else {
		state->shift_counter = 0;
	}

	return 0;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}

int process_lock(State *state)
{
	int rc = 0;
	check(state, "argument \"state\" uninitialized");

	rc = lock(state);
	check(rc >= 0, "\"lock\" returned an error");

	if ((state->level % 100) != 99 && state->level != 998) {
		state->level++;
		Timing *new_timing = get_timings(state->level);
		if (new_timing) {
			state->timing = *new_timing;
			free(new_timing);
		}
	}

	rc = clear(state);
	check(rc >= 0, "\"clear\" returned an error");
	if (rc > 0) {
		state->phase = CLEARING;
		last_clear = 1;
	} else {
		state->phase = LOADING;
		last_clear = 0;
	}

	state->phase_counter = 0;
	return 0;

error:
	if (rc < 0) {
		return rc;
	}
	return -2;
}
