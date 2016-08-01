#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "game_data.h"

ICoord* squarePositions;

ICoord* spawnPositions;

uint8_t* colors;

int GameData_init()
{	// subscopes to avoid blowing the stack
	{
		ICoord squaresInit[C_EMPTY][O_NONE][4] = {{	// red
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

		squarePositions = calloc(C_EMPTY * O_NONE * 4, sizeof(ICoord));
		alloc_check(squarePositions);

		ICoord* rp = memcpy(squarePositions, &squaresInit, (C_EMPTY * O_NONE * 4) * sizeof(ICoord));
		cond_check(rp == squarePositions, "memory copy failed");
	}

	{
		ICoord spawnsInit[C_EMPTY] = {
			{3, 17},
			{3, 18},
			{3, 18},
			{4, 18},
			{3, 18},
			{3, 18},
			{3, 18}
		};

		spawnPositions = calloc(C_EMPTY, sizeof(ICoord));
		alloc_check(spawnPositions);

		ICoord* rp = memcpy(spawnPositions, &spawnsInit, C_EMPTY * sizeof(ICoord));
		cond_check(rp == spawnPositions, "memory copy failed");
	}

	{
		uint8_t colorsInit[P_CLEAR][C_DESTROYED + 1][4] = {{	// dropping
				{0xff, 0x00, 0x00, 0xff},
				{0xff, 0x77, 0x00, 0xff},
				{0x00, 0x00, 0xff, 0xff},
				{0xff, 0xff, 0x00, 0xff},
				{0xff, 0x00, 0xff, 0xff},
				{0x00, 0xff, 0xff, 0xff},
				{0x00, 0xff, 0x00, 0xff},
				{0x00, 0x00, 0x00, 0xff},
				{0xff, 0xff, 0xff, 0xff}
			}, {	// locking
				{0xff, 0x77, 0x77, 0xff},
				{0xff, 0xbb, 0x77, 0xff},
				{0x77, 0x77, 0xff, 0xff},
				{0xff, 0xff, 0x77, 0xff},
				{0xff, 0x77, 0xff, 0xff},
				{0x77, 0xff, 0xff, 0xff},
				{0x77, 0xff, 0x77, 0xff},
				{0x00, 0x00, 0x00, 0xff},
				{0xff, 0xff, 0xff, 0xff}
			}, {	// locked
				{0xbb, 0x00, 0x00, 0xff},
				{0xdd, 0x77, 0x00, 0xff},
				{0x00, 0x00, 0xbb, 0xff},
				{0xdd, 0xdd, 0x00, 0xff},
				{0xdd, 0x00, 0xdd, 0xff},
				{0x00, 0xdd, 0xdd, 0xff},
				{0x00, 0xdd, 0x00, 0xff},
				{0x00, 0x00, 0x00, 0xff},
				{0xff, 0xff, 0xff, 0xff}
			}
		};

		colors = calloc(P_CLEAR * (C_DESTROYED + 1) * 4, sizeof(uint8_t));
		alloc_check(colors);

		uint8_t* rp = memcpy(colors, &colorsInit, (P_CLEAR * (C_DESTROYED + 1) * 4) * sizeof(uint8_t));
		cond_check(rp  == colors, "memory copy failed");
	}
	return 0;
error:
	if(squarePositions) {
		free(squarePositions);
	}
	if(spawnPositions) {
		free(spawnPositions);
	}
	if(colors) {
		free(colors);
	}
	return -1;
}

const ICoord* GameData_getSquare(int color, int orientation, int num)
{
	cond_check((color >= C_RED) && (color < C_EMPTY), "argument \"color\" out of bounds");
	cond_check((orientation >= O_NORTH) && (orientation < O_NONE), "argument \"orientation\" out of bounds");
	cond_check((num >= 0) && (num < 4), "argument \"num\" out of bounds");
	global_check(squarePositions, "squarePositions");

	int index = color * (O_NONE * 4);
	index += orientation * 4;
	index += num;

	return &squarePositions[index];
error:
	return NULL;
}

const ICoord* GameData_getSpawn(int color)
{
	cond_check((color >= C_RED) && (color < C_EMPTY), "argument \"color\" out of bounds");
	global_check(spawnPositions, "spawnPositions");

	return &spawnPositions[color];
error:
	return NULL;
}

void GameData_destroy()
{
	free(squarePositions);
	free(spawnPositions);
	free(colors);
}

const uint8_t* GameData_getColor(int phase, int color, int num)
{
	cond_check((phase >= P_LOAD) && (phase < P_CLEAR), "argument \"phase\" out of bounds");
	cond_check((color >= C_RED) && (color <= C_DESTROYED), "argument \"color\" out of bounds");
	cond_check((num >= 0) && (num < 4), "argument \"num\" out of bounds");
	global_check(colors, "colors");

	int index = phase * ((C_DESTROYED + 1) * 4);
	index += color * 4;
	index += num;

	return &colors[index];
error:
	return NULL;
}
