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
   };
}

#endif
