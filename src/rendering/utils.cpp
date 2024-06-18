#include "utils.h"
#include "../math/utils.h"
#include "../math/vector.h"
#include <algorithm>
#include <array>
#include <cmath>

namespace fps::rendering {

void draw_pixel(renderbuffer& rb, int x, int y, Color col) {
  if (x < 0 || x >= rb.width() || y < 0 || y >= rb.height())
    return;

  auto const offset = (x + y * rb.width()) * 4;
  rb.buffer()[offset] = col.r;
  rb.buffer()[offset + 1] = col.g;
  rb.buffer()[offset + 2] = col.b;
  rb.buffer()[offset + 3] = col.a;
}

void draw_line(renderbuffer& rb, float x1, float y1, float x2, float y2, Color col) {
  using namespace fps::math;

  float inc_x = sgn(x2 - x1);
  float dy = (y2 - y1) / std::fabs(x2 - x1);
  for (float x = std::round(x1), y = std::round(y1);
       not test_equal(x, std::round(x2)); x += inc_x) {
    draw_pixel(rb, (int)x, (int)y, col);
    y += dy;
  }
}

void draw_triangle(renderbuffer& rb, float x1, float y1, float x2, float y2, float x3,
                   float y3, Color col, bool bFill /* = true*/) {
  // draw_line(rb, x1, y1, x2, y2, col);
  // draw_line(rb, x2, y2, x3, y3, col);
  // draw_line(rb, x1, y1, x3, y3, col);

  if (not bFill)
    return;

  using namespace math;
  auto corners =
      std::array<math::coord2d, 3>{coord2d{std::round(x1), std::round(y1)},
                                   coord2d{std::round(x2), std::round(y2)},
                                   coord2d{std::round(x3), std::round(y3)}};
  std::ranges::sort(
      corners, [](coord2d const &a, coord2d const &b) { return a.y < b.y; });

  // auto dy1 = (corners[1].y - corners[0].y) / std::fabs(corners[1].x -
  // corners[0].x); auto dy2 = (corners[2].y - corners[0].y) /
  // std::fabs(corners[2].x - corners[0].x); for (float y = corners[0].y; y <
  // corners[1].y; y+=1.0f) {
  //    auto base1 = (y - corners[0].y) / dy1;
  //    auto base2 = (y - corners[0].y) / dy2;
  //    if (corners[1].x < corners[0].x)
  //       base1 *= -1.0f;
  //    if (corners[2].x < corners[0].x)
  //       base2 *= -1.0f;
  //    auto x1 = base1 + corners[0].x;
  //    auto x2 = base2 + corners[0].x;
  //    if (x2 < x1)
  //       std::swap(x1, x2);
  //    for (auto x = x1+1; x < x2; x += 1.0f) {
  //       DrawPixel((int) x, (int) y, GREEN);
  //    }
  // }
  //
  // dy1 = (corners[2].y - corners[1].y) / std::fabs(corners[2].x -
  // corners[1].x); dy2 = (corners[2].y - corners[0].y) / std::fabs(corners[2].x
  // - corners[0].x); for (float y = corners[1].y; y < corners[2].y; y+=1.0f) {
  //    auto base1 = (y - corners[1].y) / dy1;
  //    auto base2 = (y - corners[0].y) / dy2;
  //    if (corners[2].x < corners[1].x)
  //       base1 *= -1.0f;
  //    if (corners[2].x < corners[0].x)
  //       base2 *= -1.0f;
  //    auto x1 = base1 + corners[1].x;
  //    auto x2 = base2 + corners[0].x;
  //    if (x2 < x1)
  //       std::swap(x1, x2);
  //    for (auto x = x1+1; x < x2; x += 1.0f) {
  //       DrawPixel((int) x, (int) y, GREEN);
  //    }
  // }

  auto const draw_half = [col, &rb](coord2d const &from1, coord2d const &from2,
                                    coord2d const &to1, coord2d const &to2) {
    auto dy1 = (to1.y - from1.y) / std::fabs(to1.x - from1.x);
    auto dy2 = (to2.y - from2.y) / std::fabs(to2.x - from2.x);
    for (float y = std::max(from1.y, from2.y); y < std::min(to1.y, to2.y);
         y += 1.0f) {
      auto base1 = (y - from1.y) / dy1;
      auto base2 = (y - from2.y) / dy2;
      if (to1.x < from1.x)
        base1 *= -1.0f;
      if (to2.x < from2.x)
        base2 *= -1.0f;
      auto x1 = base1 + from1.x;
      auto x2 = base2 + from2.x;
      if (x2 < x1)
        std::swap(x1, x2);
      for (auto x = x1 + 1; x < x2; x += 1.0f) {
        draw_pixel(rb, (int)x, (int)y, col);
      }
    }
  };
  draw_half(corners[0], corners[0], corners[1], corners[2]);
  draw_half(corners[1], corners[0], corners[2], corners[2]);
}
} // namespace fps::rendering
