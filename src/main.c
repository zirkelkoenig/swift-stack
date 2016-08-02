#include <stdio.h>
#include <SDL2/SDL.h>
#include "error.h"
#include "game_data.h"
#include "logic.h"
#include "math_data.h"

#define PPS 32

void print_playfield(State* game);

int main(int argc, char *argv[])
{
	State* game = newGame();
	rc_check(game, "newGame");

	sdl_check(SDL_Init(SDL_INIT_VIDEO) == 0);
	sdl_check(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == SDL_TRUE);

	SDL_Window* window = SDL_CreateWindow("Tetris",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			FIELD_WIDTH * PPS,
			FIELD_HEIGHT * PPS,
			SDL_WINDOW_SHOWN);
	sdl_check(window);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	sdl_check(renderer);

	int quit = 0;
	int counter = 0;
	SDL_Event* event = malloc(sizeof(SDL_Event));
	int i = 0;
	int j = 0;
	int k = 0;
	int rc = 0;
	const uint8_t *squareColor[4];

	while(!quit) {
		switch(game->phase) {
		case P_LOAD:
			if(counter == LOAD_DELAY) {
				rc = nextPiece(game);
				rc_check(rc, "nextPiece");
				quit = !rc;

				//print_playfield();
				counter = 0;
				game->phase = P_DROP;
			} else {
				counter++;
			}
			break;

		case P_DROP:
			;
			int gDrop = counter / GRAVITY;
			rc = moveDown(game, -gDrop);
			rc_check(rc, "moveDown");

			if(rc) {
				counter = 0;
				game->phase = P_LOCK;
			} else {
				if(gDrop) {
					counter = 0;
				} else {
					counter++;
				}
			}
			break;

		case P_LOCK:
			if(counter == LOCK_DELAY) {
				rc_check(lock(game), "lock");
				rc = markLines(game);
				rc_check(rc, "markLines");

				game->phase = rc ? P_CLEAR : P_LOAD;
				counter = 0;
			} else {
				rc = moveDown(game, -20);
				rc_check(rc, "moveDown");
				counter++;
			}
			break;

		case P_CLEAR:
			if(counter == CLEAR_DELAY) {
				rc_check(clearLines(game), "clearLines");

				game->phase = P_LOAD;
				counter = 0;
			} else {
				counter++;
			}
			break;
		}

		while(SDL_PollEvent(event)) {
			switch(event->type) {
			case SDL_QUIT:
				quit = 1;
				break;

			case SDL_KEYDOWN:
				if((game->phase != P_DROP) && (game->phase != P_LOCK)) {
					break;
				}
				switch(event->key.keysym.scancode) {
				case SDL_SCANCODE_J:
				case SDL_SCANCODE_L:
					rc_check(rotateLeft(game), "rotateLeft");
					break;

				case SDL_SCANCODE_K:
					rc_check(rotateRight(game), "rotateRight");
					break;

				case SDL_SCANCODE_F:
					rc_check(moveHorizontal(game, 1), "moveHorizontal");
					break;

				case SDL_SCANCODE_S:
					rc_check(moveHorizontal(game, -1), "moveHorizontal");
					break;

				case SDL_SCANCODE_D:
					rc = moveDown(game, -1);
					rc_check(moveDown, "moveDown");

					if(rc) {
						game->phase = P_LOCK;
						counter = 0;
					}
					break;

				case SDL_SCANCODE_E:
					rc_check(moveDown(game, -20), "moveDown");
					game->phase = P_LOCK;
					counter = 0;
					break;

				default:	// do nothing
					break;
				}
				break;
			}
		}

		sdl_check(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff) == 0);
		sdl_check(SDL_RenderClear(renderer) == 0);

		// active piece
		if((game->phase == P_DROP) || (game->phase == P_LOCK)) {
			SDL_Rect rects[4];
			for(i = 0; i < 4; i++) {
				const ICoord* rel = GameData_getSquare(game->activePiece.color,
						game->activePiece.orientation, i);
				cond_check(rel, "function \"GameData_getSquare\" returned an error");
				ICoord square = ICoord_shift(*rel, game->activePiece.position.x,
						game->activePiece.position.y);

				rects[i].x = square.x * PPS;
				rects[i].y = (FIELD_HEIGHT - square.y) * PPS;
				rects[i].w = PPS;
				rects[i].h = -PPS;
			}

			for(i = 0; i < 4; i++) {
				squareColor[i] = GameData_getColor(game->phase - 1, game->activePiece.color, i);
				cond_check(squareColor[i], "function \"GameData_getColor\" returned an error");
			}

			int rc = SDL_SetRenderDrawColor(renderer,
					*squareColor[0],
					*squareColor[1],
					*squareColor[2],
					*squareColor[3]);
			sdl_check(rc == 0);

			sdl_check(SDL_RenderFillRects(renderer, rects, 4) == 0);
		}

		// stack
		for(i = 0; i < FIELD_HEIGHT; i++) {
			for(j = 0; j < FIELD_WIDTH; j++) {
				int index = getFieldIndex(j, i);
				rc_check(index, "getFieldIndex");
				int color = game->playfield[index];
				if(color == C_EMPTY) {
					continue;
				}

				SDL_Rect rect = {
					j * PPS,
					(FIELD_HEIGHT - i) * PPS,
					PPS,
					-PPS
				};

				for(k = 0; k < 4; k++) {
					squareColor[k] = GameData_getColor(P_LOCK, color, k);
					cond_check(squareColor[k], "function \"GameData_getColor\" returned an error");
				}

				int rc = SDL_SetRenderDrawColor(renderer,
						*squareColor[0],
						*squareColor[1],
						*squareColor[2],
						*squareColor[3]);
				sdl_check(rc == 0);

				sdl_check(SDL_RenderFillRect(renderer, &rect) == 0);
			}
		}

		// upper boundary
		for(i = 0; i < FIELD_WIDTH; i++) {
			SDL_Rect rect = {
				i * PPS,
				(FIELD_HEIGHT - 20) * PPS,
				PPS,
				-PPS
			};

			int rc = SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0x77, 0xff);
			sdl_check(rc == 0);
			sdl_check(SDL_RenderFillRect(renderer, &rect) == 0);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	if(event) {
		free(event);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	destroyGame(game);
	return EXIT_SUCCESS;

sdlerr:
error:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	if(event) {
		free(event);
	}
	destroyGame(game);
	return EXIT_FAILURE;
}

void print_playfield(State* game)
{
	int i = 0;
	int j = 0;
	for(i = 0; i < FIELD_HEIGHT; i++) {
		for(j = 0; j < FIELD_WIDTH; j++) {
			int index = getFieldIndex(j, i);
			rc_check(index, "getFieldIndex");

			if(game->playfield[index] == C_EMPTY) {
				printf(". ");
			} else {
				printf("# ");
			}

			//printf("%d ", playfield[index]);
		}
		printf("\b\n");
	}
	printf("\n");

error:
	;	// empty
}
