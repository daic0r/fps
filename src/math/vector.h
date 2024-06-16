#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace fps::math {
   struct vector {
      float x, y, z, w;

      constexpr vector() : x{}, y{}, z{}, w{} {}

      constexpr vector(float x, float y, float z, float w = 0.0f) : x{x}, y{y}, z{z}, w{w} {}

      constexpr float length() const {
         return std::sqrt(x * x + y * y + z * z);
      }

      constexpr vector normalize() const {
         const auto len = length();
         return vector(x / len, y / len, z / len);
      }

      constexpr float dot(vector const& rhs) const noexcept {
         return x * rhs.x + y * rhs.y + z * rhs.z; 
      }

      constexpr vector cross(vector const& rhs) const noexcept {
         return vector{ y * rhs.z - z * rhs.y,
                        z * rhs.x - x * rhs.z,
                        x * rhs.y - y * rhs.x };
      }
   };
}

#endif
