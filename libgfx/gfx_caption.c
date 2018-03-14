#include "stdgfx.h"

void gfx_caption(const char* caption)
{
	__gfx_backend_set_caption(caption);
}
