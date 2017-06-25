#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "static_data.h"
#include "control.h"
#include "video_data.h"
#include "dbg.h"

#define check_sdl(A) if (!(A)) {\
	fprintf(stderr, "[SDL] (%s:%s:%d) %s\n", __FILE__, __FUNCTION__, __LINE__, SDL_GetError());\
	goto error;\
}

void print_field(State *state);

int main(int argc, char *argv[])
{
	int sdl_rc = 0;

	sdl_rc = SDL_Init(SDL_INIT_VIDEO);
	check_sdl(sdl_rc == 0);

	SDL_bool sdl_success = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	check(sdl_success == SDL_TRUE, "\"SDL_SetHint\" was unsuccessful");

	SDL_Window *window = SDL_CreateWindow(
			"Tetris",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			PPS * 10,
			PPS * 23,
			SDL_WINDOW_SHOWN);
	check_sdl(window);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	check_sdl(renderer);

	State *game = init_state();
	check(game, "\"init_state\" returned an error");

	SDL_Event *event = malloc(sizeof(SDL_Event));
	check_mem(event);

	int quit = 0;

	Input_Map *input = malloc(sizeof(Input_Map));
	check_mem(input);

	while (!quit) {
		const uint8_t *keyboard = SDL_GetKeyboardState(NULL);
		input->left = keyboard[SDL_SCANCODE_S];
		input->right = keyboard[SDL_SCANCODE_F];
		input->up = keyboard[SDL_SCANCODE_SPACE];
		input->down = keyboard[SDL_SCANCODE_D];
		input->rot_left_a = keyboard[SDL_SCANCODE_J];
		input->rot_left_b = keyboard[SDL_SCANCODE_L];
		input->rot_right = keyboard[SDL_SCANCODE_K];

		while (SDL_PollEvent(event)) {
			if (event->type == SDL_QUIT) {
				quit = 1;
				break;
			}
		}

		int rc = process(game, input);
		check(rc >= 0, "\"process\" returned an error");
		if (rc == 0 || rc == 2) {
			quit = 1;
		}

		sdl_rc = SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
		check_sdl(sdl_rc == 0);
		sdl_rc = SDL_RenderClear(renderer);
		check_sdl(sdl_rc == 0);

		if (game->phase == DROPPING || game->phase == LOCKING) {
			Piece *piece = &game->cur_piece;

			SDL_Rect blocks[4];
			int i = 0;
			for (i = 0; i != 4; i++) {
				blocks[i].x = piece->x_pos[i] * PPS;
				blocks[i].y = (23 - piece->y_pos[i]) * PPS;
				blocks[i].w = PPS;
				blocks[i].h = -PPS;
			}

			sdl_rc = SDL_SetRenderDrawColor(renderer,
					display_colors[game->phase][piece->color][0],
					display_colors[game->phase][piece->color][1],
					display_colors[game->phase][piece->color][2],
					display_colors[game->phase][piece->color][3]);
			check_sdl(sdl_rc == 0);
			sdl_rc = SDL_RenderFillRects(renderer, blocks, 4);
			check_sdl(sdl_rc == 0);
		}

		int i = 0;
		int j = 0;
		for (i = 0; i != 20; i++) {
			for (j = 0; j != 10; j++) {
				int color = game->field[j][i];
				if (color == EMPTY) {
					continue;
				}

				SDL_Rect block = { j * PPS, (23 - i) * PPS, PPS, -PPS };
				sdl_rc = SDL_SetRenderDrawColor(renderer,
						display_colors[2][color][0],
						display_colors[2][color][1],
						display_colors[2][color][2],
						display_colors[2][color][3]);
				check_sdl(sdl_rc == 0);
				sdl_rc = SDL_RenderFillRect(renderer, &block);
				check_sdl(sdl_rc == 0);
			}
		}

		sdl_rc = SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0x77, 0xff);
		check_sdl(sdl_rc == 0);
		for (i = 0; i != 10; i++) {
			SDL_Rect block = { i * PPS, 3 * PPS, PPS, -PPS };
			sdl_rc = SDL_RenderFillRect(renderer, &block);
			check_sdl(sdl_rc == 0);
		}

		Piece *preview = &game->next_piece;
		sdl_rc = SDL_SetRenderDrawColor(renderer,
				display_colors[DROPPING][preview->color][0],
				display_colors[DROPPING][preview->color][1],
				display_colors[DROPPING][preview->color][2],
				display_colors[DROPPING][preview->color][3]);
		check_sdl(sdl_rc == 0);
		SDL_Rect blocks[4];
		for (i = 0; i != 4; i++) {
			blocks[i].x = preview->x_pos[i] * PPS;
			blocks[i].y = (23 - preview->y_pos[i]) * PPS;
			blocks[i].w = PPS;
			blocks[i].h = -PPS;
		}
		sdl_rc = SDL_RenderFillRects(renderer, blocks, 4);
		check_sdl(sdl_rc == 0);

		SDL_RenderPresent(renderer);
		//print_field(game);
		SDL_Delay(16);
	}

	if (event) {
		free(event);
	}
	if (input) {
		free(input);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	destroy_state(game);
	return 0;

error:
	if (event) {
		free(event);
	}
	if (input) {
		free(input);
	}
	if (game) {
		destroy_state(game);
	}
	SDL_Quit();

	return 1;
}

void print_field(State *state)
{
	check(state, "argument \"state\" uninitialized");

	int i = 0;
	int j = 0;

	for (i = 0; i != 23; i++) {
		for (j = 0; j != 10; j++) {
			int color = state->field[j][22 - i];
			if (color == EMPTY) {
				putchar('.');
			} else {
				putchar('#');
			}
		}
		putchar('\n');
	}
	putchar('\n');

error:
	; // do nothing
}
