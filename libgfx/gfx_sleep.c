#include "stdgfx.h"

void gfx_sleep(int s)
{
	__gfx_backend_sleep(s);
}
