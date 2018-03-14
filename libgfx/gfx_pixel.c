#include "stdgfx.h"

int pixel_wrap = 0;

void gfx_pixel_wrap(int wrap)
{
  pixel_wrap = wrap;
}

void gfx_pixel(int x, int y, int r, int g, int b, int a)
{
  gfx_lock();
  if(x >= gfx_surface->w) {
    if(pixel_wrap) x = x - gfx_surface->w;
    else return;
  }

  if(y >= gfx_surface->h)
  {
    if(pixel_wrap) y = y - gfx_surface->h;
    else return;
  }

  if(x < 0)
  {
    if(pixel_wrap) x = x + gfx_surface->w;
    else return;
  }

  if(y < 0)
  {
    if(pixel_wrap) y = y + gfx_surface->h;
    else return;
  }
  uint32_t* pixel_ptr = gfx_framebuffer();
  pixel_ptr[y * gfx_surface->w + x] = gfx_color(r, g, b, a);
  gfx_unlock();
}

uint32_t* gfx_framebuffer(void)
{
  return __gfx_backend_framebuffer();
}
