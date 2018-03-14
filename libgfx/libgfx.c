#include "stdgfx.h"

int gfx_height, gfx_width, gfx_bpp;

int main(int argc, char** argv)
{
	// Initialize SDL and screen output
	gfx_init();
	gfx_screen(GFX_DEFAULT_WIDTH, GFX_DEFAULT_HEIGHT, GFX_DEFAULT_BPP);

	gfx_height = GFX_DEFAULT_HEIGHT;
	gfx_width = GFX_DEFAULT_WIDTH;
	gfx_bpp = GFX_DEFAULT_BPP;

	gfx_terminal_max_x = gfx_width / gfx_font_width;
	gfx_terminal_max_y = gfx_height / gfx_font_height;

	gfx_caption(argv[0]);
	gfx_clear(0, 0, 0, 255);

	gfx_main(argc, argv);
	gfx_flush();
	__gfx_backend_wait_for_exit();
	return 0;
}
