#ifndef PLANE_H
#define PLANE_H

#include "vector.h"
#include "utils.h"

namespace fps::math {
   enum class Orientation {
      IN_FRONT,
      ON,
      BEHIND
   };

   template<typename NumericType>
   class basic_plane {
      using vec_t = vec3<NumericType>;

      vec_t normal_;
      NumericType d_;

   public:

      constexpr auto const& normal() const noexcept {
         return normal_;
      }

      constexpr auto distance() const noexcept {
         return d_;
      }

      constexpr basic_plane(vec_t const& normal, vec_t const& point) noexcept
         : normal_{ normal.normalize() }, 
         d_{ normal.dot(point) }
      {
      }

      constexpr bool is_point_on_plane(vec_t const& p) const noexcept {
         return test_equal(normal_.dot(p), d_, .00001f);
      }

      constexpr Orientation get_orientation(vec_t const& p) const noexcept {
         const auto fOrient = normal_.dot(p) - d_;
         switch (static_cast<int>(sgn(fOrient))) {
            case -1:
               return Orientation::BEHIND;
            case 1:
               return Orientation::IN_FRONT;
         }
         return Orientation::ON;
      }
   };

   using plane = basic_plane<float>;
}

#endif
