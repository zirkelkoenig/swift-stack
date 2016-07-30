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
