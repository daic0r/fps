#include <gtest/gtest.h>
#include "../math/triangle.h"
#include "../math/plane.h"
#include "../math/clipping.h"
#include <iostream>

TEST(TriangleTest, Clipping) {
   using namespace fps::math;

   plane p{ vec3f{ -1.0f, 0.0f, 0.0f }, vec3f{ 0.1f, 0.0f, 0.0f } };
   triangle tr{ vec3f{ -0.5f, 0.0f, 0.0f }, vec3f{ 0.5f, 0.0f, 0.0f }, vec3f{ 0.0f, 1.0f, 0.0f } };

   auto ret = clip(tr, p);

   if (std::get<1>(ret)) {
      std::cout << "First triangle:\n";
      std::cout << "(" << (*std::get<1>(ret))[0][0] << ' ' << (*std::get<1>(ret))[0][1] << ' ' << (*std::get<1>(ret))[0][2] << ")" << '\n';
      std::cout << "(" << (*std::get<1>(ret))[1][0] << ' ' << (*std::get<1>(ret))[1][1] << ' ' << (*std::get<1>(ret))[1][2] << ")" << '\n';
      std::cout << "(" << (*std::get<1>(ret))[2][0] << ' ' << (*std::get<1>(ret))[2][1] << ' ' << (*std::get<1>(ret))[2][2] << ")" << '\n';
   }
   if (std::get<2>(ret)) {
      std::cout << "Second triangle:\n";
      std::cout << "(" << (*std::get<2>(ret))[0][0] << ' ' << (*std::get<2>(ret))[0][1] << ' ' << (*std::get<2>(ret))[0][2] << ")" << '\n';
      std::cout << "(" << (*std::get<2>(ret))[1][0] << ' ' << (*std::get<2>(ret))[1][1] << ' ' << (*std::get<2>(ret))[1][2] << ")" << '\n';
      std::cout << "(" << (*std::get<2>(ret))[2][0] << ' ' << (*std::get<2>(ret))[2][1] << ' ' << (*std::get<2>(ret))[2][2] << ")" << '\n';
   }
}

TEST(TriangleTest, NormalTest) {
   using namespace fps::math;

   triangle tr{ vec3f{ -0.5f, 0.0f, 0.0f }, vec3f{ 0.5f, 0.0f, 0.0f }, vec3f{ 0.0f, 1.0f, 0.0f } };
   auto n = tr.normal();
   auto expected = vec3f{ 0.0f, 0.0f, 1.0f };
   EXPECT_EQ(n, expected);

   triangle tr2{ vec3f{ 0.5f, 0.0f, 0.0f }, vec3f{ -0.5f, 0.0f, 0.0f }, vec3f{ 0.0f, 1.0f, 0.0f } };
   n = tr2.normal();
   expected = vec3f{ 0.0f, 0.0f, -1.0f };
   EXPECT_EQ(n, expected);
}
