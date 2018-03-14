#include "stdgfx.h"

int gfx_printf(const char* fmt, ...)
{
  va_list arg;
  va_start(arg, fmt);
  int n = snprintf(NULL, 0, fmt, arg);
  char* s = malloc(n+1);
  vsprintf(s, fmt, arg);
  gfx_puts(s);
  free(s);
  return n;
}
