#include "stdgfx.h"

void gfx_clear(int r, int g, int b, int a)
{
	__gfx_backend_clear(r, g, b, a);
}
