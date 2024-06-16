#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace fps::math {
   struct vertex {
      float x{}, y{}, z{}, w{ 1.0f };

      constexpr vertex operator*(float scalar) const noexcept {
         return vertex{ x * scalar, y * scalar, z * scalar };
      }
      constexpr vertex operator/(float scalar) const noexcept {
         return vertex{ x / scalar, y / scalar, z / scalar };
      }
   };

   struct vector : vertex {
      friend class matrix;

      constexpr vector() = default;

      constexpr vector(float x, float y, float z, float w = 0.0f) : vertex{ x, y, z, w } {}

      constexpr float length() const noexcept {
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

      constexpr vector operator+(vector const& rhs) const noexcept {
         return vector{ x + rhs.x, y + rhs.y, z + rhs.z };
      }
      constexpr vector operator-(vector const& rhs) const noexcept {
         return vector{ x - rhs.x, y - rhs.y, z - rhs.z };
      }
   };
}

#endif
