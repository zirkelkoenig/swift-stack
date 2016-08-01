#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "game_data.h"

ICoord* squarePositions;

ICoord* spawnPositions;

int GameData_init()
{
	ICoord squaresInit[EMPTY][NONE][4] = {{	// red
			{{0, 2}, {1, 2}, {2, 2}, {3, 2}},
			{{2, 0}, {2, 1}, {2, 2}, {2, 3}},
			{{0, 2}, {1, 2}, {2, 2}, {3, 2}},
			{{2, 0}, {2, 1}, {2, 2}, {2, 3}}
		}, {	// orange
			{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
			{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
			{{0, 0}, {1, 0}, {2, 0}, {2, 1}},
			{{1, 0}, {1, 1}, {1, 2}, {2, 0}}
		}, {	// blue
			{{0, 1}, {1, 1}, {2, 0}, {2, 1}},
			{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
			{{0, 0}, {0, 1}, {1, 0}, {2, 0}},
			{{1, 0}, {1, 1}, {1, 2}, {2, 2}}
		}, {	// yellow
			{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
			{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
			{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
			{{0, 0}, {0, 1}, {1, 0}, {1, 1}}
		}, {	// pink
			{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
			{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
			{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
			{{0, 1}, {0, 2}, {1, 0}, {1, 1}}
		}, {	// turquoise
			{{0, 1}, {1, 0}, {1, 1}, {2, 1}},
			{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
			{{0, 0}, {1, 0}, {1, 1}, {2, 0}},
			{{1, 0}, {1, 1}, {1, 2}, {2, 1}}
		}, {	// green
			{{0, 1}, {1, 0}, {1, 1}, {2, 0}},
			{{1, 0}, {1, 1}, {2, 1}, {2, 2}},
			{{0, 1}, {1, 0}, {1, 1}, {2, 0}},
			{{1, 0}, {1, 1}, {2, 1}, {2, 2}}
		}
	};

	squarePositions = calloc(EMPTY * NONE * 4, sizeof(ICoord));
	alloc_check(squarePositions);

	ICoord* rp = memcpy(squarePositions, &squaresInit, (EMPTY * NONE * 4) * sizeof(ICoord));
	cond_check(rp == squarePositions, "memory copy failed");

	ICoord spawnsInit[EMPTY] = {
		{3, 17},
		{3, 18},
		{3, 18},
		{4, 18},
		{3, 18},
		{3, 18},
		{3, 18}
	};

	spawnPositions = calloc(EMPTY, sizeof(ICoord));
	alloc_check(spawnPositions);

	rp = memcpy(spawnPositions, &spawnsInit, EMPTY * sizeof(ICoord));
	cond_check(rp == spawnPositions, "memory copy failed");

	return 0;
error:
	if(squarePositions) {
		free(squarePositions);
	}
	if(spawnPositions) {
		free(spawnPositions);
	}
	return -1;
}

const ICoord* GameData_getSquare(int color, int orientation, int num)
{
	cond_check((color >= RED) && (color < EMPTY), "argument \"color\" out of bounds");
	cond_check((orientation >= NORTH) && (orientation < NONE), "argument \"orientation\" out of bounds");
	cond_check((num >= 0) && (num < 4), "argument \"num\" out of bounds");
	global_check(squarePositions, "squarePositions");

	int index = color * (NONE * 4);
	index += orientation * 4;
	index += num;

	return &squarePositions[index];
error:
	return NULL;
}

const ICoord* GameData_getSpawn(int color)
{
	cond_check((color >= RED) && (color < EMPTY), "argument \"color\" out of bounds");
	global_check(spawnPositions, "spawnPositions");

	return &spawnPositions[color];
error:
	return NULL;
}

void GameData_destroy()
{
	free(squarePositions);
	free(spawnPositions);
}
