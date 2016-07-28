#include <math.h>
#include <stdlib.h>
#include "math_data.h"

ICoord ICoord_shift(ICoord coord, int x, int y)
{
	ICoord result = {
		coord.x + x,
		coord.y + y
	};
	return result;
}

DCoord DCoord_shift(DCoord coord, double x, double y)
{
	DCoord result = {
		coord.x + x,
		coord.y + y
	};
	return result;
}

DCoord DCoord_rotate(DCoord coord, double angle)
{
	double phi = fmod(angle, 360.0) * (PI / 180.0);

	DCoord result = {
		coord.x * cos(phi) - coord.y * sin(phi),
		coord.x * sin(phi) + coord.y * cos(phi)
	};
	return result;
}

ICoord DCoord_toInt(DCoord coord)
{
	double* garbage = malloc(sizeof(double));

	ICoord result = {
		(modf(coord.x, garbage) > 0.5) ? ceil(coord.x) : floor(coord.x),
		(modf(coord.y, garbage) > 0.5) ? ceil(coord.y) : floor(coord.y)
	};

	free(garbage);
	return result;
}

DCoord ICoord_toDouble(ICoord coord)
{
	DCoord result = {
		coord.x,
		coord.y
	};
	return result;
}

DCoord DCoord_midpoint(DCoord* coords, int num)
{
	DCoord result = {
		0.0,
		0.0
	};

	int i = 0;
	for(i = 0; i < num; i++) {
		result.x += coords[i].x / (double)num;
		result.y += coords[i].y / (double)num;
	}
	return result;
}
