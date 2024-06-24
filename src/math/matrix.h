#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include "vector.h"
#include "utils.h"

namespace fps::math {
   class matrix {
      std::array<std::array<float, 4>, 4> m_data{};

   public:
      constexpr matrix() = default;
      constexpr matrix(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) :
         m_data{
            std::array<float, 4>{m00, m01, m02, m03},
            std::array<float, 4>{m10, m11, m12, m13},
            std::array<float, 4>{m20, m21, m22, m23},
            std::array<float, 4>{m30, m31, m32, m33}
         } {}

      constexpr decltype(auto) operator[](std::size_t row, std::size_t col) {
         return (m_data[row][col]);
      }

      constexpr decltype(auto) operator[](std::size_t row, std::size_t col) const {
         return (m_data[row][col]);
      }


      constexpr vec4f operator*(const vec4f& v) const {
         return vec4f{
            m_data[0][0] * v[0] + m_data[0][1] * v[1] + m_data[0][2] * v[2] + m_data[0][3] * v[3],
            m_data[1][0] * v[0] + m_data[1][1] * v[1] + m_data[1][2] * v[2] + m_data[1][3] * v[3],
            m_data[2][0] * v[0] + m_data[2][1] * v[1] + m_data[2][2] * v[2] + m_data[2][3] * v[3],
            m_data[3][0] * v[0] + m_data[3][1] * v[1] + m_data[3][2] * v[2] + m_data[3][3] * v[3]
         };
      }

      constexpr matrix operator*(const matrix& m) const {
         matrix result{};
         for (std::size_t i = 0; i < 4; ++i) {
            for (std::size_t j = 0; j < 4; ++j) {
               result[i,j] = m_data[i][0] * m[0,j] + m_data[i][1] * m[1,j] + m_data[i][2] * m[2,j] + m_data[i][3] * m[3,j];
            }
         }
         return result;
      }

      static constexpr matrix perspective(float fovH, float fAspectRatio, float fNear, float fFar) {
         const auto fov2 = fovH * 0.5f;
         const auto denom = fNear - fFar;
         matrix ret{};
         ret[0,0] = 1.0f / tanf(fov2);
         ret[1,1] = 1.0f / tanf((1.0f / fAspectRatio) * fov2);
         ret[2,2] = -(fNear + fFar) / denom;
         ret[2,3] = (2.0f * fNear * fFar) / denom;
         ret[3,2] = -1.0f;
         return ret;
      }
      static constexpr matrix viewport(int x, int y, int width, int height) {
         matrix ret{};
         const auto w2 = 0.5f * width;
         const auto h2 = 0.5f * height;
         ret[0,0] = w2;
         ret[0,3] = x + w2;
         ret[1,1] = -h2;
         ret[1,3] = h2 + y;
         return ret;
      }

      constexpr bool operator==(const matrix& other) const {
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
}

#endif // MATRIX_H
