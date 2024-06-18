#include "utils.h"
#include "../math/utils.h"
#include <algorithm>
#include <array>
#include <cmath>
#include "../math/vector.h"

namespace fps::rendering {
   void draw_line(float x1, float y1, float x2, float y2, Color col) {
      using namespace fps::math;

      float inc_x = sgn(x2 - x1);
      float dy = (y2 - y1) / std::fabs(x2 - x1);
      for (float x = std::round(x1), y = std::round(y1); not test_equal(x, std::round(x2)); x += inc_x) {
         DrawPixel((int) x, (int) y, col);
         y += dy;
      } 
   }

   void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, Color col) {
      draw_line(x1, y1, x2, y2, col); 
      draw_line(x2, y2, x3, y3, col); 
      draw_line(x1, y1, x3, y3, col); 

      using namespace math;
      auto corners = std::array<math::coord2d, 3>{ coord2d{ std::round(x1), std::round(y1) }, coord2d{ std::round(x2), std::round(y2) }, coord2d{ std::round(x3), std::round(y3) } };
      std::ranges::sort(corners, [](coord2d const& a, coord2d const& b) { return a.y < b.y; });
      auto dy1 = (corners[1].y - corners[0].y) / std::fabs(corners[1].x - corners[0].x);
      auto dy2 = (corners[2].y - corners[0].y) / std::fabs(corners[2].x - corners[0].x);
      for (float y = corners[0].y; y < corners[1].y; y+=1.0f) {
         auto base1 = (y - corners[0].y) / dy1;
         auto base2 = (y - corners[0].y) / dy2;
         if (corners[1].x < corners[0].x)
            base1 *= -1.0f;
         if (corners[2].x < corners[0].x)
            base2 *= -1.0f;
         auto x1 = base1 + corners[0].x;
         auto x2 = base2 + corners[0].x;
         if (x2 < x1)
            std::swap(x1, x2);
         for (auto x = x1+1; x < x2; x += 1.0f) {
            DrawPixel((int) x, (int) y, GREEN);
         }
      }

      dy1 = (corners[2].y - corners[1].y) / std::fabs(corners[2].x - corners[1].x);
      for (float y = corners[1].y; y < corners[2].y; y+=1.0f) {
         auto base1 = (y - corners[1].y) / dy1;
         auto base2 = (y - corners[0].y) / dy2;
         if (corners[2].x < corners[1].x)
            base1 *= -1.0f;
         if (corners[2].x < corners[0].x)
            base2 *= -1.0f;
         auto x1 = base1 + corners[1].x;
         auto x2 = base2 + corners[0].x;
         if (x2 < x1)
            std::swap(x1, x2);
         for (auto x = x1+1; x < x2; x += 1.0f) {
            DrawPixel((int) x, (int) y, GREEN);
         }
      }
   }
}
