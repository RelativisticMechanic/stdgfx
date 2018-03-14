#include "stdgfx.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

IMAGE* gfx_load_image(char* filename)
{
  IMAGE* i = malloc(sizeof(IMAGE));
  i->pixels = stbi_load(filename, &i->w, &i->h, &i->n, STBI_rgb_alpha);

  if(!i->pixels)
    return NULL;

  return i;
}

void gfx_draw_image(IMAGE* img, int x, int y, int w, int h)
{
  if((double)img->w/(double)w == 1.0f && (double)img->h/(double)h == 1.0f)
  {
    __gfx_backend_draw_pixels(img->pixels, x, y, w, h, img->w, img->h);
    return;
  }
  uint8_t* resized_image = calloc(w * h, sizeof(uint32_t));
  stbir_resize_uint8(img->pixels, img->w, img->h, 0, resized_image, w, h, 0, 4);
  __gfx_backend_draw_pixels(resized_image, x, y, w, h, w, h);
  free(resized_image);

}

void gfx_free_image(IMAGE* img)
{
  if(!img)
    return;

  stbi_image_free(img->pixels);
  free(img);
}
