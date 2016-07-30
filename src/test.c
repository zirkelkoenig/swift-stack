#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord_map.h"
#include "error.h"
#include "logic.h"
#include "math_data.h"

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
char* print_ICoord_p(ICoord* data);
char* print_int(int value);
char* print_Piece_p(Piece* piece);

// testing functions
void test_ICoord_shift();

void test_CoordMap_init();
void test_CoordMap_get();

void test_init();
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
	test_CoordMap_init();
	test_CoordMap_get();
	test_init();
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

void test_CoordMap_init()
{
	test_function(print_int, CoordMap_init, 0);
	CoordMap_destroy();
}

void test_CoordMap_get()
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
	tests[0].arg_color = RED;
	tests[0].arg_orientation = NORTH;
	tests[0].arg_num = 0;
	tests[0].result = malloc(sizeof(ICoord));
	alloc_check(tests[0].result);
	tests[0].result->x = 0;
	tests[0].result->y = 2;

	tests[1].arg_color = RED;
	tests[1].arg_orientation = NORTH;
	tests[1].arg_num = 3;
	tests[1].result = malloc(sizeof(ICoord));
	alloc_check(tests[1].result);
	tests[1].result->x = 3;
	tests[1].result->y = 2;

	tests[2].arg_color = RED;
	tests[2].arg_orientation = WEST;
	tests[2].arg_num = 0;
	tests[2].result = malloc(sizeof(ICoord));
	alloc_check(tests[2].result);
	tests[2].result->x = 2;
	tests[2].result->y = 0;

	tests[3].arg_color = RED;
	tests[3].arg_orientation = WEST;
	tests[3].arg_num = 3;
	tests[3].result = malloc(sizeof(ICoord));
	alloc_check(tests[3].result);
	tests[3].result->x = 2;
	tests[3].result->y = 3;

	tests[4].arg_color = GREEN;
	tests[4].arg_orientation = NORTH;
	tests[4].arg_num = 0;
	tests[4].result = malloc(sizeof(ICoord));
	alloc_check(tests[4].result);
	tests[4].result->x = 0;
	tests[4].result->y = 1;

	tests[5].arg_color = GREEN;
	tests[5].arg_orientation = NORTH;
	tests[5].arg_num = 3;
	tests[5].result = malloc(sizeof(ICoord));
	alloc_check(tests[5].result);
	tests[5].result->x = 2;
	tests[5].result->y = 0;

	tests[6].arg_color = GREEN;
	tests[6].arg_orientation = WEST;
	tests[6].arg_num = 0;
	tests[6].result = malloc(sizeof(ICoord));
	alloc_check(tests[6].result);
	tests[6].result->x = 1;
	tests[6].result->y = 0;

	tests[7].arg_color = GREEN;
	tests[7].arg_orientation = WEST;
	tests[7].arg_num = 3;
	tests[7].result = malloc(sizeof(ICoord));
	alloc_check(tests[7].result);
	tests[7].result->x = 2;
	tests[7].result->y = 2;

	rc_check(CoordMap_init(), "CoordMap_init");

	for(i = 0; i < numTests; i++) {
		test_function(print_ICoord_p, CoordMap_get,
				tests[i].result,
				tests[i].arg_color,
				tests[i].arg_orientation,
				tests[i].arg_num);
	}

	// invalid via loops
	int invColors[6] = {EMPTY, DESTROYED, 9, 12354, -1, -54778};
	int invOrientations[5] = {NONE, 6, 432170, -1, -93478};
	int invNums[4] = {4, 53278, -1, -157890};

	for(i = 0; i < 6; i++) {
		test_function(print_ICoord_p, CoordMap_get, NULL,
				invColors[i],
				EAST,
				1);
	}

	for(i = 0; i < 5; i++) {
		test_function(print_ICoord_p, CoordMap_get, NULL,
				ORANGE,
				invOrientations[i],
				1);
	}

	for(i = 0; i < 4; i++) {
		test_function(print_ICoord_p, CoordMap_get, NULL,
				ORANGE,
				EAST,
				invNums[i]);
	}

	for(i = 0; i < 6; i++) {
		for(j = 0; j < 5; j++) {
			test_function(print_ICoord_p, CoordMap_get, NULL,
					invColors[i],
					invOrientations[j],
					1);
		}
	}

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 4; j++) {
			test_function(print_ICoord_p, CoordMap_get, NULL,
					ORANGE,
					invOrientations[i],
					invNums[j]);
		}
	}

	for(i = 0; i < 6; i++) {
		for(j = 0; j < 5; j++) {
			for(k = 0; k < 4; k++) {
				test_function(print_ICoord_p, CoordMap_get, NULL,
						invColors[i],
						invOrientations[j],
						invNums[k]);
			}
		}
	}

	// freeing tests[n]->result is not necessary here, they are freed by the printing function
	CoordMap_destroy();
	return;

error:
	CoordMap_destroy();
	for(i = 0; i < numTests; i++) {
		if(tests[i].result) {
			free(tests[i].result);
		}
	}
}

char* print_ICoord(ICoord data)
{
	return make_string(2 * (3 * sizeof(int)), "{%d, %d}", data.x, data.y);
}

char* print_ICoord_p(ICoord* data)
{
	if(!data) {
		return NULL;
	} else {
		char* result = print_ICoord(*data);
		free(data);
		data = NULL;
		return result;
	}
}

char* print_int(int value)
{
	return make_string(3 * sizeof(int), "%d", value);
}

void test_init()
{
	test_function(print_int, init, 0);
	destroy();
}

void test_getFieldIndex()
{
	int numTests = 3;
	ICoord arg_square[numTests];
	int result[numTests];
	int i = 0;
	int j = 0;

	// valid
	arg_square[0].x = 0;
	arg_square[0].y = 0;
	result[0] = 0;

	arg_square[1].x = 9;
	arg_square[1].y = 19;
	result[1] = 199;

	arg_square[2].x = 7;
	arg_square[2].y = 12;
	result[2] = 127;

	for(i = 0; i < numTests; i++) {
		test_function(print_int, getFieldIndex, result[i], arg_square[i]);
	}

	// invalid loops

	int invX[4] = {-1, -253789, 10, 57089};
	int invY[4] = {-1, -52370952, 20, 7482398};

	for(i = 0; i < 4; i++) {
		ICoord arg = {7, invY[i]};
		test_function(print_int, getFieldIndex, -1, arg);
	}

	for(i = 0; i < 4; i++) {
		ICoord arg = {invX[i], 12};
		test_function(print_int, getFieldIndex, -1, arg);
	}

	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			ICoord arg = {invX[i], invY[j]};
			test_function(print_int, getFieldIndex, -1, arg);
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
	init();
	int i = 0;
	test_function(print_int, nextPiece, 1);

	// manipulate playfield to simulate collisions
	ICoord blockPositions[4] = {
		{4, 19},
		{5, 19},
		{4, 18},
		{5, 18}
	};

	for(i = 0; i < 4; i++) {
		int index = getFieldIndex(blockPositions[i]);
		rc_check(index, "getFieldIndex");
		playfield[index] = YELLOW;
	}

	test_function(print_int, nextPiece, 0);

error:	// fallthrough
	destroy();
}

void test_lock()
{
	rc_check(init(), "init");
	rc_check(nextPiece(), "nextPiece");
	test_function(print_int, lock, 0);

	test_function(print_int, lock, -1);

	rc_check(nextPiece(), "nextPiece");
	test_function(print_int, lock, 0);

error:	// fallthrough
	destroy();
}

void test_moveHorizontal()
{
	rc_check(init(), "init");
	rc_check(nextPiece(), "nextPiece");
	test_function(print_int, moveHorizontal, 0, 0);
	test_function(print_int, moveHorizontal, 0, 1);
	test_function(print_int, moveHorizontal, 0, -2);
	test_function(print_int, moveHorizontal, 0, 4);
	test_function(print_int, moveHorizontal, 0, -4);
	test_function(print_int, moveHorizontal, 1, 10);
	test_function(print_int, moveHorizontal, 1, -10);

	rc_check(lock(), "lock");	// lock at left border
	test_function(print_int, moveHorizontal, -1, -4);

	rc_check(nextPiece(), "nextPiece");
	test_function(print_int, moveHorizontal, 1, -3);

error:	// fallthrough
	destroy();
}

void test_moveDown()
{
	rc_check(init(), "init");
	rc_check(nextPiece(), "nextPiece");
	test_function(print_int, moveDown, 0, 0);
	test_function(print_int, moveDown, 0, -18);
	test_function(print_int, moveDown, 0, 2);
	test_function(print_int, moveDown, 1, -2);

	rc_check(lock(), "lock");
	test_function(print_int, moveDown, -1, -4);

	rc_check(nextPiece(), "nextPiece");
	test_function(print_int, moveDown, 1, -20);

error:	// fallthrough
	destroy();
}

void test_rotateLeft()
{
	rc_check(init(), "init");
	rc_check(nextPiece(), "nextPiece");
	rc_check(moveDown(-2), "moveDown");
	test_function(print_int, rotateLeft, 0);

	// manipulate active Piece to be an I at the left wall to simulate wall collision
	activePiece->color = RED;
	activePiece->orientation = EAST;
	activePiece->position.x = -2;
	activePiece->position.y = 12;
	test_function(print_int, rotateLeft, 0);

	rc_check(lock(), "lock");
	test_function(print_int, rotateLeft, -1);

	rc_check(nextPiece(), "nextPiece");
	rc_check(moveDown(-2), "moveDown");
	test_function(print_int, rotateLeft, 0);

error:	// fallthrough
	destroy();
}

void test_rotateRight()
{
	rc_check(init(), "init");
	rc_check(nextPiece(), "nextPiece");
	rc_check(moveDown(-2), "moveDown");
	test_function(print_int, rotateRight, 0);

	// manipulate active Piece to be an I at the left wall to simulate wall collision
	activePiece->color = RED;
	activePiece->orientation = EAST;
	activePiece->position.x = -2;
	activePiece->position.y = 12;
	test_function(print_int, rotateRight, 0);

	rc_check(lock(), "lock");
	test_function(print_int, rotateRight, -1);

	rc_check(nextPiece(), "nextPiece");
	rc_check(moveDown(-2), "moveDown");
	test_function(print_int, rotateRight, 0);

error:	// fallthrough
	destroy();
}

void test_markLines()
{
	rc_check(init(), "init");
	test_function(print_int, markLines, 0);

	// manipulate playfield to contain 1 marked line
	int line_start = 4 * FIELD_WIDTH;
	int i = 0;
	for(i = 0; i < FIELD_WIDTH; i++) {
		playfield[line_start + i] = BLUE;
	}
	test_function(print_int, markLines, 1);
	test_function(print_int, markLines, 0);

	// free playfield to force error
	free(playfield);
	playfield = NULL;
	test_function(print_int, markLines, -1);

error:	// fallthrough
	destroy();
}

void test_clearLines()
{
	rc_check(init(), "init");
	test_function(print_int, clearLines, 0);

	// manipulate playfield to contain 1 marked line
	int line_start = 4 * FIELD_WIDTH;
	int i = 0;
	for(i = 0; i < FIELD_WIDTH; i++) {
		playfield[line_start + i] = DESTROYED;
	}
	test_function(print_int, clearLines, 1);
	test_function(print_int, clearLines, 0);

	// free playfield to force error
	free(playfield);
	playfield = NULL;
	test_function(print_int, clearLines, -1);

error:	// fallthrough
	destroy();
}
