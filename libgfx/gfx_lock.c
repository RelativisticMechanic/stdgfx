#include "stdgfx.h"

void gfx_lock(void)
{
	__gfx_backend_lock();
}

void gfx_unlock(void)
{
	__gfx_backend_unlock();
}
