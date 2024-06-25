#include <gtest/gtest.h>
#include "../math/line.h"

TEST(LineTest, PlaneTests) {
   using namespace fps::math;

   line l{ vec3f{ 0.0f, 0.0f, 0.0f }, vec3f{ 1.0f, 1.0f, 1.0f } };

   plane pl{ vec3f{ 0.0f, 0.0f, 1.0f }, vec3f{ 0.0f, 0.0f, 0.5f } };

   auto res = l.intersects(pl);
   EXPECT_TRUE(res.has_value());
   EXPECT_EQ(res.value()[0], 0.5f);
   EXPECT_EQ(res.value()[1], 0.5f);
   EXPECT_EQ(res.value()[2], 0.5f);
}

TEST(LineTest, TriangleTests) {
   using namespace fps::math;

   line l{ vec3f{ 0.0f, 0.0f, 0.0f }, vec3f{ 1.0f, 1.0f, 1.0f } };

   triangle tr{ vec3f{ 0.0f, 0.0f, 0.0f }, vec3f{ 1.0f, 0.0f, 0.0f }, vec3f{ 0.0f, 1.0f, 0.0f } };

   auto res = l.intersects(tr);
   EXPECT_TRUE(res.has_value());
   EXPECT_EQ(res.value()[0], 0.0f);
   EXPECT_EQ(res.value()[1], 0.0f);
   EXPECT_EQ(res.value()[2], 0.0f);

   triangle tr2{ vec3f{ 0.0f, 0.0f, 0.5f }, vec3f{ 1.0f, 0.0f, 0.5f }, vec3f{ 0.0f, 1.0f, 0.5f } };

   res = l.intersects(tr2);
   EXPECT_TRUE(res.has_value());
   EXPECT_EQ(res.value()[0], 0.5f);
   EXPECT_EQ(res.value()[1], 0.5f);
   EXPECT_EQ(res.value()[2], 0.5f);
}
