#include "stdgfx.h"

void gfx_ok(int test, const char* fmt, ...)
{
	if(!test) return;
	
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(EXIT_FAILURE);
}
