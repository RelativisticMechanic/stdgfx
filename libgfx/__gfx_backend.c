#include "__gfx_backend.h"

SDL_Surface* gfx_surface;
SDL_Event ev;
uint8_t* inkeys;
int mouse_coords[2] = { 0 };

int __gfx_backend_init(void)
{
  	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
  	{
		fprintf(stderr, "sdl failed to init: %s\n", SDL_GetError());
		return -1;
	}

	#ifdef GFX_AUDIO
	audio_interface_init();
	#endif
	atexit(SDL_Quit);
	inkeys = SDL_GetKeyState(NULL);
	SDL_PumpEvents();
 	return 0;
}

void __gfx_backend_screen(int w, int h, int bpp)
{
	gfx_surface = SDL_SetVideoMode(w, h, bpp, SDL_SWSURFACE | SDL_NOFRAME);
	if(gfx_surface == NULL)
	fprintf(stderr, "error setting video mode: %s\n", SDL_GetError());
}

void __gfx_backend_resize(int w, int h, int bpp)
{
	__gfx_backend_screen(w, h, bpp);
}

void __gfx_backend_set_caption(const char* caption)
{
 	SDL_WM_SetCaption(caption, caption);
}

void __gfx_backend_clear(int r, int g, int b, int a)
{
 	SDL_FillRect(gfx_surface, NULL, __gfx_backend_color32(r, g, b, a));
}

uint32_t __gfx_backend_color32(int r, int g, int b, int a)
{
 	return SDL_MapRGBA(gfx_surface->format, r, g, b, a);
}

void __gfx_backend_flush(void)
{
 	SDL_UpdateRect(gfx_surface, 0, 0, 0, 0);
}

void __gfx_backend_input(void)
{
	SDL_PumpEvents();
	
	SDL_GetMouseState(&mouse_coords[0], &mouse_coords[1]);
}

int __gfx_backend_iskey(int key)
{
  switch(key)
	{
		case GFX_W: return (inkeys[SDLK_w] != 0);
		case GFX_A: return (inkeys[SDLK_a] != 0);
		case GFX_S: return (inkeys[SDLK_s] != 0);
		case GFX_D: return (inkeys[SDLK_d] != 0);
		case GFX_SPACE: return (inkeys[SDLK_SPACE] != 0);
		case GFX_Q: return (inkeys[SDLK_q] != 0);
		case GFX_E: return (inkeys[SDLK_e] != 0);
		case GFX_R: return (inkeys[SDLK_r] != 0);
		case GFX_CTRL: return (inkeys[SDLK_LCTRL] != 0);
		case GFX_ESC: return (inkeys[SDLK_ESCAPE] != 0);
		default: return 0;
	}
}

void __gfx_backend_lock(void)
{
  if(SDL_MUSTLOCK(gfx_surface))
	if(SDL_MUSTLOCK(gfx_surface) < 0)
		return;
}

void __gfx_backend_unlock(void)
{
  if(SDL_MUSTLOCK(gfx_surface))
	SDL_UnlockSurface(gfx_surface);
}

uint32_t* __gfx_backend_framebuffer(void)
{
  return gfx_surface->pixels;
}

uint32_t* __gfx_backend_direct_framebuffer(void)
{
	return SDL_GetVideoSurface()->pixels;
}

void __gfx_backend_draw_pixels(uint8_t* pixels, int x, int y, int w, int h, int image_width, int image_height)
{
  /* SDL specific code */
  int depth = 32;
  int pitch = 4 * image_width;
  SDL_Surface* image_surface = SDL_CreateRGBSurfaceFrom((void*)pixels, w, h, depth, pitch, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
  SDL_Rect dst_rect;

  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = w;
  dst_rect.h = h;

  SDL_BlitSurface(image_surface, NULL, gfx_surface, &dst_rect);
  SDL_FreeSurface(image_surface);
}

int __gfx_backend_ticks(void)
{
  return (int)SDL_GetTicks();
}

void __gfx_backend_sleep(int s) {
  SDL_Delay(s);
}

void __gfx_backend_wait_for_exit(void)
{
	// wait for input
	int waiting = 1;
	while(waiting)
	{
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					waiting = 0;
					break;
				case SDL_KEYDOWN:
					waiting = 0;
					break;
			}
		}
	}
}
int __gfx_backend_get_mouse_x(void)
{
	return mouse_coords[0];
}

int __gfx_backend_get_mouse_y(void)
{
	return mouse_coords[1];
}

int __gfx_backend_play_sound(int channels, int sample_rate, int len, uint16_t* data)
{
	#ifdef GFX_AUDIO
	return audio_interface_play_sound(channels, sample_rate, len, data);
	#endif
}
void __gfx_backend_stop_sound(int id)
{
	#ifdef GFX_AUDIO
	return audio_interface_stop_sound(id);
	#endif
}
int	__gfx_backend_isplaying_sound(int id)
{
	#ifdef GFX_AUDIO
	return audio_interface_isplaying(id);
	#else
		return 0;
	#endif
}
