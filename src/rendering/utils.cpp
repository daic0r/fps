#include "utils.h"
#include "../math/triangle.h"
#include "../math/utils.h"
#include "../math/vector.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstring>
#include <exception>
#include <iostream>
#include <limits>

bool operator==(Color const &lhs, Color const &rhs) {
   return lhs.r == rhs.r and lhs.g == rhs.g and lhs.b == rhs.b and lhs.a == rhs.a;
}

namespace fps::rendering {

   void draw_line(renderbuffer &rb, int x1, int y1, int x2, int y2,
         Color const &col) {
      using namespace fps::math;

      auto bresenham = make_bresenham_interpolator(x1, y1, x2, y2);
      auto ret = bresenham();
      while (ret) {
         rb.draw_pixel(ret.value().first, ret.value().second, col);
         ret = bresenham();
      }
   }

   constexpr void barycentric2d(int x1, int y1, int x2, int y2, int x3, int y3,
         int x, int y, float &u, float &v, float &w,
         float total_area) {
      using namespace fps::math;

      struct vec2d {
         int x, y;

         vec2d operator-(vec2d const &rhs) const {
            return vec2d{x - rhs.x, y - rhs.y};
         }

         int cross(vec2d const &rhs) const { return abs(x * rhs.y - y * rhs.x); }
      };
      u = (vec2d{x, y} - vec2d{x3, y3}).cross(vec2d{x2, y2} - vec2d{x3, y3}) /
         total_area;
      v = (vec2d{x, y} - vec2d{x1, y1}).cross(vec2d{x3, y3} - vec2d{x1, y1}) /
         total_area;
      w = 1.0f - u - v;
   }
   
} // namespace fps::rendering
