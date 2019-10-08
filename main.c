/*
Copyright (C) 2017 Marvin Kipping

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; version 2 only.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <SDL.h>
#include <SDL_ttf.h>
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

#define check_ttf(A) if (!(A)) {\
	fprintf(stderr, "[TTF] (%s:%s:%d) %s\n", __FILE__, __FUNCTION__, __LINE__, TTF_GetError());\
	goto error;\
}

void print_field(State *state);

int main(int argc, char *argv[])
{
	int sdl_rc = 0;

	sdl_rc = SDL_Init(SDL_INIT_VIDEO);
	check_sdl(sdl_rc == 0);
	sdl_rc = TTF_Init();
	check_ttf(sdl_rc >= 0);

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

	const char *font_path = "font/NotoMono-hinted/NotoMono-Regular.ttf";
	TTF_Font *font = TTF_OpenFont(font_path, 28);
	check_ttf(font);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	SDL_Color text_color = { 255, 255, 255, 255 };


	State *game = init_state();
	check(game, "\"init_state\" returned an error");

	SDL_Event *event = malloc(sizeof(SDL_Event));
	check_mem(event);

	int quit = 0;

	Input_Map *input = malloc(sizeof(Input_Map));
	check_mem(input);
	char *level_string = calloc(14, sizeof(char));
	check_mem(level_string);

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

		sdl_rc = SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xff);
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

		sdl_rc = SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0x80);
		check_sdl(sdl_rc == 0);
		for (i = 0; i != 10; i++) {
			SDL_Rect block = { i * PPS, 3 * PPS, PPS, -PPS };
			sdl_rc = SDL_RenderFillRect(renderer, &block);
			check_sdl(sdl_rc == 0);
		}

		int dst_level = game->level >= 900 ? 999 : ((game->level / 100) + 1) * 100;
		rc = snprintf(level_string, 14, "LEVEL %3d/%3d", game->level, dst_level);
		check(rc == 13, "error creating level string");
		SDL_Surface *string_surface = TTF_RenderText_Blended(font, level_string, text_color);
		check_ttf(string_surface);
		SDL_Texture *string_texture = SDL_CreateTextureFromSurface(renderer, string_surface);
		check_sdl(string_texture);

		int text_width = 0;
		int text_height = 0;
		sdl_rc = SDL_QueryTexture(string_texture, NULL, NULL, &text_width, &text_height);
		check_sdl(sdl_rc == 0);

		SDL_Rect text_rect = { 0, 2 * PPS, text_width, text_height };
		sdl_rc = SDL_RenderCopy(renderer, string_texture, NULL, &text_rect);
		check_sdl(sdl_rc == 0);

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
		SDL_Delay(16);
	}

	if (event) {
		free(event);
	}
	if (input) {
		free(input);
	}
	if (level_string) {
		free(level_string);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
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
	if (font) {
		TTF_CloseFont(font);
	}
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	if (level_string) {
		free(level_string);
	}

	TTF_Quit();
	SDL_Quit();
	return 1;
}
