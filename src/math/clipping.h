#ifndef CLIPPING_H
#define CLIPPING_H

#include "triangle.h"
#include "line.h"
#include "intersection.h"
#include <bitset>
#include <cassert>
#include <optional>

namespace fps::math {
   
   template<template<typename> typename Vec, typename NumericType, typename triangle_t = triangle<Vec, NumericType>>
      constexpr std::tuple<bool, std::optional<triangle_t>, std::optional<triangle_t>> clip(triangle<Vec, NumericType> const& tr, basic_plane<NumericType> const& pln) noexcept {
         std::bitset<3> back, front;

         std::size_t nOn{};
         for (int i = 0; i < 3; ++i) {
            auto const orient = pln.get_orientation(tr[i]);
            if (orient == Orientation::BEHIND) {
               back.set(i);
            }
            else 
            if (orient == Orientation::IN_FRONT) {
               front.set(i);
            }
            else {
               ++nOn;
            }
         }

         if (nOn == 3) {
            return { true, std::nullopt, std::nullopt };
         } else
         if (nOn == 2) {
            if (back.count() == 1)
               return { false, std::nullopt, std::nullopt };
            else
            if (front.count() == 1) {
               return { true, std::nullopt, std::nullopt };
            }
         }
         else
         if (nOn == 1) {
            if (back.count() == 2)
               return { false, std::nullopt, std::nullopt };
            else
            if (front.count() == 2) {
               return { true, std::nullopt, std::nullopt };
            }
         }

         if (back.count() == 3) {
            return { false, std::nullopt, std::nullopt };
         }
         if (front.count() == 3) {
            return { true, std::nullopt, std::nullopt };
         }
         

         auto const find_single = [](std::bitset<3> const& bs) -> std::size_t {
            for (std::size_t i = 0; i < 3; ++i) {
               if (bs.test(i)) {
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

         // One point in front of the plane
         if (front.count() == 1) {
            auto const nWhichFront = find_single(front);
            auto const points = find_intersect_points(nWhichFront);
            return { true, triangle{ tr[nWhichFront], points.first, points.second }, std::nullopt }; 
         }

         // Two points in front of the plane
         auto const nWhichBack = find_single(back); 
         auto const points = find_intersect_points(nWhichBack);
         return { true, triangle{ points.first, tr[(nWhichBack + 1) % 3], tr[(nWhichBack + 2) % 3] }, 
                  triangle{ points.first, tr[(nWhichBack + 2) % 3], points.second } }; 
      }
}

#endif
