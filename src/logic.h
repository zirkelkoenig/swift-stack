#ifndef _logic_h
#define _logic_h

#include "game_data.h"
#include "math_data.h"

/* Creates a new game and returns its state. Returns NULL on error. */
State* newGame();

/* Returns the index in the playfield array for a given position and -1 on errors. */
int getFieldIndex(ICoord square);

/* Pseudo-randomly generates and spawns the next piece. Returns 0 when placement failes due to collisions (= game
over), 1 on succes and -1 on error. */
int nextPiece(State* state);

/* Marks the active piece's current position on the playfield. Returns 0 on
success and a negative integer on error. */
int lock(State* state);

/* Moves the active piece along the horizontal axis by x columns. Breaks and returns 1 once the piece becomes
blocked. Returns 0 on success and -1 on error. */
int moveHorizontal(State* state, int x);

/* Moves the active piece along the vertical axis by negative y lines. Positive values for y are ignored. Breaks and
returns 1 once the piece becomes blocked. Returns 0 on success and -1 on error. */
int moveDown(State* state, int y);

/* Rotates the piece by 90 degrees if possible. Returns 0 on success and a negative integer on error. */
int rotateLeft(State* state);

/* Rotates the piece by -90 degrees if possible. Returns 0 on success and a negative integer on error. */
int rotateRight(State* state);

/* Destroys the playfield and frees all resources. */
void destroyGame(State* state);

/* Marks completed lines that are about to be destroyed and returns their quantity. Returns a negative integer on
error. */
int markLines(State* state);

/* Clears all previously marked lines and returns their quantity. Returns a negative integer on error. */
int clearLines(State* state);

#endif
