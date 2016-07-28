#ifndef _logic_h
#define _logic_h

#include "math_data.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define RAND_TRIES 4
#define HISTORY 4

typedef enum Square {
	RED,
	ORANGE,
	BLUE,
	YELLOW,
	PINK,
	TURQUOISE,
	GREEN,
	EMPTY,
	DESTROYED
} Square;

typedef enum Orientation {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NONE
} Orientation;

typedef struct Piece {
	Square color;
	Orientation orientation;
	ICoord position;
} Piece;

Square* playfield;

Piece* activePiece;

Phase fieldState;

/* Initializes the playfield and all globals. Sets the playfield's state to INIT. */
int init();

/* Returns the index in the playfield array for a given position. */
int getFieldIndex(ICoord square);

/* Pseudo-randomly generates and spawns the next piece. Returns 0 when placement failes due to collisions (= game
over) */
int nextPiece();

/* Marks the active piece's current position on the playfield, then destroys it and frees its resources */
void lock();

/* Moves the active piece along the horizontal axis by x columns. Breaks and returns 1 once the piece becomes
blocked. */
int moveHorizontal(int x);

/* Moves the active piece along the vertical axis by negative y lines. Positive values for y are ignored. Breaks and
returns 1 once the piece becomes blocked. */
int moveDown(int y);

/* Rotates the piece by 90 degrees if possible. */
void rotateLeft();

/* Rotates the piece by -90 degrees if possible. */
void rotateRight();

/* Destroys the playfield and frees all resources. */
void destroy();

/* Marks completed lines that are about to be destroyed. */
void markLines();

/* Clears all previously marked lines and returns their quantity */
int clearLines();

#endif
