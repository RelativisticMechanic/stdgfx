#include "stdgfx.h"

void gfx_flush(void)
{
	__gfx_backend_flush();
}
