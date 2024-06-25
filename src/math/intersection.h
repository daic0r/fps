#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "line.h"
#include "plane.h"

namespace fps::math {

   template<template<typename> typename Vec, typename NumericType, typename vec_t = Vec<NumericType>>
      constexpr std::optional<vec_t> intersects(line<Vec, NumericType> ln, fps::math::basic_plane<NumericType> const& pln) noexcept {
         auto const t = (pln.distance() - (pln.normal().dot(ln.start()))) / (pln.normal().dot((ln.end() - ln.start())));
         if (t < 0.0f or t > 1.0f)
            return std::nullopt;
         return ln.start() + (ln.end() - ln.start()) * t;
      }

   template<template<typename> typename Vec, typename NumericType, typename vec_t = Vec<NumericType>>
      constexpr std::optional<vec_t> intersects(line<Vec, NumericType> ln, triangle<Vec, NumericType> const& tri) noexcept {
         auto const pln = tri.plane();
         auto const res = intersects(ln, pln);
         if (res == std::nullopt)
            return std::nullopt;
         float u, v, w;
         tri.barycentric(*res, u, v, w);
         if (u >= 0.0f and u <= 1.0f and v >= 0.0f and v <= 1.0f and w >= 0.0f and w <= 1.0f and (u + v + w) == 1.0f)
            return res; 
         return std::nullopt; 
      }
}

#endif
