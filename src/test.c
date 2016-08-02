#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "game_data.h"
#include "logic.h"

/* Macro function which performs the test and prints out the return value if it differs from the expectations.
	P - name of the return type's printing function
	F - name of the function to test
	R - expected test result
	... - arguments that are passed to the tested function
*/
#define test_function(P, F, R, ...)\
	res = (P)((R));\
	act = (P)((F)(__VA_ARGS__));\
	if((!res && act) || (res && !act) || (res && act && strcmp(res , act))) {\
		fprintf(stdout, "Failed test for function \"%s\"!\n", #F);\
		fprintf(stdout, "\tExpected result: %s\n", res);\
		fprintf(stdout, "\tActual result: %s\n", act);\
		fprintf(stdout, "\tLine: %d\n\n", __LINE__);\
	}\
	if(res) {free(res); res = NULL;}\
	if(act) {free(act); act = NULL;}

/* Globals for the test_function macro. Do not alter by hand! */
char* res = NULL;
char* act = NULL;

/* Combines different values into a string according to a format. The size of the returned string has to be large
enough. Returns NULL on error. */
char* make_string(size_t size, const char* format, ...);

// printing functions
char* print_ICoord(ICoord data);
char* print_ICoord_p(const ICoord* data);
char* print_int(int value);
char* print_Piece_p(Piece* piece);
char* print_uint8_t_p(const uint8_t* value);

// testing functions
void test_ICoord_shift();

void test_GameData_init();
void test_GameData_getSquare();
void test_GameData_getSpawn();
void test_GameData_getColor();

//void test_newGame();
void test_getFieldIndex();
void test_nextPiece();
void test_lock();
void test_moveHorizontal();
void test_moveDown();
void test_rotateLeft();
void test_rotateRight();
void test_markLines();
void test_clearLines();

int main(int argc, char *argv[])
{
	test_ICoord_shift();
	test_GameData_init();
	test_GameData_getSquare();
	test_GameData_getSpawn();
	test_GameData_getColor();
	//test_newGame();
	test_getFieldIndex();
	test_nextPiece();
	test_lock();
	test_moveHorizontal();
	test_moveDown();
	test_rotateLeft();
	test_rotateRight();
	test_markLines();
	test_clearLines();

	return EXIT_SUCCESS;
}

char* make_string(size_t size, const char* format, ...)
{
	char* result = calloc(size, sizeof(char));
	alloc_check(result);

	va_list ap;
	va_start(ap, format);
	int newSize = vsprintf(result, format, ap);
	rc_check(newSize, "vsprintf");
	va_end(ap);

	char* rp = realloc(result, (newSize + 1) * sizeof(char));
	alloc_check(rp);
	result = rp;
	return result;

error:
	if(result) {
		free(result);
	}
	return NULL;
}

void test_ICoord_shift()
{
	ICoord arg_coord = {84932, -24365};
	int arg_x = -90;
	int arg_y = 329;
	ICoord result = {84842, -24036};

	test_function(print_ICoord, ICoord_shift, result, arg_coord, arg_x, arg_y);
}

void test_GameData_init()
{
	test_function(print_int, GameData_init, 0);
	GameData_destroy();
}

void test_GameData_getSquare()
{
	int i = 0;
	int j = 0;
	int k = 0;

	struct TestData {
		int arg_color;
		int arg_orientation;
		int arg_num;
		ICoord* result;
	};

	int numTests = 8;
	struct TestData tests[numTests];

	// valid - boundary checks
	tests[0].arg_color = C_RED;
	tests[0].arg_orientation = O_NORTH;
	tests[0].arg_num = 0;
	tests[0].result = malloc(sizeof(ICoord));
	alloc_check(tests[0].result);
	tests[0].result->x = 0;
	tests[0].result->y = 2;

	tests[1].arg_color = C_RED;
	tests[1].arg_orientation = O_NORTH;
	tests[1].arg_num = 3;
	tests[1].result = malloc(sizeof(ICoord));
	alloc_check(tests[1].result);
	tests[1].result->x = 3;
	tests[1].result->y = 2;

	tests[2].arg_color = C_RED;
	tests[2].arg_orientation = O_WEST;
	tests[2].arg_num = 0;
	tests[2].result = malloc(sizeof(ICoord));
	alloc_check(tests[2].result);
	tests[2].result->x = 2;
	tests[2].result->y = 0;

	tests[3].arg_color = C_RED;
	tests[3].arg_orientation = O_WEST;
	tests[3].arg_num = 3;
	tests[3].result = malloc(sizeof(ICoord));
	alloc_check(tests[3].result);
	tests[3].result->x = 2;
	tests[3].result->y = 3;

	tests[4].arg_color = C_GREEN;
	tests[4].arg_orientation = O_NORTH;
	tests[4].arg_num = 0;
	tests[4].result = malloc(sizeof(ICoord));
	alloc_check(tests[4].result);
	tests[4].result->x = 0;
	tests[4].result->y = 1;

	tests[5].arg_color = C_GREEN;
	tests[5].arg_orientation = O_NORTH;
	tests[5].arg_num = 3;
	tests[5].result = malloc(sizeof(ICoord));
	alloc_check(tests[5].result);
	tests[5].result->x = 2;
	tests[5].result->y = 0;

	tests[6].arg_color = C_GREEN;
	tests[6].arg_orientation = O_WEST;
	tests[6].arg_num = 0;
	tests[6].result = malloc(sizeof(ICoord));
	alloc_check(tests[6].result);
	tests[6].result->x = 1;
	tests[6].result->y = 0;

	tests[7].arg_color = C_GREEN;
	tests[7].arg_orientation = O_WEST;
	tests[7].arg_num = 3;
	tests[7].result = malloc(sizeof(ICoord));
	alloc_check(tests[7].result);
	tests[7].result->x = 2;
	tests[7].result->y = 2;

	rc_check(GameData_init(), "GameData_init");

	for(i = 0; i < numTests; i++) {
		test_function(print_ICoord_p, GameData_getSquare,
				tests[i].result,
				tests[i].arg_color,
				tests[i].arg_orientation,
				tests[i].arg_num);
	}

	// invalid via loops
	int invColors[6] = {C_EMPTY, C_DESTROYED, 9, 12354, -1, -54778};
	int invOrientations[5] = {O_NONE, 6, 432170, -1, -93478};
	int invNums[4] = {4, 53278, -1, -157890};

	for(i = 0; i < 6; i++) {
		test_function(print_ICoord_p, GameData_getSquare, NULL,
				invColors[i],
				O_EAST,
				1);
	}

	for(i = 0; i < 5; i++) {
		test_function(print_ICoord_p, GameData_getSquare, NULL,
				C_ORANGE,
				invOrientations[i],
				1);
	}

	for(i = 0; i < 4; i++) {
		test_function(print_ICoord_p, GameData_getSquare, NULL,
				C_ORANGE,
				O_EAST,
				invNums[i]);
	}

	for(i = 0; i < 6; i++) {
		for(j = 0; j < 5; j++) {
			test_function(print_ICoord_p, GameData_getSquare, NULL,
					invColors[i],
					invOrientations[j],
					1);
		}
	}

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 4; j++) {
			test_function(print_ICoord_p, GameData_getSquare, NULL,
					C_ORANGE,
					invOrientations[i],
					invNums[j]);
		}
	}

	for(i = 0; i < 6; i++) {
		for(j = 0; j < 5; j++) {
			for(k = 0; k < 4; k++) {
				test_function(print_ICoord_p, GameData_getSquare, NULL,
						invColors[i],
						invOrientations[j],
						invNums[k]);
			}
		}
	}

error:	// fallthrough
	GameData_destroy();
	for(i = 0; i < numTests; i++) {
		if(tests[i].result) {
			free(tests[i].result);
		}
	}
}

void test_GameData_getColor()
{
	int i = 0;
	int j = 0;
	int k = 0;

	struct TestData {
		int arg_phase;
		int arg_color;
		int arg_num;
		uint8_t result;
	};

	int numTests = 8;
	struct TestData tests[numTests];

	// valid (boundary checks)
	tests[0].arg_phase = P_LOAD;
	tests[0].arg_color = C_RED;
	tests[0].arg_num = 0;
	tests[0].result = 0xff;

	tests[1].arg_phase = P_LOAD;
	tests[1].arg_color = C_RED;
	tests[1].arg_num = 3;
	tests[1].result = 0xff;

	tests[2].arg_phase = P_LOAD;
	tests[2].arg_color = C_DESTROYED;
	tests[2].arg_num = 0;
	tests[2].result = 0xff;

	tests[3].arg_phase = P_LOAD;
	tests[3].arg_color = C_DESTROYED;
	tests[3].arg_num = 3;
	tests[3].result = 0xff;

	tests[4].arg_phase = P_LOCK;
	tests[4].arg_color = C_RED;
	tests[4].arg_num = 0;
	tests[4].result = 0xbb;

	tests[5].arg_phase = P_LOCK;
	tests[5].arg_color = C_RED;
	tests[5].arg_num = 3;
	tests[5].result = 0xff;

	tests[6].arg_phase = P_LOCK;
	tests[6].arg_color = C_DESTROYED;
	tests[6].arg_num = 0;
	tests[6].result = 0xff;

	tests[7].arg_phase = P_LOCK;
	tests[7].arg_color = C_DESTROYED;
	tests[7].arg_num = 3;
	tests[7].result = 0xff;

	rc_check(GameData_init(), "GameData_init");

	for(i = 0; i < numTests; i++) {
		test_function(print_uint8_t_p, GameData_getColor,
				&tests[i].result,
				tests[i].arg_phase,
				tests[i].arg_color,
				tests[i].arg_num);
	}

	// invalid via loops
	int invPhases[5] = {P_CLEAR, 4, 472389, -1, -8905};
	int invColors[4] = {-1, -43280954, 9, 78923};
	int invNums[4] = {-1, -823467, 4, 6724386};

	for(i = 0; i < 5; i++) {
		test_function(print_uint8_t_p, GameData_getColor, NULL,
				invPhases[i],
				C_YELLOW,
				2);
	}

	for(i = 0; i < 4; i++) {
		test_function(print_uint8_t_p, GameData_getColor, NULL,
				P_DROP,
				invColors[i],
				2);
	}

	for(i = 0; i < 4; i++) {
		test_function(print_uint8_t_p, GameData_getColor, NULL,
				P_DROP,
				C_YELLOW,
				invNums[i]);
	}

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 4; j++) {
			test_function(print_uint8_t_p, GameData_getColor, NULL,
					invPhases[i],
					invColors[j],
					2);
		}
	}

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			test_function(print_uint8_t_p, GameData_getColor, NULL,
					P_DROP,
					invColors[i],
					invNums[j]);
		}
	}

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				test_function(print_uint8_t_p, GameData_getColor, NULL,
						invPhases[i],
						invColors[j],
						invNums[k]);
			}
		}
	}

error:	// fallthrough
	GameData_destroy();
}

char* print_ICoord(ICoord data)
{
	return make_string(2 * (3 * sizeof(int)), "{%d, %d}", data.x, data.y);
}

char* print_ICoord_p(const ICoord* data)
{
	if(!data) {
		return NULL;
	} else {
		char* result = print_ICoord(*data);
		return result;
	}
}

char* print_int(int value)
{
	return make_string(6 * sizeof(int), "%d", value);
}

char* print_uint8_t_p(const uint8_t* value)
{
	if(value) {
		return make_string(3 * sizeof(uint8_t), "%X", *value);
	} else {
		return NULL;
	}
}

void test_getFieldIndex()
{
	int numTests = 3;
	int arg_x[numTests];
	int arg_y[numTests];
	int result[numTests];
	int i = 0;
	int j = 0;

	// valid
	arg_x[0] = 0;
	arg_y[0] = 0;
	result[0] = 0;

	arg_x[1] = 9;
	arg_y[1] = 19;
	result[1] = 199;

	arg_x[2] = 7;
	arg_y[2] = 12;
	result[2] = 127;

	for(i = 0; i < numTests; i++) {
		test_function(print_int, getFieldIndex, result[i], arg_x[i], arg_y[i]);
	}

	// invalid loops

	int invX[4] = {-1, -253789, 10, 57089};
	int invY[4] = {-1, -52370952, 23, 7482398};

	for(i = 0; i < 4; i++) {
		test_function(print_int, getFieldIndex, -1, 7, invY[i]);
	}

	for(i = 0; i < 4; i++) {
		test_function(print_int, getFieldIndex, -1, invX[i], 12);
	}

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			test_function(print_int, getFieldIndex, -1, invX[i], invY[i]);
		}
	}
}

char* print_Piece_p(Piece* piece)
{
	return make_string(4 * (3 * sizeof(int)), "%d, %d, {%d, %d}",
			piece->color, piece->orientation, piece->position.x, piece->position.y);
}

void test_nextPiece()
{
	State* game = newGame();
	rc_check(game, "newGame");

	int i = 0;
	test_function(print_int, nextPiece, 1, game);

	// manipulate playfield to simulate collisions
	ICoord blockPositions[4] = {
		{4, 19},
		{5, 19},
		{4, 18},
		{5, 18}
	};

	for(i = 0; i < 4; i++) {
		int index = getFieldIndex(blockPositions[i].x, blockPositions[i].y);
		rc_check(index, "getFieldIndex");
		game->playfield[index] = C_YELLOW;
	}
	game->activePiece.color = C_EMPTY;

	test_function(print_int, nextPiece, 0, game);

error:	// fallthrough
	destroyGame(game);
}

void test_lock()
{
	State* game = newGame();
	rc_check(game, "newGame");
	rc_check(nextPiece(game), "nextPiece");
	test_function(print_int, lock, 0, game);

	test_function(print_int, lock, -1, game);

	rc_check(nextPiece(game), "nextPiece");
	test_function(print_int, lock, 0, game);

error:	// fallthrough
	destroyGame(game);
}

void test_moveHorizontal()
{
	State* game = newGame();
	rc_check(game, "newGame");
	rc_check(nextPiece(game), "nextPiece");

	test_function(print_int, moveHorizontal, 0, game, 0);
	test_function(print_int, moveHorizontal, 0, game, 1);
	test_function(print_int, moveHorizontal, 0, game, -2);
	test_function(print_int, moveHorizontal, 0, game, 4);
	test_function(print_int, moveHorizontal, 0, game, -4);
	test_function(print_int, moveHorizontal, 1, game, 10);
	test_function(print_int, moveHorizontal, 1, game, -10);

	rc_check(lock(game), "lock");	// lock at left border
	test_function(print_int, moveHorizontal, -1, game, -4);

	rc_check(nextPiece(game), "nextPiece");
	test_function(print_int, moveHorizontal, 1, game, -3);

error:	// fallthrough
	destroyGame(game);
}

void test_moveDown()
{
	State* game = newGame();
	rc_check(game, "newGame");
	rc_check(nextPiece(game), "nextPiece");

	test_function(print_int, moveDown, 0, game, 0);
	test_function(print_int, moveDown, 0, game, -18);
	test_function(print_int, moveDown, 0, game, 2);
	test_function(print_int, moveDown, 1, game, -2);

	rc_check(lock(game), "lock");
	test_function(print_int, moveDown, -1, game, -4);

	rc_check(nextPiece(game), "nextPiece");
	test_function(print_int, moveDown, 1, game, -20);

error:	// fallthrough
	destroyGame(game);
}

void test_rotateLeft()
{
	State* game = newGame();
	rc_check(game, "newGame");
	rc_check(nextPiece(game), "nextPiece");
	rc_check(moveDown(game, -2), "moveDown");
	test_function(print_int, rotateLeft, 0, game);

	// manipulate active Piece to be an I at the left wall to simulate wall collision
	game->activePiece.color = C_RED;
	game->activePiece.orientation = O_EAST;
	game->activePiece.position.x = -2;
	game->activePiece.position.y = 12;
	test_function(print_int, rotateLeft, 0, game);

	rc_check(lock(game), "lock");
	test_function(print_int, rotateLeft, -1, game);

	rc_check(nextPiece(game), "nextPiece");
	rc_check(moveDown(game, -2), "moveDown");
	test_function(print_int, rotateLeft, 0, game);

error:	// fallthrough
	destroyGame(game);
}

void test_rotateRight()
{
	State* game = newGame();
	rc_check(game, "newGame");
	rc_check(nextPiece(game), "nextPiece");
	rc_check(moveDown(game, -2), "moveDown");
	test_function(print_int, rotateRight, 0, game);

	// manipulate active Piece to be an I at the left wall to simulate wall collision
	game->activePiece.color = C_RED;
	game->activePiece.orientation = O_EAST;
	game->activePiece.position.x = -2;
	game->activePiece.position.y = 12;
	test_function(print_int, rotateRight, 0, game);

	rc_check(lock(game), "lock");
	test_function(print_int, rotateRight, -1, game);

	rc_check(nextPiece(game), "nextPiece");
	rc_check(moveDown(game, -2), "moveDown");
	test_function(print_int, rotateRight, 0, game);

error:	// fallthrough
	destroyGame(game);
}

void test_markLines()
{
	State* game = newGame();
	rc_check(game, "newGame");
	test_function(print_int, markLines, 0, game);

	// manipulate playfield to contain 1 marked line
	int line_start = 4 * FIELD_WIDTH;
	int i = 0;
	for(i = 0; i < FIELD_WIDTH; i++) {
		game->playfield[line_start + i] = C_BLUE;
	}
	test_function(print_int, markLines, 1, game);
	test_function(print_int, markLines, 0, game);

	// free playfield to force error
	free(game->playfield);
	game->playfield = NULL;
	test_function(print_int, markLines, -1, game);

error:	// fallthrough
	destroyGame(game);
}

void test_clearLines()
{
	State* game = newGame();
	rc_check(game, "newGame");
	test_function(print_int, clearLines, 0, game);

	// manipulate playfield to contain 1 marked line
	int line_start = 4 * FIELD_WIDTH;
	int i = 0;
	for(i = 0; i < FIELD_WIDTH; i++) {
		game->playfield[line_start + i] = C_DESTROYED;
	}
	test_function(print_int, clearLines, 1, game);
	test_function(print_int, clearLines, 0, game);

	// free playfield to force error
	free(game->playfield);
	game->playfield = NULL;
	test_function(print_int, clearLines, -1, game);

error:	// fallthrough
	destroyGame(game);
}

void test_GameData_getSpawn()
{
	ICoord results[3] = {
		{3, 17},
		{3, 18},
		{3, 18}
	};
	int arg_color[3] = {
		C_RED,
		C_GREEN,
		C_BLUE
	};

	rc_check(GameData_init(), "GameData_init");

	int i = 0;
	for(i = 0; i < 3; i++) {
		test_function(print_ICoord_p, GameData_getSpawn, &results[i], arg_color[i]);
	}

	test_function(print_ICoord_p, GameData_getSpawn, NULL, -1);
	test_function(print_ICoord_p, GameData_getSpawn, NULL, -49874);
	test_function(print_ICoord_p, GameData_getSpawn, NULL, C_EMPTY);
	test_function(print_ICoord_p, GameData_getSpawn, NULL, 9);
	test_function(print_ICoord_p, GameData_getSpawn, NULL, 789532);

	GameData_destroy();
error:
	;	// nothing
}
