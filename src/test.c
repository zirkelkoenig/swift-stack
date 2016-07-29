#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord_map.h"
#include "error.h"
#include "logic.h"
#include "math_data.h"

/* Macro function which performs the test and prints out their return values if they differ from expectations.
	P - name of the return type's printing function
	F - name of the function to test
	R - expected test result
	... - arguments that are passed to the tested function
*/
#define test_function(P, F, R, ...)\
	char* exp = (P)((R));\
	char* act = (P)((F)(__VA_ARGS__));\
	if((!exp && act) || (exp && !act) || (exp && act && strcmp(exp, act))) {\
		fprintf(stdout, "Failed test for function \"%s\"!\n", #F);\
		fprintf(stdout, "\tExpected result: %s\n", exp);\
		fprintf(stdout, "\tActual result:   %s\n\n", act);\
	}\
	if(exp) free(exp);\
	if(act) free(act);

/* Combines different values into a string according to a format. The size of the returned string has to be large
enough. Returns NULL on error. */
char* make_string(size_t size, const char* format, ...);

// printing functions
char* print_ICoord(ICoord data);
char* print_ICoord_p(ICoord* data);
char* print_int(int value);

// testing functions
void test_ICoord_shift();
void test_CoordMap_init();
void test_CoordMap_get();

int main(int argc, char *argv[])
{
	test_ICoord_shift();
	test_CoordMap_init();
	test_CoordMap_get();

	return 0;
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
	CoordMap_init();
	int i = 0;
	int j = 0;
	int k = 0;

	struct TestData {
		Square arg_color;
		Orientation arg_orientation;
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

	for(i = 0; i < numTests; i++) {
		test_function(print_ICoord_p, CoordMap_get,
				tests[i].result,
				tests[i].arg_color,
				tests[i].arg_orientation,
				tests[i].arg_num);
	}

	// invalid via loops
	Square invColors[6] = {EMPTY, DESTROYED, 9, 12354, -1, -54778};
	Orientation invOrientations[5] = {NONE, 6, 432170, -1, -93478};
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

error:	// fallthrough
	CoordMap_destroy();
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
