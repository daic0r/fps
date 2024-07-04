#include "renderbuffer.h"
#include "../math/vector.h"
#include "../math/triangle.h"

namespace fps::rendering {

   void renderbuffer::clear() {
      //std::memset(buffer_.get(), 0, width_ * height_ * 4);
      std::memset(sdl_texture_, 0, width_ * height_ * 4);
      for (int i = 0; i < width_ * height_; ++i)
         zbuffer_[i] = std::numeric_limits<float>::max();
   }
   void renderbuffer::draw_pixel(int x, int y, Color const &col) {
      auto const offset = (x + y * width()) * 4;
      //std::memcpy(buffer() + offset, &col, sizeof(Color));
      auto swapped = std::byteswap(std::bit_cast<int>(col));
      std::memcpy(sdl_texture_ + offset, &swapped, sizeof(int));
   }
   void renderbuffer::draw_pixel(int x, int y, float z, Color const &col) {
      if (z < zbuffer_[x + y * width()]) {
         zbuffer_[x + y * width()] = z;
         draw_pixel(x, y, col);
      }
   }
void renderbuffer::draw_triangle(int x1, int y1, float z1, int x2, int y2, float z2, int x3,
         int y3, float z3, Color col1, Color col2, Color col3,
         bool bFill /* = true*/) {

      using namespace math;
      auto corners = std::array<math::coord2d, 3>{coord2d{x1, y1}, coord2d{x2, y2},
         coord2d{x3, y3}};
      std::ranges::sort(
            corners, [](coord2d const &a, coord2d const &b) { return a.y < b.y; });

      auto bresenham_second = make_bresenham_interpolator(
            corners[0].x, corners[0].y, corners[1].x, corners[1].y);
      auto bresenham_first = make_bresenham_interpolator(
            corners[0].x, corners[0].y, corners[2].x, corners[2].y);
      auto bresenham_third = make_bresenham_interpolator(
            corners[1].x, corners[1].y, corners[2].x, corners[2].y);
      auto line1 = bresenham_first();
      auto line2 = bresenham_second();
      int prev_y_1 = line1.value().second, prev_y_2 = line2.value().second;
      bool bSecondHalf{};
      bool b1{true}, b2{true};
      triangle<vec2, int> t{vec2i{x1, y1}, vec2i{x2, y2}, vec2i{x3, y3}};
      // float const total_area = (vec2i{ x2, y2 } - vec2i{ x1, y1
      // }).cross_magnitude(vec2i{ x3, y3 } - vec2i{ x1, y1 });
      while (b1) {
         auto line_min_x_1 = std::numeric_limits<int>::max();
         auto line_max_x_1 = std::numeric_limits<int>::min();
         auto line_min_x_2 = std::numeric_limits<int>::max();
         auto line_max_x_2 = std::numeric_limits<int>::min();

         auto advance_line = [&](int &min_x, int &max_x, auto &init, int &prev_y,
               auto &interpolator) {
            if (not init)
               init = interpolator();
            while (init and init.value().second == prev_y) {
               min_x = std::min(min_x, init.value().first);
               max_x = std::max(max_x, init.value().first);
               float u, v, w;
               t.barycentric(vec2i{init.value().first, init.value().second}, u, v, w);
               // barycentric2d(x1, y1, x2, y2, x3, y3, init.value().first,
               // init.value().second, u, v, w, total_area);
               Color final_col;
               if (col1 == col2 and col2 == col3) {
                  final_col = col1;
               } else {
                  final_col.r =
                     static_cast<unsigned char>(col1.r * u + col2.r * v + col3.r * w);
                  final_col.g =
                     static_cast<unsigned char>(col1.g * u + col2.g * v + col3.g * w);
                  final_col.b =
                     static_cast<unsigned char>(col1.b * u + col2.b * v + col3.b * w);
                  final_col.a =
                     static_cast<unsigned char>(col1.a * u + col2.a * v + col3.a * w);
               }
               auto const fZ = z1 * u + z2 * v + z3 * w;
               draw_pixel(init.value().first, init.value().second, fZ, final_col);
               prev_y = init.value().second;
               init = interpolator();
            }
            if (init)
               prev_y = init.value().second;

            return init.has_value();
         };
         b1 = advance_line(line_min_x_1, line_max_x_1, line1, prev_y_1,
               bresenham_first);
         b2 = advance_line(line_min_x_2, line_max_x_2, line2, prev_y_2,
               bSecondHalf ? bresenham_third : bresenham_second);
         if (not b2) {
            bSecondHalf = true;
            if (not b1) {
               // We have a trinagle like this if we get here (flat bottom):
               //
               //   /
               //  /|
               // / |
               // ---
               //
               // There is no second half, so do one pass for the bottom line here
               // because after this iteration the loop will exit
               line_min_x_2 = std::numeric_limits<int>::max();
               line_max_x_2 = std::numeric_limits<int>::min();
               advance_line(line_min_x_2, line_max_x_2, line2, prev_y_2,
                     bresenham_third);
            }
         }

         if (not bFill)
            continue;

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
         // std::cout << "line_min_x_1: " << line_min_x_1 << ", line_max_x_1: " <<
         // line_max_x_1 << "\n"; std::cout << "line_min_x_2: " << line_min_x_2 << ",
         // line_max_x_2: " << line_max_x_2 << "\n"; std::cout << "from: " << from_x
         // << ", to: " << to_x << "\n";
         for (int x = from_x; x <= to_x; ++x) {
            float u, v, w;
            // barycentric2d(x1, y1, x2, y2, x3, y3, x, prev_y_1, u, v, w,
            // total_area);
            t.barycentric(vec2i{x, prev_y_1}, u, v, w);
            Color final_col;
            if (col1 == col2 and col2 == col3) {
               final_col = col1;
            } else {
               final_col.r =
                  static_cast<unsigned char>(col1.r * u + col2.r * v + col3.r * w);
               final_col.g =
                  static_cast<unsigned char>(col1.g * u + col2.g * v + col3.g * w);
               final_col.b =
                  static_cast<unsigned char>(col1.b * u + col2.b * v + col3.b * w);
               final_col.a =
                  static_cast<unsigned char>(col1.a * u + col2.a * v + col3.a * w);
            }
            auto const fZ = z1 * u + z2 * v + z3 * w;
            draw_pixel(x, prev_y_1, fZ, final_col);
         }
      }
   }}
