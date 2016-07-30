#ifndef _math_data_h
#define _math_data_h

#define PI 3.14159265

typedef struct ICoord {
	int x;
	int y;
} ICoord;

ICoord ICoord_shift(ICoord coord, int x, int y);

#endif
