#include "utils.h"
#include "../math/utils.h"
#include "../math/vector.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>

namespace fps::rendering {

void draw_pixel(renderbuffer& rb, int x, int y, Color const& col) {
  auto const offset = (x + y * rb.width()) * 4;
  std::memcpy(rb.buffer() + offset, &col, sizeof(Color));
}

void draw_line(renderbuffer& rb, int x1, int y1, int x2, int y2, Color const& col) {
  using namespace fps::math;

  int* pGoverning = abs(x2 - x1) >= abs(y2 - y1) ? &x1 : &y1;
  int inc = pGoverning == &x1 ? sgn(x2 - x1) : sgn(y2 - y1);
  int secondary = pGoverning == &x1 ? y1 : x1;
  int delta = pGoverning == &x1 ? (x2 - x1) / abs(y2 - y1) : (y2 - y1) / abs(x2 - x1);
  for (int tmp = *pGoverning, limit = pGoverning == &x1 ? x2 : y2; tmp != limit;
       tmp += inc) {
    draw_pixel(rb, pGoverning == &x1 ? tmp : secondary,
               pGoverning == &x1 ? secondary : tmp, col);
    secondary += delta;
  }
}

void draw_triangle(renderbuffer& rb, int x1, int y1, int x2, int y2, int x3,
                   int y3, Color col, bool bFill /* = true*/) {
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

  auto const draw_half = [](renderbuffer& rb, coord2d const &from1, coord2d const &from2,
                                    coord2d const &to1, coord2d const &to2, Color const &col) {
    auto dy1 = ((float)(to1.y - from1.y)) / abs(to1.x - from1.x);
    auto dy2 = ((float)(to2.y - from2.y)) / abs(to2.x - from2.x);
    for (int y = std::max(from1.y, from2.y); y < std::min(to1.y, to2.y);
         ++y) {
      auto base1 = (int) std::round((y - from1.y) / dy1);
      auto base2 = (int) std::round((y - from2.y) / dy2);
      if (to1.x < from1.x)
        base1 *= -1;
      if (to2.x < from2.x)
        base2 *= -1;
      auto x1 = base1 + from1.x;
      auto x2 = base2 + from2.x;
      if (x2 < x1)
        std::swap(x1, x2);
      for (auto x = x1 + 1; x < x2; ++x) {
         draw_pixel(rb, x, y, col);
      }
    }
  };
  draw_half(rb, corners[0], corners[0], corners[1], corners[2], col);
  draw_half(rb, corners[1], corners[0], corners[2], corners[2], col);
}
} // namespace fps::rendering
