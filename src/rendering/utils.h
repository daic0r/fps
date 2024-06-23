#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <raylib.h>
#include "renderbuffer.h"

namespace fps::rendering {
   void draw_pixel(renderbuffer& rb, int x, int y, Color const& col);
   void draw_line(renderbuffer& rb, int x1, int y1, int x2, int y2, Color const& col);
   void draw_triangle(renderbuffer& rb, int x1, int y1, int x2, int y2, int x3, int y3, Color col1, Color col2, Color col3, bool bFill = true);
}

#endif // GRAPHICS_UTILS_H
