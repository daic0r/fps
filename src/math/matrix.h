#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include "vector.h"
#include "utils.h"

namespace fps::math {

   namespace detail {
      template<typename NumericType>
      struct mat3x3 {
         std::array<std::array<NumericType, 3>, 3> data_;
         
         constexpr NumericType determinant() const noexcept {
            return data_[0][0] * (data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1]) +
               data_[0][1] * (data_[1][2] * data_[2][0] - data_[1][0] * data_[2][2]) +
               data_[0][2] * (data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0]);
         }

         constexpr mat3x3 invert() const noexcept {
            const auto d = determinant();
            if (test_equal(d, 0.0f, .00001f)) {
               return mat3x3{};
            }
            const auto invdet = NumericType{1} / d;
            return mat3x3{
               std::array<NumericType, 3>{
                  (data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1]) * invdet,
                  (data_[0][2] * data_[2][1] - data_[0][1] * data_[2][2]) * invdet,
                  (data_[0][1] * data_[1][2] - data_[0][2] * data_[1][1]) * invdet
               },
               std::array<NumericType, 3>{
                  (data_[1][2] * data_[2][0] - data_[1][0] * data_[2][2]) * invdet,
                  (data_[0][0] * data_[2][2] - data_[0][2] * data_[2][0]) * invdet,
                  (data_[0][2] * data_[1][0] - data_[0][0] * data_[1][2]) * invdet
               },
               std::array<NumericType, 3>{
                  (data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0]) * invdet,
                  (data_[0][1] * data_[2][0] - data_[0][0] * data_[2][1]) * invdet,
                  (data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0]) * invdet
               }
            };
         }
      };
   }

   template<typename NumericType>
   class basic_matrix {
      static constexpr inline auto Zero = NumericType{ 0 };
      static constexpr inline auto One = NumericType{ 1 };
      static constexpr inline auto Two = NumericType{ 2 };
      static constexpr inline auto Half = NumericType{ 0.5 };

      std::array<std::array<NumericType, 4>, 4> m_data{};

   public:
      constexpr basic_matrix() = default;
      constexpr basic_matrix(NumericType m00, NumericType m01, NumericType m02, NumericType m03,
            NumericType m10, NumericType m11, NumericType m12, NumericType m13,
            NumericType m20, NumericType m21, NumericType m22, NumericType m23,
            NumericType m30, NumericType m31, NumericType m32, NumericType m33) :
         m_data{
            std::array<NumericType, 4>{m00, m01, m02, m03},
            std::array<NumericType, 4>{m10, m11, m12, m13},
            std::array<NumericType, 4>{m20, m21, m22, m23},
            std::array<NumericType, 4>{m30, m31, m32, m33}
         } {}

      constexpr decltype(auto) operator[](std::size_t row, std::size_t col) {
         return (m_data[row][col]);
      }

      constexpr decltype(auto) operator[](std::size_t row, std::size_t col) const {
         return (m_data[row][col]);
      }


      constexpr vec4<NumericType> operator*(const vec4<NumericType>& v) const {
         return vec4<NumericType>{
            m_data[0][0] * v[0] + m_data[0][1] * v[1] + m_data[0][2] * v[2] + m_data[0][3] * v[3],
            m_data[1][0] * v[0] + m_data[1][1] * v[1] + m_data[1][2] * v[2] + m_data[1][3] * v[3],
            m_data[2][0] * v[0] + m_data[2][1] * v[1] + m_data[2][2] * v[2] + m_data[2][3] * v[3],
            m_data[3][0] * v[0] + m_data[3][1] * v[1] + m_data[3][2] * v[2] + m_data[3][3] * v[3]
         };
      }

      constexpr basic_matrix operator*(const basic_matrix& m) const {
         basic_matrix result{};
         for (std::size_t i = 0; i < 4; ++i) {
            for (std::size_t j = 0; j < 4; ++j) {
               result[i,j] = m_data[i][0] * m[0,j] + m_data[i][1] * m[1,j] + m_data[i][2] * m[2,j] + m_data[i][3] * m[3,j];
            }
         }
         return result;
      }

      constexpr basic_matrix transpose() const noexcept {
         basic_matrix result{};
         for (std::size_t i = 0; i < 4; ++i) {
            for (std::size_t j = 0; j < 4; ++j) {
               result[i,j] = result[j,i];
            }
         }
         return result;
      }

      constexpr basic_matrix invert() const noexcept {
         // https://math.stackexchange.com/questions/152462/inverse-of-transformation-matrix
         const auto inv_top_left3x3 = detail::mat3x3<NumericType>{
            std::array<NumericType, 3>{m_data[0][0], m_data[0][1], m_data[0][2]},
            std::array<NumericType, 3>{m_data[1][0], m_data[1][1], m_data[1][2]},
            std::array<NumericType, 3>{m_data[2][0], m_data[2][1], m_data[2][2]}
         }.invert();
         const auto transl = vec3<NumericType>{m_data[0][3], m_data[1][3], m_data[2][3]};
         const auto inv_transl = inv_top_left3x3 * transl;
         return basic_matrix{
            inv_top_left3x3.data_[0][0], inv_top_left3x3.data_[0][1], inv_top_left3x3.data_[0][2], -inv_transl[0],
            inv_top_left3x3.data_[1][0], inv_top_left3x3.data_[1][1], inv_top_left3x3.data_[1][2], -inv_transl[1],
            inv_top_left3x3.data_[2][0], inv_top_left3x3.data_[2][1], inv_top_left3x3.data_[2][2], -inv_transl[2],
            Zero, Zero, Zero, One
         };
      }

      static constexpr basic_matrix perspective(NumericType fovV, NumericType fAspectRatio, NumericType fNear, NumericType fFar) {
         const auto fov2 = fovV * Half;
         const auto denom = fNear - fFar;
         basic_matrix ret{};
         ret[0,0] = One / (tanf(fov2) * fAspectRatio);
         ret[1,1] = One / tanf(fov2);
         ret[2,2] = (fNear + fFar) / denom;
         ret[2,3] = (Two * fNear * fFar) / denom;
         ret[3,2] = -One;
         return ret;
      }
      static constexpr basic_matrix viewport(int x, int y, int width, int height) {
         basic_matrix ret{};
         const auto w2 = Half * width;
         const auto h2 = Half * height;
         ret[0,0] = w2 - Half;
         ret[0,3] = w2 - Half + x;
         ret[1,1] = -h2 + Half;
         ret[1,3] = h2 - Half + y;
         ret[2,2] = One;
         return ret;
      }
      static constexpr basic_matrix identity() noexcept {
         return basic_matrix{ One, Zero, Zero, Zero,
                        Zero, One, Zero, Zero,
                        Zero, Zero, One, Zero,
                        Zero, Zero, Zero, One };
      }
      static constexpr basic_matrix rotate_x(NumericType angle) noexcept {
         const auto s = sinf(angle);
         const auto c = cosf(angle);
         return basic_matrix{ One, Zero, Zero, Zero,
                        Zero, c, -s, Zero,
                        Zero, s, c, Zero,
                        Zero, Zero, Zero, One };
      }
      static constexpr basic_matrix rotate_y(NumericType angle) noexcept {
         const auto s = sinf(angle);
         const auto c = cosf(angle);
         return basic_matrix{ c, Zero, s, Zero,
                        Zero, One, Zero, Zero,
                        -s, Zero, c, Zero,
                        Zero, Zero, Zero, One };
      }
      static constexpr basic_matrix rotate_z(NumericType angle) noexcept {
         const auto s = sinf(angle);
         const auto c = cosf(angle);
         return basic_matrix{ c, -s, Zero, Zero,
                        s, c, Zero, Zero,
                        Zero, Zero, One, Zero,
                        Zero, Zero, Zero, One };
      }
      static constexpr basic_matrix translate(NumericType x, NumericType y, NumericType z) noexcept {
         return basic_matrix{ One, Zero, Zero, x,
                        Zero, One, Zero, y,
                        Zero, Zero, One, z,
                        Zero, Zero, Zero, One };
      }

      constexpr bool operator==(const basic_matrix& other) const {
         for (std::size_t i = 0; i < 4; ++i) {
            for (std::size_t j = 0; j < 4; ++j) {
               if (not fps::math::test_equal(m_data[i][j], other[i,j], .00001f)) {
                  return false;
               }
            }
         }
         return true;
      }

   };

   using matrix = basic_matrix<float>;
}

#endif // MATRIX_H
