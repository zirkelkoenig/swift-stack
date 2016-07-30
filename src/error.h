#ifndef _error_h
#define _error_h

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define cond_check(A, M, ...) if(!(A)) {\
	fprintf(stderr, "(%s:%d, %s) " M "\n", __FILE__, __LINE__, strerror(errno), ##__VA_ARGS__);\
	errno = 0;\
	goto error;}

#define alloc_check(A) cond_check((A), "memory allocation failed")

#define global_check(A, N) cond_check((A), "global \"" N "\" uninitialized")

#define rc_check(A, N) cond_check((A) >= 0, "function \"" N "\" returned an error")

#define sdl_check(A) if(!(A)) {\
	fprintf(stderr, "(%s:%d) %s\n", __FILE__, __LINE__, SDL_GetError());\
	goto sdlerr;}

#endif
