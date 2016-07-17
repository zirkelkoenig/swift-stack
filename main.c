#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define ROWS 20
#define COLS 10
#define GSIZE 32

enum Shapes {
	T_I,
	T_S,
	T_Z,
	T_O,
	T_T,
	T_L,
	T_J
};

enum Directions {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

/*
uint8_t colors[][] = {
	{0xff, 0x00, 0x00, 0xff},
	{0xff, 0x55, 0xbb, 0xff},
	{0x00, 0xff, 0x00, 0xff},
	{0xff, 0xff, 0x00, 0xff},
	{0x00, 0xff, 0xff, 0xff},
	{0xff, 0x77, 0x00, 0xff},
	{0x00, 0x00, 0xff, 0xff}
};
*/

struct Shape {
	uint8_t color[4];
	int numSquares;
	SDL_Rect* squares;
	int direction;
};

SDL_Rect gridNudge(SDL_Rect rectangle, int x, int y)
{
	struct SDL_Rect newRect = {
		rectangle.x + (x * GSIZE) + x,
		rectangle.y + (y * GSIZE) + y,
		rectangle.w,
		rectangle.h
	};
	return newRect;
}

struct Shape* buildShapes()
{
	struct Shape* shapes = calloc(2, sizeof(struct Shape));
	SDL_Rect baseRect = {1, 1, GSIZE, GSIZE};

	// I
	shapes[0].color[0] = 0xff;
	shapes[0].color[1] = 0x00;
	shapes[0].color[2] = 0x00;
	shapes[0].color[3] = 0xff;
	shapes[0].direction = NORTH;
	shapes[0].squares = calloc(4, sizeof(SDL_Rect));
	shapes[0].squares[0] = gridNudge(baseRect, 0, 0);
	shapes[0].squares[1] = gridNudge(baseRect, 1, 0);
	shapes[0].squares[2] = gridNudge(baseRect, 2, 0);
	shapes[0].squares[3] = gridNudge(baseRect, 3, 0);
	shapes[0].numSquares = 4;

	// S
	shapes[1].color[0] = 0xff;
	shapes[1].color[1] = 0x55;
	shapes[1].color[2] = 0xbb;
	shapes[1].color[3] = 0xff;
	shapes[1].direction = NORTH;
	shapes[1].squares = calloc(4, sizeof(SDL_Rect));
	shapes[1].squares[0] = gridNudge(baseRect, 5, 6);
	shapes[1].squares[1] = gridNudge(baseRect, 6, 6);
	shapes[1].squares[2] = gridNudge(baseRect, 6, 5);
	shapes[1].squares[3] = gridNudge(baseRect, 7, 5);
	shapes[1].numSquares = 4;

	return shapes;
}

int main(int argc, char* argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		goto error;
	}
	if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == SDL_FALSE) {
		printf("Hint could not be set!\n");
		return EXIT_FAILURE;
	}

	SDL_Window* window = SDL_CreateWindow("Tetris",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			(COLS * GSIZE) + (COLS + 1),
			(ROWS * GSIZE) + (ROWS + 1),
			SDL_WINDOW_SHOWN);
	if(window == NULL) {
		goto error;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		goto error;
	}
	if(SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff) != 0) {
		goto error;
	}

	int quit = 0;
	SDL_Event* event = malloc(sizeof(SDL_Event));
	if(event == NULL) {
		printf("Could not allocate memory!\n");
		return EXIT_FAILURE;
	}

	struct Shape* shapes = buildShapes();

	while(!quit) {
		while(SDL_PollEvent(event)) {
			if(event->type == SDL_QUIT) {
				quit = 1;
			}
		}

		// reset renderer
		if(SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff)
				!= 0) {
			goto error;
		}
		if(SDL_RenderClear(renderer) != 0) {
			goto error;
		}

		// draw grid
		if(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff)
				!= 0) {
			goto error;
		}
		int i = 0;
		for(i = 0; i < (COLS + 1); i++) {
			int rc = SDL_RenderDrawLine(renderer,
					(i * GSIZE) + i, 0,
					(i * GSIZE) + i, (ROWS * GSIZE) + ROWS);
			if(rc != 0) {
				goto error;
			}
		}

		for(i = 0; i < (ROWS + 1); i++) {
			int rc = SDL_RenderDrawLine(renderer,
					0, (i * GSIZE) + i,
					(COLS * GSIZE) + COLS, (i * GSIZE) + i);
			if(rc != 0) {
				goto error;
			}
		}

		// draw shapes
		SDL_SetRenderDrawColor(renderer,
			shapes[0].color[0],
			shapes[0].color[1],
			shapes[0].color[2],
			shapes[0].color[3]);
		SDL_RenderFillRects(renderer, shapes[0].squares,
				shapes[0].numSquares);

		SDL_SetRenderDrawColor(renderer,
			shapes[1].color[0],
			shapes[1].color[1],
			shapes[1].color[2],
			shapes[1].color[3]);
		SDL_RenderFillRects(renderer, shapes[1].squares,
				shapes[1].numSquares);

		SDL_RenderPresent(renderer);
	}

	if(event) {
		free(event);
	}
	free(shapes);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;

error:
	if(window) {
		SDL_DestroyWindow(window);
	}
	if(renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if(event) {
		free(event);
	}

	printf("SDL Error: %s\n", SDL_GetError());
	return EXIT_FAILURE;
}
