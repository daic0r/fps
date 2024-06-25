#ifndef MATH_UTILS_H
#define MATH_UTILS_H
   
#include <cstdlib>
#include <limits>
#include <type_traits>
#include <utility>
#include <optional>

namespace fps::math {
   template<typename T>
      requires std::is_arithmetic_v<T>
   constexpr bool test_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
      return std::abs(a - b) < epsilon;
   }

   template<typename T>
      requires std::is_arithmetic_v<T>
   constexpr T sgn(T x) noexcept {
      if (x < 0.0f)
         return -1.0f;
      if (x > 0.0f)
         return 1.0f;
      return 0.0f;
   }

   template<typename T>
      requires std::is_arithmetic_v<T>
   constexpr T abs(T x) noexcept {
      return x < T{0} ? -x : x;
   }

   constexpr auto make_bresenham_interpolator(int x0, int y0, int x1, int y1) {
      auto dx = abs(x1 - x0);
      auto dy = abs(y1 - y0);
      auto sx = x0 < x1 ? 1 : -1;
      auto sy = y0 < y1 ? 1 : -1;
      const auto bFastIsX = dx > dy;
      if (not bFastIsX) {
         std::swap(dx, dy);
         std::swap(sx, sy);
      }
      auto x = bFastIsX ? x0 : y0;
      auto y = bFastIsX ? y0 : x0;
      const auto d = 2*dy - dx;
      const auto dO = 2*dy;
      const auto dNO = 2*(dy - dx);
      auto error = d;
      return [=]() mutable -> std::optional<std::pair<int, int>> {
         if ((sx == 1 and x > (bFastIsX ? x1 : y1)) or
               (sx == -1 and x < (bFastIsX ? x1 : y1)))
            return std::nullopt;

         auto ret_y = y;
         if (error <= 0)
            error += dO;
         else {
            y += sy;
            error += dNO;
         }
         auto ret_x = x;
         x += sx;
         return std::make_pair(bFastIsX ? ret_x : ret_y, bFastIsX ? ret_y : ret_x);
      };
   }
}

#endif
