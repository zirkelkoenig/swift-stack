#ifndef _logic_h
#define _logic_h

#include "math_data.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define RAND_TRIES 4
#define HISTORY 4

// Types
typedef enum Square {
	RED,
	ORANGE,
	BLUE,
	YELLOW,
	PINK,
	TURQUOISE,
	GREEN,
	EMPTY
} Square;

typedef enum Orientation {
	NORTH,
	EAST,
	SOUTH,
	WEST
} Orientation;

typedef struct Piece {
	Square color;
	ICoord position;
	Orientation orientation;
	ICoord squares[4];
} Piece;

// Globals
Square* playfield;

Square* pieceHistory;

Piece* prototypes;

Piece* activePiece;

// Functions
int init();

int getFieldNum(ICoord square);

Piece* nextPiece();

void move(Piece* piece, int x, int y);

#endif
