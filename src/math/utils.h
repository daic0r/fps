#include <cstdlib>
#include <limits>
#include <type_traits>

namespace fps::math {
   template<typename T>
      requires std::is_floating_point_v<T>
   constexpr bool test_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
      return std::abs(a - b) < epsilon;
   }

   template<typename T>
      requires std::is_floating_point_v<T>
   constexpr T sgn(T x) noexcept {
      if (x < 0.0f)
         return -1.0f;
      if (x > 0.0f)
         return 1.0f;
      return 0.0f;
   }
}
