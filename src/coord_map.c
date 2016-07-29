#include <stdlib.h>
#include <string.h>
#include "coord_map.h"
#include "error.h"

ICoord* map;

int CoordMap_init()
{
	ICoord redCoords[NONE][4] = {
		{{0, 2}, {1, 2}, {2, 2}, {3, 2}},
		{{2, 0}, {2, 1}, {2, 2}, {2, 3}},
		{{0, 2}, {1, 2}, {2, 2}, {3, 2}},
		{{2, 0}, {2, 1}, {2, 2}, {2, 3}}
	};
	ICoord orangeCoords[NONE][4] = {
		{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
		{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
		{{0, 0}, {1, 0}, {2, 0}, {2, 1}},
		{{1, 0}, {1, 1}, {1, 2}, {2, 0}}
	};
	ICoord blueCoords[NONE][4] = {
		{{0, 1}, {1, 1}, {2, 0}, {2, 1}},
		{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
		{{0, 0}, {0, 1}, {1, 0}, {2, 0}},
		{{1, 0}, {1, 1}, {1, 2}, {2, 2}}
	};
	ICoord yellowCoords[NONE][4] = {
		{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
		{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
		{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
		{{0, 0}, {0, 1}, {1, 0}, {1, 1}}
	};
	ICoord pinkCoords[NONE][4] = {
		{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
		{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
		{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
		{{0, 1}, {0, 2}, {1, 0}, {1, 1}}
	};
	ICoord turquoiseCoords[NONE][4] = {
		{{0, 1}, {1, 0}, {1, 1}, {2, 1}},
		{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
		{{0, 0}, {1, 0}, {1, 1}, {2, 0}},
		{{1, 0}, {1, 1}, {1, 2}, {2, 1}}
	};
	ICoord greenCoords[NONE][4] = {
		{{0, 1}, {1, 0}, {1, 1}, {2, 0}},
		{{1, 0}, {1, 1}, {2, 1}, {2, 2}},
		{{0, 1}, {1, 0}, {1, 1}, {2, 0}},
		{{1, 0}, {1, 1}, {2, 1}, {2, 2}}
	};

	int size = EMPTY * NONE * 4;
	map = calloc(size, sizeof(ICoord));
	alloc_check(map);
	
	int copyIndex = NONE * 4;
	int copySize = NONE * 4 * sizeof(ICoord);

	ICoord* rp = NULL;
	rp = memcpy(&map[RED * copyIndex], &redCoords, copySize);
	cond_check(rp == &map[RED * copyIndex], "memory copy failed");

	rp = memcpy(&map[ORANGE * copyIndex], &orangeCoords, copySize);
	cond_check(rp == &map[ORANGE * copyIndex], "memory copy failed");

	rp = memcpy(&map[BLUE * copyIndex], &blueCoords, copySize);
	cond_check(rp == &map[BLUE * copyIndex], "memory copy failed");

	rp = memcpy(&map[YELLOW * copyIndex], &yellowCoords, copySize);
	cond_check(rp == &map[YELLOW * copyIndex], "memory copy failed");

	rp = memcpy(&map[PINK * copyIndex], &pinkCoords, copySize);
	cond_check(rp == &map[PINK * copyIndex], "memory copy failed");

	rp = memcpy(&map[TURQUOISE * copyIndex], &turquoiseCoords, copySize);
	cond_check(rp == &map[TURQUOISE * copyIndex], "memory copy failed");

	rp = memcpy(&map[GREEN * copyIndex], &greenCoords, copySize);
	cond_check(rp == &map[GREEN * copyIndex], "memory copy failed");

	return 0;

error:
	if(map) {
		free(map);
		map = NULL;
	}
	return -1;
}

void CoordMap_destroy()
{
	if(map) {
		free(map);
		map = NULL;
	}
}

ICoord* CoordMap_get(Square color, Orientation orientation, int num)
{
	cond_check((color >= 0) && (color < EMPTY), "argument \"color\" out of bounds");
	cond_check((orientation >= 0) && (orientation < NONE), "argument \"orientation\" out of bounds");

	int index = color * (NONE * 4);
	index += orientation * 4;
	index += num;

	ICoord* result = malloc(sizeof(ICoord));
	alloc_check(result);
	result->x = map[index].x;
	result->y = map[index].y;
	return result;

error:
	return NULL;
}
