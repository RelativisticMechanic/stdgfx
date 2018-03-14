#include "stdgfx.h"

void gfx_update(void)
{
	gfx_flush();
	while(SDL_PollEvent(&ev))
	{
		if(ev.type == SDL_QUIT)
		{
			gfx_ok(0, "quit called.\n");
			exit(EXIT_FAILURE);
		}
	}
	gfx_input();
}
