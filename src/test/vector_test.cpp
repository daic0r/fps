#include <gtest/gtest.h>
#include "../math/vector.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
   // Expect two strings not to be equal.
   EXPECT_STRNE("hello", "world");
   // Expect equality.
   EXPECT_EQ(7 * 6, 42);
}

TEST(VectorTest, BasicAssertions) {
   using namespace fps::math;
   vec3 v1 = {1, 2, 3};
   EXPECT_EQ(v1[0], 1);
   EXPECT_EQ(v1[1], 2);
   EXPECT_EQ(v1[2], 3);
}

TEST(VectorTest, DotProduct) {
   using namespace fps::math;
   vec3 v1 = {1, 2, 3};
   vec3 v2 = {4, 5, 6};
   EXPECT_EQ(v1.dot(v2), 32);

   vec3 v3 = {1, 0, 0};
   vec3 v4 = {0, 1, 0};
   EXPECT_EQ(v3.dot(v4), 0);
}

TEST(VectorTest, CrossProduct) {
   using namespace fps::math;
   vec3 v1 = {1, 0, 0};
   vec3 v2 = {0, 1, 0};
   vec3 v3 = v1.cross(v2);
   EXPECT_EQ(v3[0], 0);
   EXPECT_EQ(v3[1], 0);
   EXPECT_EQ(v3[2], 1);

   vec3 v4 = {1, 2, 3};
   vec3 v5 = {4, 5, 6};
   vec3 v6 = v4.cross(v5);
   EXPECT_EQ(v6[0], -3);
   EXPECT_EQ(v6[1], 6);
   EXPECT_EQ(v6[2], -3);
}
