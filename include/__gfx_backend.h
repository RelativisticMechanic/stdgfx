#ifndef __GFX_BACKEND_H
#define __GFX_BACKEND_H

/* Audio Compilation */
#define GFX_AUDIO

#include <SDL.h>

#ifdef GFX_AUDIO
#include <pa_interface.h>
#endif

enum {
	GFX_W = 0,
	GFX_A,
	GFX_S,
	GFX_D,
	GFX_SPACE,
	GFX_R,
	GFX_Q,
	GFX_E,
	GFX_CTRL,
	GFX_ESC
};

extern SDL_Surface* gfx_surface;
extern uint8_t* inkeys;
extern SDL_Event ev;

int         __gfx_backend_init(void);
void        __gfx_backend_screen(int w, int h, int bpp);
uint32_t*   __gfx_backend_framebuffer(void);
void        __gfx_backend_set_caption(const char* caption);
void        __gfx_backend_clear(int r, int g, int b, int a);
uint32_t    __gfx_backend_color32(int r, int g, int b, int a);
void        __gfx_backend_flush(void);
void        __gfx_backend_input(void);
int         __gfx_backend_iskey(int key);
void        __gfx_backend_lock(void);
void        __gfx_backend_unlock(void);
void        __gfx_backend_draw_pixels(uint8_t* pixels, int x, int y, int w, int h, int image_width, int image_height);
int         __gfx_backend_ticks(void);
void        __gfx_backend_wait_for_exit(void);
void        __gfx_backend_sleep(int s);
int			__gfx_backend_get_mouse_x(void);
int			__gfx_backend_get_mouse_y(void);
void 		__gfx_backend_resize(int w, int h, int bpp);
uint32_t* 	__gfx_backend_direct_framebuffer(void);
int 		__gfx_backend_play_sound(int channels, int sample_rate, int len, uint16_t* data);
void 		__gfx_backend_stop_sound(int id);
int			__gfx_backend_isplaying_sound(int id);

#endif
