#include "stdgfx.h"

uint32_t gfx_color(int r, int g, int b, int a)
{
	return __gfx_backend_color32(r, g, b, a);
}
