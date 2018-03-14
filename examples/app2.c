#include <stdgfx.h>

int gfx_main(int argc, char** argv)
{
    int mytime = gfx_ticks();
	uint8_t r = 0xff;
	while(!gfx_iskey(GFX_W))
	{
		gfx_clear(r, 0, 0, 0);
        gfx_text_cursor(0,0);
		gfx_text_bg(0, 0, 0, 255);
		gfx_text_fg(0, 0, 255, 0);
		gfx_printf("The screen is flashing. Press 'w' to quit!\n");
		if(gfx_elapsed(mytime) > 1000)
		{
			mytime = gfx_ticks();
			if(r == 0xff)
				r = 0x00;
			else
				r = 0xff;
		}
		gfx_flush();
		gfx_input();
	}
}