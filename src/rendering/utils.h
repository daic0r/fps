#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <raylib.h>
#include "renderbuffer.h"

namespace fps::rendering {
   void draw_pixel(renderbuffer& rb, int x, int y, Color col);
   void draw_line(renderbuffer& rb, float x1, float y1, float x2, float y2, Color col);
   void draw_triangle(renderbuffer& rb, float x1, float y1, float x2, float y2, float x3, float y3, Color col, bool bFill = true);
}

#endif // GRAPHICS_UTILS_H
