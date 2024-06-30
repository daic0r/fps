#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "plane.h"
#include "matrix.h"

namespace fps::math {

   template<template<typename> typename Vec, typename FloatingPointType>
   class triangle {
      using vec_t = Vec<FloatingPointType>;

      std::array<vec_t, 3> vertices_;
      FloatingPointType area_;
      vec_t side1_, side2_;

      constexpr auto get_cross_magnitude(vec_t const& lhs, vec_t const& rhs) {
         if constexpr (std::is_same_v<vec_t, vec2<FloatingPointType>>) {
            return lhs.cross_magnitude(rhs);
         } else {
            return lhs.cross(rhs).length();
         }
      };


   public:
      constexpr bool operator==(triangle const& rhs) const noexcept {
         for (std::size_t i = 0; i < 3; ++i) {
            if (vertices_[i] != rhs.vertices_[i]) {
               return false;
            }
         }
         return true;
      }

      __attribute__((always_inline))
      constexpr triangle(vec_t v0, vec_t v1, vec_t v2) :
         vertices_{v0, v1, v2}, 
         area_{ get_cross_magnitude(vertices_[1] - vertices_[0], vertices_[2] - vertices_[0]) },
         side1_{ vertices_[1] - vertices_[2] },
         side2_{ vertices_[2] - vertices_[0] }
      {}

      __attribute__((always_inline))
      constexpr void barycentric(vec_t const& p,
                         float& u, float& v, float& w) const noexcept {
        using namespace fps::math;

        auto const v1 = p - vertices_[2];
        auto const v2 = p - vertices_[0];
        u = abs(v1[0] * side1_[1] - v1[1] * side1_[0]) / static_cast<float>(area_);
        v = abs(v2[0] * side2_[1] - v2[1] * side2_[0]) / static_cast<float>(area_);
        w = 1.0f - u - v; 
      }

      __attribute__((always_inline))
      auto normal() const noexcept {
         return (vertices_[1] - vertices_[0]).cross(vertices_[2] - vertices_[0]).normalize();
      }
      
      __attribute__((always_inline))
      auto plane() const noexcept {
         return fps::math::basic_plane<FloatingPointType>{ normal(), vertices_[0] };
      }

      __attribute__((always_inline))
      constexpr auto const& operator[](std::size_t nIdx) const noexcept {
         return vertices_[nIdx];
      }

      __attribute__((always_inline))
      constexpr auto& operator[](std::size_t nIdx)  noexcept {
         return vertices_[nIdx];
      }

      __attribute__((always_inline))
      constexpr void transform(matrix const& m) noexcept {
         for (auto& v : vertices_) {
            v = m * v;
         }
      }
   };
}

#endif
