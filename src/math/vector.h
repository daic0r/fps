#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <execution>

namespace fps::math {

   template<typename Derived, std::size_t N, typename NumericType>
   struct vec {
      std::array<NumericType, N> data_{};

      // !!! These just slow things down considerably !!!
      //
      // constexpr vec() = default;
      // constexpr vec(vec const&) = default;
      // template<std::size_t M>
      // __attribute__((always_inline))
      // constexpr vec(vec<Derived, M, NumericType> const& other) {
      //    std::copy_n(std::execution::par_unseq, other.data_.begin(), std::min(N, M), data_.begin());
      // }
      // constexpr vec& operator=(vec const&) = default;
      // template<std::size_t M>
      // __attribute__((always_inline))
      // constexpr vec& operator=(vec<Derived, M, NumericType> const& other) {
      //    std::copy_n(std::execution::par_unseq,other.data_.begin(), std::min(N, M), data_.begin());
      //    return *this;
      // }
      // constexpr vec(vec&&) = default;
      // constexpr vec& operator=(vec&&) = default;
      // __attribute__((always_inline))
      // constexpr vec(std::initializer_list<NumericType> list) {
      //    std::copy(std::execution::par_unseq,list.begin(), list.end(), data_.begin());
      // }

      __attribute__((always_inline))
      constexpr NumericType& operator[](std::size_t index) noexcept {
         return data_[index];
      }
      __attribute__((always_inline))
      constexpr NumericType const& operator[](std::size_t index) const noexcept {
         return data_[index];
      }
      __attribute__((always_inline))
      constexpr auto operator+(vec const& rhs) const noexcept {
         Derived result{};
         //std::transform(std::execution::par_unseq, data_.begin(), data_.end(), rhs.data_.begin(), result.data_.begin(), std::plus{});
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data_[i] + rhs[i];
         }
         return result;
      }
      __attribute__((always_inline))
      constexpr auto operator-(vec const& rhs) const noexcept {
         Derived result{};
         //std::transform(std::execution::par_unseq, data_.begin(), data_.end(), rhs.data_.begin(), result.data_.begin(), std::minus{});
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data_[i] - rhs[i];
         }
         return result;
      }
      __attribute__((always_inline))
      constexpr auto operator*(NumericType scalar) const noexcept {
         Derived result{};
         // std::transform(std::execution::par_unseq, data_.begin(), data_.end(), result.data_.begin(), [scalar](auto const& elem) {
         //    return elem * scalar;
         // });
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data_[i] * scalar;
         }
         return result;
      }
      __attribute__((always_inline))
      constexpr auto operator/(NumericType scalar) const noexcept {
         Derived result{};
         // std::transform(std::execution::par_unseq, data_.begin(), data_.end(), result.data_.begin(), [scalar](auto const& elem) {
         //    return elem / scalar;
         // });
         const auto fFactor = 1.0f / scalar;
         for (std::size_t i = 0; i < N; ++i) {
            result[i] = data_[i] * fFactor;
         }
         return result;
      }
      __attribute__((always_inline))
      constexpr NumericType dot(vec const& rhs) const noexcept {
         NumericType result{};
         for (std::size_t i = 0; i < N; ++i) {
            result += data_[i] * rhs[i];
         }
         return result;
         // return std::inner_product(
         //       data_.begin(), data_.end(), rhs.data_.begin(), NumericType{});
      }
      __attribute__((always_inline))
      constexpr NumericType length() const noexcept {
         return std::sqrt(dot(*this));
      }
      __attribute__((always_inline))
      constexpr vec normalize() const noexcept {
         return *this / length();
      }
      __attribute__((always_inline))
      constexpr bool operator==(vec const& rhs) const noexcept {
         return std::all_of(std::execution::par_unseq, data_.begin(), data_.end(), [&rhs, i = 0](auto const& elem) mutable {
            return test_equal(elem, rhs[i++]);
         });
      }
   };

   template<typename NumericType>
   struct vec2 : vec<vec2<NumericType>, 2, NumericType> {
      constexpr vec2() = default;
      __attribute__((always_inline))
      constexpr vec2(NumericType x, NumericType y) 
         : vec<vec2<NumericType>, 2, NumericType>{ x, y } {}
      __attribute__((always_inline))
      constexpr NumericType cross_magnitude(vec2 const& rhs) const noexcept {
         using base = vec<vec2<NumericType>, 2, NumericType>;
         return abs(base::data_[0] * rhs[1] - base::data_[1] * rhs[0]);
      } 
   };

   template<typename NumericType>
   struct vec3 : vec<vec3<NumericType>, 3, NumericType> {
      constexpr vec3() = default;
      __attribute__((always_inline))
      constexpr vec3(NumericType x, NumericType y, NumericType z) 
         : vec<vec3<NumericType>, 3, NumericType>{ x, y, z } {}
      __attribute__((always_inline))
      constexpr vec3 cross(vec3 const& rhs) const noexcept {
         using base = vec<vec3<NumericType>, 3, NumericType>;
         return vec3{ base::data_[1] * rhs[2] - base::data_[2] * rhs[1],
                      base::data_[2] * rhs[0] - base::data_[0] * rhs[2],
                      base::data_[0] * rhs[1] - base::data_[1] * rhs[0] };
      }
   };

   template<typename NumericType>
   struct vec4 : vec<vec4<NumericType>, 4, NumericType> {
      constexpr vec4() = default;
      __attribute__((always_inline))
      constexpr vec4(NumericType x, NumericType y, NumericType z, NumericType w) 
         : vec<vec4<NumericType>, 4, NumericType>{ x, y, z, w } {}
   };


   using vec2f = vec2<float>;
   using vec3f = vec3<float>;
   using vec4f = vec4<float>;

   using vec2i = vec2<int>;
   using vec3i = vec3<int>;
   using vec4i = vec4<int>;

   struct vertex : vec4f {
      constexpr vertex() = default;
      constexpr vertex(float x, float y, float z, float w = 1.0f) : vec4f{ x, y, z, w } {}
   };

   struct coord2d {
      int x, y;
   };
}

#endif
