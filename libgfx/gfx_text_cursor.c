#include "stdgfx.h"

// This expects terminal co-ordinates and not screen co-ordinates!
void gfx_text_cursor(int x, int y)
{
    if(x >= gfx_terminal_max_x || y >= gfx_terminal_max_y) return;

    gfx_terminal_x = x;
    gfx_terminal_y = y;
}