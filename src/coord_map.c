#include <stdlib.h>
#include <string.h>
#include "coord_map.h"

ICoord* map;

void CoordMap_init()
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
	
	int copyIndex = NONE * 4;
	int copySize = NONE * 4 * sizeof(ICoord);

	memcpy(&map[RED * copyIndex], &redCoords, copySize);
	memcpy(&map[ORANGE * copyIndex], &orangeCoords, copySize);
	memcpy(&map[BLUE * copyIndex], &blueCoords, copySize);
	memcpy(&map[YELLOW * copyIndex], &yellowCoords, copySize);
	memcpy(&map[PINK * copyIndex], &pinkCoords, copySize);
	memcpy(&map[TURQUOISE * copyIndex], &turquoiseCoords, copySize);
	memcpy(&map[GREEN * copyIndex], &greenCoords, copySize);
}

void CoordMap_destroy()
{
	free(map);
	map = NULL;
}

ICoord CoordMap_get(Square color, Orientation orientation, int num)
{
	int index = color * (NONE * 4);
	index += orientation * 4;
	index += num;

	return map[index];
}
