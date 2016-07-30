#ifndef _logic_h
#define _logic_h

#include "math_data.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define RAND_TRIES 4
#define HISTORY 4

enum Square {
	RED,
	ORANGE,
	BLUE,
	YELLOW,
	PINK,
	TURQUOISE,
	GREEN,
	EMPTY,
	DESTROYED
};

enum Orientation {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NONE
};

typedef struct Piece {
	int color;
	int orientation;
	ICoord position;
} Piece;

int* playfield;

Piece* activePiece;

/* Initializes the playfield and all globals. Sets the playfield's state to INIT. Returns 0 on success and a non-
negative integer on error. */
int init();

/* Returns the index in the playfield array for a given position and -1 on errors. */
int getFieldIndex(ICoord square);

/* Pseudo-randomly generates and spawns the next piece. Returns 0 when placement failes due to collisions (= game
over), 1 on succes and -1 on error. */
int nextPiece();

/* Marks the active piece's current position on the playfield, then destroys it and frees its resources. Returns 0 on
success and a negative integer on error. */
int lock();

/* Moves the active piece along the horizontal axis by x columns. Breaks and returns 1 once the piece becomes
blocked. Returns 0 on success and -1 on error. */
int moveHorizontal(int x);

/* Moves the active piece along the vertical axis by negative y lines. Positive values for y are ignored. Breaks and
returns 1 once the piece becomes blocked. Returns 0 on success and -1 on error. */
int moveDown(int y);

/* Rotates the piece by 90 degrees if possible. Returns 0 on success and a negative integer on error. */
int rotateLeft();

/* Rotates the piece by -90 degrees if possible. Returns 0 on success and a negative integer on error. */
int rotateRight();

/* Destroys the playfield and frees all resources. */
void destroy();

/* Marks completed lines that are about to be destroyed and returns their quantity. Returns a negative integer on
error. */
int markLines();

/* Clears all previously marked lines and returns their quantity. Returns a negative integer on error. */
int clearLines();

#endif
