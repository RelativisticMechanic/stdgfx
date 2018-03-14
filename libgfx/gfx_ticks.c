#include "stdgfx.h"

int gfx_ticks(void)
{
	return __gfx_backend_ticks();
}

int gfx_elapsed(int ticks)
{
	return gfx_ticks() - ticks;
}