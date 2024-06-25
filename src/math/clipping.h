#ifndef CLIPPING_H
#define CLIPPING_H

#include "triangle.h"
#include "line.h"
#include "intersection.h"

namespace fps::math {
   
   template<template<typename> typename Vec, typename NumericType, typename triangle_t = triangle<Vec, NumericType>>
      __attribute__((always_inline))
      std::pair<std::optional<triangle_t>, std::optional<triangle_t>> clip(triangle<Vec, NumericType> const& tr, basic_plane<NumericType> const& pln) noexcept {
         std::array<bool, 3> arInFront{}, arBack{};
         int nCountFront{}, nCountBack{};

         for (int i = 0; i < 3; ++i) {
            auto const orient = pln.get_orientation(tr[i]);
            if (orient == Orientation::BEHIND) {
               arBack[i] = true;
               ++nCountBack;
            }
            else {
               arInFront[i] = true;
               ++nCountFront;
            }
         }

         if (nCountFront == 3 or nCountBack == 3)
            return { std::nullopt, std::nullopt };

         auto const find_single = [](std::array<bool, 3> const& arr) -> std::size_t {
            for (int i = 0; i < 3; ++i) {
               if (arr[i]) {
                  return i;
               }
            }
            return std::numeric_limits<std::size_t>::max();
         };

         auto const find_intersect_points = [&tr, &pln](std::size_t nWhichSingle) {
            auto const l1 = line{ tr[(nWhichSingle + 1) % 3], tr[nWhichSingle] };
            auto const l2 = line{ tr[(nWhichSingle + 2) % 3], tr[nWhichSingle] };
            auto const p1 = intersects(l1, pln);
            auto const p2 = intersects(l2, pln);
            return std::make_pair(*p1, *p2);
         };


         if (nCountFront == 1) {
            auto const nWhichFront = find_single(arInFront);
            auto const points = find_intersect_points(nWhichFront);
            auto ret = triangle{ tr[0], points.first, points.second };
            return { ret, std::nullopt }; 
         }

         auto const nWhichBack = find_single(arBack); 
         auto const points = find_intersect_points(nWhichBack);
         auto ret1 = triangle{ points.first, tr[(nWhichBack + 1) % 3], tr[(nWhichBack + 2) % 3] };
         auto ret2 = triangle{ points.first, tr[(nWhichBack + 2) % 3], points.second };
         return { ret1, ret2 }; 
      }
}

#endif
