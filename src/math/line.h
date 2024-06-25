#ifndef LINE_H
#define LINE_H

#include "triangle.h"
#include "plane.h"
#include <optional>

namespace fps::math {
   template<template<typename> typename Vec, typename NumericType>
   class line {
      using vec_t = Vec<NumericType>;

      vec_t p1_, p2_;
      
   public:
      constexpr line(vec_t const& p1, vec_t const& p2) noexcept
         : p1_{ p1 }, p2_{ p2 }
      {}

      constexpr auto const& start() const noexcept {
         return p1_;
      }

      constexpr auto const& end() const noexcept {
         return p2_;
      }

      constexpr std::optional<vec_t> intersects(fps::math::basic_plane<NumericType> const& pln) const noexcept {
         auto const t = (pln.distance() - (pln.normal().dot(start()))) / (pln.normal().dot((end() - start())));
         if (t < 0.0f or t > 1.0f)
            return std::nullopt;
         return start() + (end() - start()) * t;
      }
      constexpr std::optional<vec_t> intersects(triangle<Vec, NumericType> const& tri) const noexcept {
         auto const pln = tri.plane();
         auto const res = intersects(pln);
         if (res == std::nullopt)
            return std::nullopt;
         float u, v, w;
         tri.barycentric(*res, u, v, w);
         if (u >= 0.0f and u <= 1.0f and v >= 0.0f and v <= 1.0f and w >= 0.0f and w <= 1.0f and (u + v + w) == 1.0f)
            return res; 
         return std::nullopt; 
      }

   };
}

#endif
