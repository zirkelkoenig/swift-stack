#include <stdio.h>
#include <SDL2/SDL.h>
#include "coord_map.h"
#include "error.h"
#include "logic.h"
#include "math_data.h"

#define PPS 32
#define GRAVITY 10.0
#define LOAD_DELAY 30
#define LOCK_DELAY 30
#define CLEAR_DELAY 30

enum Phase {
	P_LOAD,
	P_DROP,
	P_LOCK,
	P_CLEAR
};

uint8_t activeColors[DESTROYED + 1][4] = {
	{0xff, 0x00, 0x00, 0xff},
	{0xff, 0x77, 0x00, 0xff},
	{0x00, 0x00, 0xff, 0xff},
	{0xff, 0xff, 0x00, 0xff},
	{0xff, 0x00, 0xff, 0xff},
	{0x00, 0xff, 0xff, 0xff},
	{0x00, 0xff, 0x00, 0xff},
	{0x00, 0x00, 0x00, 0xff},
	{0xff, 0xff, 0xff, 0xff}
};

uint8_t lockingColors[DESTROYED + 1][4] = {
	{0xff, 0x77, 0x77, 0xff},
	{0xff, 0xbb, 0x77, 0xff},
	{0x77, 0x77, 0xff, 0xff},
	{0xff, 0xff, 0x77, 0xff},
	{0xff, 0x77, 0xff, 0xff},
	{0x77, 0xff, 0xff, 0xff},
	{0x77, 0xff, 0x77, 0xff},
	{0x00, 0x00, 0x00, 0xff},
	{0xff, 0xff, 0xff, 0xff}
};

uint8_t stackColors[DESTROYED + 1][4] = {
	{0xbb, 0x00, 0x00, 0xff},
	{0xdd, 0x77, 0x00, 0xff},
	{0x00, 0x00, 0xbb, 0xff},
	{0xdd, 0xdd, 0x00, 0xff},
	{0xdd, 0x00, 0xdd, 0xff},
	{0x00, 0xdd, 0xdd, 0xff},
	{0x00, 0xdd, 0x00, 0xff},
	{0x00, 0x00, 0x00, 0xff},
	{0xff, 0xff, 0xff, 0xff}
};

int phase;

void print_playfield();

int main(int argc, char *argv[])
{
	rc_check(init(), "init");
	phase = P_LOAD;

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
	int rc = 0;

	while(!quit) {
		switch(phase) {
		case P_LOAD:
			if(counter == LOAD_DELAY) {
				rc = nextPiece();
				rc_check(rc, "nextPiece");
				quit = !rc;

				//print_playfield();
				counter = 0;
				phase = P_DROP;
			} else {
				counter++;
			}
			break;

		case P_DROP:
			;
			int gDrop = counter / GRAVITY;
			rc = moveDown(-gDrop);
			rc_check(rc, "moveDown");

			if(rc) {
				counter = 0;
				phase = P_LOCK;
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
				rc_check(lock(), "lock");
				rc = markLines();
				rc_check(rc, "markLines");

				phase = rc ? P_CLEAR : P_LOAD;
				counter = 0;
			} else {
				counter++;
			}
			break;

		case P_CLEAR:
			if(counter == CLEAR_DELAY) {
				rc_check(clearLines(), "clearLines");

				phase = P_LOAD;
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
			}
		}

		sdl_check(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff) == 0);
		sdl_check(SDL_RenderClear(renderer) == 0);

		// active piece
		if((phase == P_DROP) || (phase == P_LOCK)) {
			SDL_Rect rects[4];
			for(i = 0; i < 4; i++) {
				const ICoord* rel = CoordMap_getSquare(activePiece->color, activePiece->orientation, i);
				cond_check(rel, "function \"CoordMap_getSquare\" returned an error");
				ICoord square = ICoord_shift(*rel, activePiece->position.x, activePiece->position.y);

				rects[i].x = square.x * PPS;
				rects[i].y = (FIELD_HEIGHT - square.y) * PPS;
				rects[i].w = PPS;
				rects[i].h = -PPS;
			}

			if(phase == P_LOCK) {
				int rc = SDL_SetRenderDrawColor(renderer,
						lockingColors[activePiece->color][0],
						lockingColors[activePiece->color][1],
						lockingColors[activePiece->color][2],
						lockingColors[activePiece->color][3]);
				sdl_check(rc == 0);
			} else {
				int rc = SDL_SetRenderDrawColor(renderer,
						activeColors[activePiece->color][0],
						activeColors[activePiece->color][1],
						activeColors[activePiece->color][2],
						activeColors[activePiece->color][3]);
				sdl_check(rc == 0);
			}

			sdl_check(SDL_RenderFillRects(renderer, rects, 4) == 0);
		}

		// stack
		for(i = 0; i < FIELD_HEIGHT; i++) {
			for(j = 0; j < FIELD_WIDTH; j++) {
				ICoord square = {j, i};
				int index = getFieldIndex(square);
				rc_check(index, "getFieldIndex");
				int color = playfield[index];
				if(color == EMPTY) {
					continue;
				}

				SDL_Rect rect = {
					square.x * PPS,
					(FIELD_HEIGHT - square.y) * PPS,
					PPS,
					-PPS
				};

				int rc = SDL_SetRenderDrawColor(renderer,
					stackColors[color][0],
					stackColors[color][1],
					stackColors[color][2],
					stackColors[color][3]);
				sdl_check(rc == 0);

				sdl_check(SDL_RenderFillRect(renderer, &rect) == 0);
			}
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

	destroy();
	return EXIT_SUCCESS;

sdlerr:
error:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	if(event) {
		free(event);
	}
	destroy();
	return EXIT_FAILURE;
}

void print_playfield()
{
	int i = 0;
	int j = 0;
	for(i = 0; i < FIELD_HEIGHT; i++) {
		for(j = 0; j < FIELD_WIDTH; j++) {
			ICoord square = {j, i};
			int index = getFieldIndex(square);
			rc_check(index, "getFieldIndex");

			if(playfield[index] == EMPTY) {
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
