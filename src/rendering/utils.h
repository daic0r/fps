#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <raylib.h>

namespace fps::rendering {
   void draw_line(float x1, float y1, float x2, float y2, Color col);
   void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col);
}

#endif // GRAPHICS_UTILS_H
