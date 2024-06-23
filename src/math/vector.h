#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace fps::math {

   template<std::size_t N, typename FloatingPointType>
   struct vec {
      std::array<FloatingPointType, N> data{};

      constexpr vec() = default;
      constexpr vec(vec const&) = default;
      template<std::size_t M>
      constexpr vec(vec<M, FloatingPointType> const& other) {
         std::copy_n(other.data.begin(), std::min(N, M), data.begin());
      }
      constexpr vec& operator=(vec const&) = default;
      template<std::size_t M>
      constexpr vec& operator=(vec<M, FloatingPointType> const& other) {
         std::copy_n(other.data.begin(), std::min(N, M), data.begin());
         return *this;
      }
      constexpr vec(vec&&) = default;
      constexpr vec& operator=(vec&&) = default;
      constexpr vec(std::initializer_list<FloatingPointType> list) {
         std::copy(list.begin(), list.end(), data.begin());
      }

      constexpr FloatingPointType& operator[](std::size_t index) noexcept {
         return data[index];
      }
      constexpr FloatingPointType const& operator[](std::size_t index) const noexcept {
         return data[index];
      }
      constexpr vec operator+(vec const& rhs) const noexcept {
         vec result{};
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] + rhs[i];
         }
         return result;
      }
      constexpr vec operator-(vec const& rhs) const noexcept {
         vec result{};
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] - rhs[i];
         }
         return result;
      }
      constexpr vec operator*(FloatingPointType scalar) const noexcept {
         vec result{};
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] * scalar;
         }
         return result;
      }
      constexpr vec operator/(FloatingPointType scalar) const noexcept {
         vec result{};
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] / scalar;
         }
         return result;
      }
      constexpr FloatingPointType dot(vec const& rhs) const noexcept {
         FloatingPointType result{};
         for (std::size_t i = 0; i < N; ++i) {
            result += data[i] * rhs[i];
         }
         return result;
      }
      constexpr FloatingPointType length() const noexcept {
         return std::sqrt(dot(*this));
      }
      constexpr vec normalize() const noexcept {
         return *this / length();
      }
   };

   struct vec2 : vec<2, float> {
      constexpr vec2() = default;
      constexpr vec2(float x, float y) : vec<2, float>{ x, y } {}
   };

   struct vec3 : vec<3, float> {
      constexpr vec3() = default;
      constexpr vec3(float x, float y, float z) : vec<3, float>{ x, y, z } {}
      constexpr vec3 cross(vec3 const& rhs) const noexcept {
         return vec3{ data[1] * rhs[2] - data[2] * rhs[1],
                      data[2] * rhs[0] - data[0] * rhs[2],
                      data[0] * rhs[1] - data[1] * rhs[0] };
      }
   };

   using vec4 = vec<4, float>;

   struct vertex : vec4 {
      constexpr vertex() = default;
      constexpr vertex(float x, float y, float z, float w = 1.0f) : vec4{ x, y, z, w } {}
   };

   struct coord2d {
      int x, y;
   };
}

#endif
