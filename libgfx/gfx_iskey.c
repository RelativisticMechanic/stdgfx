#include "stdgfx.h"

int gfx_iskey(int key)
{
	return __gfx_backend_iskey(key);
}
