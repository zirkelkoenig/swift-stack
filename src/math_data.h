#ifndef _math_data_h
#define _math_data_h

typedef struct ICoord {
	int x;
	int y;
} ICoord;

typedef struct DCoord {
	double x;
	double y;
} DCoord;

ICoord ICoord_shift(ICoord coord, int x, int y);

DCoord DCoord_shift(DCoord coord, double x, double y);

DCoord DCoord_rotate(DCoord coord, double angle);

ICoord DCoord_toInt(DCoord coord);

DCoord ICoord_toDouble(ICoord coord);

DCoord DCoord_midpoint(DCoord* coords, int num);

#endif
