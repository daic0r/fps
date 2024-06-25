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

   if (ret.first) {
      std::cout << "First triangle:\n";
      std::cout << "(" << (*ret.first)[0][0] << ' ' << (*ret.first)[0][1] << ' ' << (*ret.first)[0][2] << ")" << '\n';
      std::cout << "(" << (*ret.first)[1][0] << ' ' << (*ret.first)[1][1] << ' ' << (*ret.first)[1][2] << ")" << '\n';
      std::cout << "(" << (*ret.first)[2][0] << ' ' << (*ret.first)[2][1] << ' ' << (*ret.first)[2][2] << ")" << '\n';
   }
   if (ret.second) {
      std::cout << "Second triangle:\n";
      std::cout << "(" << (*ret.second)[0][0] << ' ' << (*ret.second)[0][1] << ' ' << (*ret.second)[0][2] << ")" << '\n';
      std::cout << "(" << (*ret.second)[1][0] << ' ' << (*ret.second)[1][1] << ' ' << (*ret.second)[1][2] << ")" << '\n';
      std::cout << "(" << (*ret.second)[2][0] << ' ' << (*ret.second)[2][1] << ' ' << (*ret.second)[2][2] << ")" << '\n';
   }
}

