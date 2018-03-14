#include "stdgfx.h"

void gfx_screen(int w, int h, int bpp)
{
	__gfx_backend_screen(w, h, bpp);
}
