#include <cstdlib>
#include <limits>

namespace fps::math {
   bool test_equal(float a, float b, float epsilon = std::numeric_limits<float>::epsilon()) {
      return std::abs(a - b) < epsilon;
   }
}
