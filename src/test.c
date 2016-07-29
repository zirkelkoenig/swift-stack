#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "coord_map.h"
#include "error.h"
#include "math_data.h"

#define test_function(P, F, R, ...) {\
	char* exp = (P)((R));\
	char* act = (P)((F)(__VA_ARGS__));\
	fprintf(stdout, "Running function \"%s\"...\n", #F);\
	fprintf(stdout, "\tExpected result: %s\n", exp);\
	fprintf(stdout, "\tActual result:   %s\n\n", act);\
	if(exp) free(exp);\
	if(act) free(act);}

char* make_string(size_t size, const char* format, ...);

// printing functions
char* print_ICoord(ICoord data);
char* print_int(int value);

// testing functions
void test_ICoord_shift();
void test_CoordMap_init();

int main(int argc, char *argv[])
{
	test_ICoord_shift();
	test_CoordMap_init();

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

char* print_ICoord(ICoord data)
{
	return make_string(2 * (3 * sizeof(int)), "{%d, %d}", data.x, data.y);
}

char* print_int(int value)
{
	return make_string(3 * sizeof(int), "%d", value);
}
