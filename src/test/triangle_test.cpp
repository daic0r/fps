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

   auto vertices = std::vector<fps::math::vertex>{
      vertex{ -10.0, -10.0, -10.0, 1.0 },
      vertex{ 10.0, -10.0, -10.0, 1.0 },
      vertex{ 10.0, 10.0, -10.0, 1.0 },
      vertex{ -10.0, 10.0, -10.0, 1.0 },
      vertex{ -10.0, -10.0, 10.0, 1.0 }, // 4
      vertex{ 10.0, -10.0, 10.0, 1.0 },
      vertex{ 10.0, 10.0, 10.0, 1.0 }, // 6
      vertex{ -10.0, 10.0, 10.0, 1.0 }
   };
   // counter-clockwise indices
   auto indices = std::vector<std::size_t>{
      0, 1, 2, 2, 3, 0, // front
      5, 4, 7, 7, 6, 5, // back 
      0, 4, 3, 3, 4, 7, // left
      5, 1, 6, 6, 1, 2, // right
      3, 7, 2, 2, 7, 6, // top
      0, 1, 4, 4, 1, 5 // bottom
      // 4, 5, 6, 6, 7, 4,
      // 1, 0, 2, 2, 0, 3,
      // 2, 3, 6, 6, 3, 7,
      // 6, 7, 5, 5, 7, 4,
      // 5, 4, 1, 1, 4, 0,
      // 0, 4, 3, 3, 4, 7,
   };

   triangle tr{ vec3f{ -0.5f, 0.0f, 0.0f }, vec3f{ 0.5f, 0.0f, 0.0f }, vec3f{ 0.0f, 1.0f, 0.0f } };
   auto n = tr.normal();
   auto expected = vec3f{ 0.0f, 0.0f, 1.0f };
   EXPECT_EQ(n, expected);

   triangle tr2{ vec3f{ 0.5f, 0.0f, 0.0f }, vec3f{ -0.5f, 0.0f, 0.0f }, vec3f{ 0.0f, 1.0f, 0.0f } };
   n = tr2.normal();
   expected = vec3f{ 0.0f, 0.0f, -1.0f };
   EXPECT_EQ(n, expected);

   triangle front1{ vertices[0], vertices[1], vertices[2] };
   n = front1.normal();
   expected = vec3f{ 0.0f, 0.0f, 1.0f };
   EXPECT_EQ(n, expected);

   triangle front2{ vertices[2], vertices[3], vertices[0] };
   n = front2.normal();
   expected = vec3f{ 0.0f, 0.0f, 1.0f };
   EXPECT_EQ(n, expected);

   // check the other 5 faces of the cube
   triangle back1{ vertices[5], vertices[4], vertices[7] };
   n = back1.normal();
   expected = vec3f{ 0.0f, 0.0f, -1.0f };
   EXPECT_EQ(n, expected);

   triangle back2{ vertices[7], vertices[6], vertices[5] };
   n = back2.normal();
   expected = vec3f{ 0.0f, 0.0f, -1.0f };
   EXPECT_EQ(n, expected);

   triangle left1{ vertices[0], vertices[4], vertices[3] };
   n = left1.normal();
   expected = vec3f{ -1.0f, 0.0f, 0.0f };
   EXPECT_EQ(n, expected);

   triangle left2{ vertices[3], vertices[4], vertices[7] };
   n = left2.normal();
   expected = vec3f{ -1.0f, 0.0f, 0.0f };
   EXPECT_EQ(n, expected);

   triangle right1{ vertices[5], vertices[1], vertices[6] };
   n = right1.normal();
   expected = vec3f{ 1.0f, 0.0f, 0.0f };
   EXPECT_EQ(n, expected);

   triangle right2{ vertices[6], vertices[1], vertices[2] };
   n = right2.normal();
   expected = vec3f{ 1.0f, 0.0f, 0.0f };
   EXPECT_EQ(n, expected);

   triangle top1{ vertices[3], vertices[7], vertices[2] };
   n = top1.normal();
   expected = vec3f{ 0.0f, 1.0f, 0.0f };
   EXPECT_EQ(n, expected);

   triangle top2{ vertices[2], vertices[7], vertices[6] };
   n = top2.normal();
   expected = vec3f{ 0.0f, 1.0f, 0.0f };
   EXPECT_EQ(n, expected);

   triangle bottom1{ vertices[0], vertices[1], vertices[4] };
   n = bottom1.normal();
   expected = vec3f{ 0.0f, -1.0f, 0.0f };
   EXPECT_EQ(n, expected);

   triangle bottom2{ vertices[4], vertices[1], vertices[5] };
   n = bottom2.normal();
   expected = vec3f{ 0.0f, -1.0f, 0.0f };
   EXPECT_EQ(n, expected);
   
   triangle misc{ vec3{ 10.0f, -10.0f, 10.0f }, vec3{ 10.0f, -10.0f, -10.0f }, vec3{ -10.0f, 10.0f, -10.0f } };
   n = misc.normal();
   expected = vec3f{ 0.707107f, 0.707107f, 0.0f };
   std::cout << "Misc normal: " << n[0] << ' ' << n[1] << ' ' << n[2] << '\n';   
   std::cout << "Expected normal: " << expected[0] << ' ' << expected[1] << ' ' << expected[2] << '\n';
   EXPECT_EQ(n, expected);

   vec4f p1{-1.00297916, -0.214191183, 0.99873811, 1}; 
   vec4f p2{-0.482353061, -0.407408953, 0.998453021, 1}; 
   vec4f p3{-0.307203919, 0.389778435, 0.998638988, 1};

   triangle t{ p1, p2, p3 };

   vec4f p4{-1, -0.215296835, 0.998736501, 1};
   vec4f p5{-0.482353061, -0.407408953, 0.998453021, 1};
   vec4f p6{-0.307203919, 0.389778435, 0.998638988, 1};
   
   triangle t2{ p4, p5, p6 };
   
   auto n1 = t.normal();
   auto n2 = t2.normal();
   std::cout << "Normal: " << n1[0] << ", " << n1[1] << ", " << n1[2] << '\n';
   std::cout << "Normal: " << n2[0] << ", " << n2[1] << ", " << n2[2] << '\n';
   EXPECT_EQ(n1, n2);
}
