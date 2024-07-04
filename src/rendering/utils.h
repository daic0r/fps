#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <raylib.h>
#include "renderbuffer.h"

namespace fps::rendering {
   void draw_line(renderbuffer& rb, int x1, int y1, int x2, int y2, Color const& col);
}

#endif // GRAPHICS_UTILS_H
