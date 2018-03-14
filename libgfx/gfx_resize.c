#include "stdgfx.h"

void gfx_resize(int w, int h, int bpp)
{
    gfx_width = w;
    gfx_height = h;
    gfx_bpp = bpp;
    gfx_terminal_max_x = gfx_width / gfx_font_width;
	gfx_terminal_max_y = gfx_height / gfx_font_height;
    
    __gfx_backend_resize(w, h, bpp);
}