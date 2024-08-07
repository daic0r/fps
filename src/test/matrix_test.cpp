#include <gtest/gtest.h>
#include "../math/matrix.h"
#include <numbers>
#include <iostream>

TEST(MatrixTests, MatrixMultiplication) {
   using namespace fps::math;

   matrix m1{
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
   };
   matrix m2{
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
   };

   matrix result = m1 * m2;
   matrix expected = {
      90.0f, 100.0f, 110.0f, 120.0f,
      202.0f, 228.0f, 254.0f, 280.0f,
      314.0f, 356.0f, 398.0f, 440.0f,
      426.0f, 484.0f, 542.0f, 600.0f
   };
   EXPECT_EQ(result, expected);

}

TEST(MatrixTests, PerspectiveMatrix) {
   using namespace fps::math;

   auto m = matrix::perspective(std::numbers::pi_v<float> / 4.0f, 4.0f/3.0f, 0.1f, 100.0f);

   matrix expected = {
      1.81066f, 0.0f, 0.0f, 0.0f,
      0.0f, 2.41421f, 0.0f, 0.0f,
      0.000000f, 0.000000f, -1.002002f, -0.200200f,
      0.000000f, 0.000000f, -1.000000f, 0.000000f
   };
   // print it
   // for (auto i = 0; i < 4; ++i) {
   //    for (auto j = 0; j < 4; ++j) {
   //       std::cout << m[i,j] << ' ';
   //    }
   //    std::cout << '\n';
   // }
   EXPECT_EQ(m, expected);
}

TEST(MatrixTests, ViewportMatrix) {
   using namespace fps::math;

   auto m = matrix::viewport(0, 0, 800, 600);

   matrix expected = {
      399.5f, 0.0f, 0.0f, 399.5f,
      0.0f, -299.5f, 0.0f, 299.5f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f
   };
   EXPECT_EQ(m, expected);
}
