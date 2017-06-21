#ifndef _static_data_h
#define _static_data_h

/* The color value is used not only to get a piece color, but identifies the piece's type itself. */
enum Color {
	RED,
	ORANGE,
	BLUE,
	YELLOW,
	PINK,
	TURQUOISE,
	GREEN,
	// the next ones are for field states only
	EMPTY,
	DESTROYED
};

/* A piece's orientation. */
enum Orientation {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

/* The phases of the main loop. */
enum Phase {
	DROPPING,
	LOCKING,
	CLEARING,
	LOADING
};

/*
A piece with it's color, orientation and block positions on the field, which are ordered "column (line)", starting from
the lower left.
*/
typedef struct Piece {
	int color;
	int orientation;
	int x_pos[4];
	int y_pos[4];
} Piece;

/* A boolean map of currently pressed input buttons. */
typedef struct Input_Map {
	int left;
	int right;
	int up;
	int down;
	int rot_left_a;
	int rot_left_b;
	int rot_right;
} Input_Map;

/*
The state of a game including the current phase with a frame counter, the piece (Color) history, the currently active
piece and the field, wich is ordered "lines => columns" starting from the lower left with 3 extra lines on top, which
shouldn't be drawn.
*/
typedef struct State {
	int phase;
	int phase_counter;
	int history[4];
	int field[10][23];
	Piece cur_piece;
} State;

/*
All possible piece rotations in clockwise direction starting with NORTH orientation. To get the
counter-clockwise rotation, just negate the values of the inverted rotation. Ordered by: Color => Orientation. Block
ordering is congruent to the Piece struct.
*/
extern const int x_rotations[7][4][4];
extern const int y_rotations[7][4][4];

/* All piece prototypes in their initial positions. Ordered by: Color */
extern const Piece init_pieces[7];

#endif
