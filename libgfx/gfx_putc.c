#include "stdgfx.h"
/* A tiny terminal emulator.. Hehe */

uint32_t gfx_print_fg = 0xFFFFFFFF;
uint32_t gfx_print_bg = 0;

int gfx_terminal_max_x = 0;
int gfx_terminal_max_y = 0;
int gfx_terminal_x = 0;
int gfx_terminal_y = 0;

void __gfx_gen_pixels(int c, unsigned int w, unsigned char* pixels)
{
	unsigned int x, y;
	uint8_t* fnt = gfx_font();

	if(c != 0 && w != 0)
  {
    for(y = 0; y < gfx_font_height; y++)
    {
      for(x = 0; x < gfx_font_width; x++)
      {
        if(fnt[c * gfx_font_height + y] >> (7 - x) & 1)
        {
          pixels[x + (y * w)] = 1;
        }
      }
    }
  }
}

int __gfx_putc_at(int c, int x, int y)
{
  uint8_t* pixbuf = calloc(gfx_bytes_per_char, 1);
  __gfx_gen_pixels(c, gfx_font_width, pixbuf);

  uint32_t* framebuffer = gfx_framebuffer();
  gfx_lock();
  for(int iy = 0; iy < gfx_font_height; iy++)
  {
    for(int ix = 0; ix < gfx_font_width; ix++)
    {
      if(pixbuf[iy * gfx_font_width + ix])
        framebuffer[((y + iy) * gfx_surface->w) + (x + ix)] = gfx_print_fg;
      else
			{
				// Platform specific code!!
				// Expects alpha value as top 8-bits!!
				// TODO: Fix!
				double alpha_frac = ((double)((gfx_print_bg & 0xFF000000) >> 24)) / 255.0f;
				uint32_t background = framebuffer[((y + iy) * gfx_surface->w) + (x + ix)];
				// displayColor = source * alphaFrac + bgcolor * (1 - alphaFrac)
				framebuffer[((y + iy) * gfx_surface->w) + (x + ix)] = (uint32_t)((double)gfx_print_bg * alpha_frac + (double)background * (1 - alpha_frac));
			}
    }
  }
  gfx_unlock();
  free(pixbuf);
  return c;
}

void gfx_text_fg(int r, int g, int b, int a)
{
  gfx_print_fg = gfx_color(r, g, b, a);
}

void gfx_text_bg(int r, int g, int b, int a)
{
  gfx_print_bg = gfx_color(r, g, b, a);
}

void __gfx_text_scroll(void)
{
  uint32_t* framebuffer = gfx_framebuffer();
  for(int i = 0; i < gfx_width * (gfx_terminal_max_y - 1) * gfx_font_height; i++)
  {
    framebuffer[i] = framebuffer[i + gfx_width * gfx_font_height];
  }
  for(int i = (gfx_width * (gfx_terminal_max_y - 1) * gfx_font_height); i < gfx_width * gfx_height; i++)
  {
    framebuffer[i] = 0;
  }
}
int gfx_putc(int c)
{
  if(gfx_terminal_x >= gfx_terminal_max_x)
  {
    gfx_terminal_x = 0;
    gfx_terminal_y += 1;
  }
  if(gfx_terminal_y >= gfx_terminal_max_y)
  {
    gfx_terminal_y = gfx_terminal_max_y - 1;
    gfx_terminal_x = 0;
    __gfx_text_scroll();
  }

  switch(c)
  {
    case '\n':
      gfx_terminal_x = 0;
      if(gfx_terminal_y < gfx_terminal_max_y)
        gfx_terminal_y += 1;
      return c;
    case '\b':
      if(gfx_terminal_x > 0)
        gfx_terminal_x--;
      return c;
    default:
      break;
  }

  __gfx_putc_at(c, gfx_terminal_x * gfx_font_width, gfx_terminal_y * gfx_font_height);
  gfx_terminal_x += 1;
}

int gfx_puts(char* str)
{
  int i = 0;
  while(str[i] != '\0')
  {
    gfx_putc(str[i]);
    i++;
  }
  return (i-1);
}
