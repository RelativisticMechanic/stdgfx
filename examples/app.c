#include <stdio.h>
#include <stdgfx.h>

int gfx_main(int argc, char** argv)
{
	gfx_resize(800, 600, 32);
	// Images
	IMAGE* i = gfx_load_image("./lenna.jpg");
	IMAGE* i2 = gfx_load_image("./abstract.jpg");
	gfx_draw_image(i2, 0, 0, 800, 600);
	gfx_draw_image(i, 320, 180, i->w/2, i->h/2);
	// Text output, set transparency = 255!!
	gfx_text_bg(0, 0, 0, 255);
	gfx_printf("Hello, World! 0x%x\n", 0x123456);
	gfx_printf("Hey, this is written in C!\n");
	// Rasterization routines
	gfx_disk(80, 50, 10, 0, 255, 255, 255);
	gfx_line(40, 60, 21, 30, 255, 0, 0, 255);
	gfx_triangle(50, 50, 32, 65, 90, 90, 255, 255, 255, 0);
	gfx_flush();
	gfx_saveshot("program.png");
	return 0;
}
