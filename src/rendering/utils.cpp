#include "utils.h"
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

namespace fps::rendering {

void draw_pixel(renderbuffer& rb, int x, int y, Color const& col) {
  auto const offset = (x + y * rb.width()) * 4;
  std::memcpy(rb.buffer() + offset, &col, sizeof(Color));
}

void draw_line(renderbuffer& rb, int x1, int y1, int x2, int y2, Color const& col) {
  using namespace fps::math;

  auto bresenham = make_bresenham_interpolator(x1, y1, x2, y2);
  auto ret = bresenham();
  while (ret) {
    draw_pixel(rb, ret.value().first, ret.value().second, col); 
    ret = bresenham();
  }
}

constexpr void barycentric2d(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y,
                   float& u, float& v, float& w) {
  using namespace fps::math;

  struct vec2d {
     int x, y;

     vec2d operator-(vec2d const& rhs) const {
        return vec2d{x - rhs.x, y - rhs.y};
     }

     int cross(vec2d const& rhs) const {
        return abs(x * rhs.y - y * rhs.x);
     }
  };
  float const total_area = (vec2d{ x2, y2 } - vec2d{ x1, y1 }).cross(vec2d{ x3, y3 } - vec2d{ x1, y1 });
  u = (vec2d{ x, y } - vec2d{ x3, y3 }).cross(vec2d{ x2, y2 } - vec2d{ x3, y3 }) / total_area;
  v = (vec2d{ x, y } - vec2d{ x1, y1 }).cross(vec2d{ x3, y3 } - vec2d{ x1, y1 }) / total_area;
  w = 1.0f - u - v; 
}

void draw_triangle(renderbuffer& rb, int x1, int y1, int x2, int y2, int x3,
                   int y3, Color col1, Color col2, Color col3, bool bFill /* = true*/) {
  // draw_line(rb, x1, y1, x2, y2, col);
  // draw_line(rb, x2, y2, x3, y3, col);
  // draw_line(rb, x1, y1, x3, y3, col);

  if (not bFill)
    return;

  using namespace math;
  auto corners =
      std::array<math::coord2d, 3>{coord2d{x1, y1},
                                   coord2d{x2, y2},
                                   coord2d{x3, y3}};
  std::ranges::sort(
      corners, [](coord2d const &a, coord2d const &b) { return a.y < b.y; });

  auto bresenham_second = make_bresenham_interpolator(corners[0].x, corners[0].y, corners[1].x, corners[1].y);
  auto bresenham_first = make_bresenham_interpolator(corners[0].x, corners[0].y, corners[2].x, corners[2].y);
  auto bresenham_third = make_bresenham_interpolator(corners[1].x, corners[1].y, corners[2].x, corners[2].y);
  auto line1 = bresenham_first();
  auto line2 = bresenham_second();
  int prev_y_1 = line1.value().second, prev_y_2 = line2.value().second;
  bool bSecondHalf{};
  bool b1{true},b2{true};
  while (b1) {
     auto line_min_x_1 = std::numeric_limits<int>::max();
     auto line_max_x_1 = std::numeric_limits<int>::min();
     auto line_min_x_2 = std::numeric_limits<int>::max();
     auto line_max_x_2 = std::numeric_limits<int>::min();

     auto advance_line = [&](int& min_x, int& max_x, auto& init, int& prev_y, auto& interpolator) {
        if (not init)
           init = interpolator();
        while (init and init.value().second == prev_y) {
           min_x = std::min(min_x, init.value().first);
           max_x = std::max(max_x, init.value().first);
        float u, v, w;
        barycentric2d(x1, y1, x2, y2, x3, y3, init.value().first, init.value().second, u, v, w);
        Color final_col;
        final_col.r = static_cast<unsigned char>(col1.r * u + col2.r * v + col3.r * w);
        final_col.g = static_cast<unsigned char>(col1.g * u + col2.g * v + col3.g * w);
        final_col.b = static_cast<unsigned char>(col1.b * u + col2.b * v + col3.b * w);
           draw_pixel(rb, init.value().first, init.value().second, final_col);
           prev_y = init.value().second;
           init = interpolator();
        }
        if (init)
          prev_y = init.value().second;

        return init.has_value();
     };
     b1 = advance_line(line_min_x_1, line_max_x_1, line1, prev_y_1, bresenham_first);
     b2 = advance_line(line_min_x_2, line_max_x_2, line2, prev_y_2, bSecondHalf ? bresenham_third : bresenham_second);
     int from_x, to_x;
     if (abs(line_max_x_1 - line_min_x_2) < abs(line_max_x_2 - line_min_x_1)) {
        from_x = line_max_x_1;
        to_x = line_min_x_2;
     } else {
        from_x = line_min_x_1;
        to_x = line_max_x_2;
     }
     if (from_x > to_x)
        std::swap(from_x, to_x);
     // std::cout << "line_min_x_1: " << line_min_x_1 << ", line_max_x_1: " << line_max_x_1 << "\n";
     // std::cout << "line_min_x_2: " << line_min_x_2 << ", line_max_x_2: " << line_max_x_2 << "\n";
     // std::cout << "from: " << from_x << ", to: " << to_x << "\n";
     for (int x = from_x; x <= to_x; ++x) {
        float u, v, w;
        barycentric2d(x1, y1, x2, y2, x3, y3, x, prev_y_1, u, v, w);
        Color final_col;
        final_col.r = static_cast<unsigned char>(col1.r * u + col2.r * v + col3.r * w);
        final_col.g = static_cast<unsigned char>(col1.g * u + col2.g * v + col3.g * w);
        final_col.b = static_cast<unsigned char>(col1.b * u + col2.b * v + col3.b * w);
        final_col.a = 255;
        draw_pixel(rb, x, prev_y_1, final_col);
     }
     if (not b2)
        bSecondHalf = true;
  }
}
} // namespace fps::rendering
