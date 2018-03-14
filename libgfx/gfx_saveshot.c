#include "stdgfx.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void gfx_saveshot(char* filename)
{
    stbi_write_bmp(filename, gfx_width, gfx_height, 4, __gfx_backend_direct_framebuffer());
}