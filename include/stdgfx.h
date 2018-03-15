#ifndef STD_GFX_H
#define STD_GFX_H

#define GFX_VERSION 1.0

#define GFX_DEFAULT_WIDTH 640
#define GFX_DEFAULT_HEIGHT 480
#define GFX_DEFAULT_BPP 32

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

int main(int argc, char** argv);
int gfx_main(int argc, char** argv);

typedef struct {
		int w;
		int h;
		int n;
		uint8_t* pixels;
} IMAGE;

typedef struct {
	int channels;
	int length;
	int sample_rate;
	int16_t* data;
} SOUND;


#include "__gfx_backend.h"

int 			gfx_init(void);
void 			gfx_ok(int test, const char* fmt, ...);
void			gfx_resize(int w, int h, int bpp);
uint32_t 		gfx_color(int r, int g, int b, int a);
void 			gfx_screen(int w, int h, int bpp);
void 			gfx_input(void);
int 			gfx_iskey(int key);
void 			gfx_sleep(int s);
int 			gfx_ticks(void);
void 			gfx_update(void);
void 			gfx_lock(void);
void 			gfx_unlock(void);
void 			gfx_caption(const char* caption);
void 			gfx_flush(void);
void 			gfx_clear(int r, int g, int b, int a);
void 			gfx_pixel(int x, int y, int r, int g, int b, int a);
void 			gfx_pixel_wrap(int wrap);
uint8_t*		gfx_font(void);
int 			gfx_putc(int c);
int 			gfx_puts(char* str);
int 			gfx_printf(const char* fmt, ...);
uint32_t*		gfx_framebuffer(void);
void 			gfx_text_fg(int r, int g, int b, int a);
void 			gfx_text_bg(int r, int g, int b, int a);
void			gfx_text_cursor(int x, int y);

int 			gfx_line(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
int 			gfx_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
int 			gfx_circle(int xc, int yc, int radius, int r, int g, int b, int a);
int 			gfx_disk(int xc, int yc, int radius, int r, int g, int b, int a);
int 			gfx_quad(int x1, int y1, int x2, int y2, int r, int g, int b, int a);

IMAGE* 			gfx_load_image(char* filename);
void			gfx_free_image(IMAGE* img);
void			gfx_draw_image(IMAGE* img, int x, int y, int w, int h);

int 			gfx_mouse_x(void);
int 			gfx_mouse_y(void);

int				gfx_elapsed(int ticks);

void 			gfx_saveshot(char* filename);

SOUND* 			gfx_load_sound(char* filename);
int 			gfx_play_sound(SOUND* snd);
void 			gfx_stop_sound(int id);
int 			gfx_isplaying_sound(int id);
void 			gfx_free_sound(SOUND* snd);

extern const int gfx_font_height, gfx_font_width, gfx_bytes_per_char;
extern int gfx_height, gfx_width, gfx_bpp;
extern int gfx_terminal_max_x, gfx_terminal_max_y, gfx_terminal_x, gfx_terminal_y;
#endif
