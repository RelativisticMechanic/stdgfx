#include "stdgfx.h"
#include <math.h>

extern int pixel_wrap;

// Fast horizontal line from (x1,y) to (x2,y), with rgb color
int __gfx_hor_line(int y, int x1, int x2, int r, int g, int b, int a)
{
  int w = gfx_surface->w;
  int h = gfx_surface->h;

  if(x2 < x1) {x1 += x2; x2 = x1 - x2; x1 -= x2;} //swap x1 and x2, x1 must be the leftmost endpoint
  if(x2 < 0 || x1 >= w || y < 0 || y >= h) return 0; //no single point of the line is on screen
  if(x1 < 0) x1 = 0; //clip
  if(x2 >= w) x2 = w - 1; //clip

  uint32_t color = gfx_color(r, g, b, a);
  uint32_t* framebuffer = gfx_framebuffer();
  gfx_lock();
  for(int x = x1; x <= x2; x++)
  {
    framebuffer[y * w + x] = color;
  }
  gfx_unlock();
  return 1;
}


// fast vertical line from (x,y1) to (x,y2), with rgb color
int __gfx_ver_line(int x, int y1, int y2, int r, int g, int b, int a)
{
  int w = gfx_surface->w;
  int h = gfx_surface->h;

  if(y2 < y1) {y1 += y2; y2 = y1 - y2; y1 -= y2;} //swap y1 and y2
  if(y2 < 0 || y1 >= h  || x < 0 || x >= w) return 0; //no single point of the line is on screen
  if(y1 < 0) y1 = 0; //clip
  if(y2 >= w) y2 = h - 1; //clip

  uint32_t color = gfx_color(r, g, b, a);
  uint32_t* framebuffer = gfx_framebuffer();
  gfx_lock();
  for(int y = y1; y <= y2; y++)
  {
    framebuffer[y * w + x] = color;
    y += gfx_surface->w;
  }
  gfx_unlock();
  return 1;
}


//Bresenham line from (x1,y1) to (x2,y2) with rgb color
int gfx_line(int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
  int w = gfx_surface->w;
  int h = gfx_surface->h;
  if(!pixel_wrap)
    if(x1 < 0 || x1 > w - 1 || x2 < 0 || x2 > w - 1 || y1 < 0 || y1 > h - 1 || y2 < 0 || y2 > h - 1) return 0;

  int deltax = fabs(x2 - x1); //The difference between the x's
  int deltay = fabs(y2 - y1); //The difference between the y's
  int x = x1; //Start x off at the first pixel
  int y = y1; //Start y off at the first pixel
  int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels, curpixel;

  if(x2 >= x1) //The x-values are increasing
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else //The x-values are decreasing
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  if(y2 >= y1) //The y-values are increasing
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else //The y-values are decreasing
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  if (deltax >= deltay) //There is at least one x-value for every y-value
  {
    xinc1 = 0; //Don't change the x when numerator >= denominator
    yinc2 = 0; //Don't change the y for every iteration
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax; //There are more x-values than y-values
  }
  else //There is at least one y-value for every x-value
  {
    xinc2 = 0; //Don't change the x for every iteration
    yinc1 = 0; //Don't change the y when numerator >= denominator
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay; //There are more y-values than x-values
  }
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    gfx_pixel(x % w, y % h, r, g, b, a);  //Draw the current pixel
    num += numadd;  //Increase the numerator by the top of the fraction
    if (num >= den) //Check if numerator >= denominator
    {
      num -= den; //Calculate the new numerator value
      x += xinc1; //Change the x as appropriate
      y += yinc1; //Change the y as appropriate
    }
    x += xinc2; //Change the x as appropriate
    y += yinc2; //Change the y as appropriate
  }

  return 1;
}


//Bresenham circle with center at (xc,yc) with radius and red green blue color
int gfx_circle(int xc, int yc, int radius, int r, int g, int b, int a)
{
  int w = gfx_surface->w;
  int h = gfx_surface->h;
  if(!pixel_wrap)
    if(xc - radius < 0 || xc + radius >= w || yc - radius < 0 || yc + radius >= h) return 0;

  int x = 0;
  int y = radius;
  int p = 3 - (radius << 1);
  int n1, n2, n3, n4, n5, n6, n7, n8;
  while (x <= y)
  {
     n1 = xc + x; //8 pixels can be calculated at once thanks to the symmetry
     n2 = yc + y;
     n3 = xc - x;
     n4 = yc - y;
     n5 = xc + y;
     n6 = yc + x;
     n7 = xc - y;
     n8 = yc - x;
     gfx_pixel(n1, n2, r, g, b, a);
     gfx_pixel(n3, n4, r, g, b, a);
     gfx_pixel(n5, n6, r, g, b, a);
     gfx_pixel(n7, n6, r, g, b, a);
     if(x > 0) //avoid drawing pixels at same position as the other ones
     {
       gfx_pixel(n1, n4, r, g, b, a);
       gfx_pixel(n3, n2, r, g, b, a);
       gfx_pixel(n5, n8, r, g, b, a);
       gfx_pixel(n7, n8, r, g, b, a);
     }
     if(p < 0) p += (x++ << 2) + 6;
     else p += ((x++ - y--) << 2) + 10;
  }

  return 1;
}

int gfx_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a)
{
  int l1 = gfx_line(x1, y1, x2, y2, r, g, b, a);
  int l2 = gfx_line(x2, y2, x3, y3, r, g, b, a);
  int l3 = gfx_line(x3, y3, x1, y1, r, g, b, a);
  
  if(!pixel_wrap)
    if(l1 != 0 && l2 != 0 && l3 != 0)
      return 1;
  
  return 0;
}
int gfx_disk(int xc, int yc, int radius, int r, int g, int b, int a)
{
  int w = gfx_surface->w;
  int h = gfx_surface->h;
  if(xc + radius < 0 || xc - radius >= w || yc + radius < 0 || yc - radius >= h) return 0; //every single pixel outside screen, so don't waste time on it
  int x = 0;
  int y = radius;
  int p = 3 - (radius << 1);
  int n1, n2, n3, n4, n5, n6, n7, n8;
  int pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times  (ensure initial value is outside the range)
  while (x <= y)
  {
     // write data
     n1 = xc + x;
     n2 = yc + y;
     n3 = xc - x;
     n4 = yc - y;
     n5 = xc + y;
     n6 = yc + x;
     n7 = xc - y;
     n8 = yc - x;
     if(n2 != pb) __gfx_hor_line(n2, n1, n3, r, g, b, a);
     if(n4 != pd) __gfx_hor_line(n4, n1, n3, r, g, b, a);
     if(n6 != b)  __gfx_hor_line(n6, n5, n7, r, g, b, a);
     if(n8 != n4 && n8 != n6) __gfx_hor_line(n8, n5, n7, r, g, b, a);
     pb = n2;
     pd = n4;
     if(p < 0) p += (x++ << 2) + 6;
     else p += ((x++ - y--) << 2) + 10;
  }

  return 1;
}

int gfx_quad(int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
  int w = gfx_surface->w;
  int h = gfx_surface->h;

  if(x1 < 0 || x1 > w - 1 || x2 < 0 || x2 > w - 1 || y1 < 0 || y1 > h - 1 || y2 < 0 || y2 > h - 1) return 0;
  SDL_Rect rec;
  rec.x = x1;
  rec.y = y1;
  rec.w = x2 - x1 + 1;
  rec.h = y2 - y1 + 1;
  uint32_t color = gfx_color(r, g, b, a);
  SDL_FillRect(gfx_surface, &rec, color);  //SDL's ability to draw a hardware rectangle is used for now
  return 1;
}
