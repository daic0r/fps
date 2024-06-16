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
   vector v1 = {1, 2, 3};
   EXPECT_EQ(v1.x, 1);
   EXPECT_EQ(v1.y, 2);
   EXPECT_EQ(v1.z, 3);
}

TEST(VectorTest, DotProduct) {
   using namespace fps::math;
   vector v1 = {1, 2, 3};
   vector v2 = {4, 5, 6};
   EXPECT_EQ(v1.dot(v2), 32);

   vector v3 = {1, 0, 0};
   vector v4 = {0, 1, 0};
   EXPECT_EQ(v3.dot(v4), 0);
}

TEST(VectorTest, CrossProduct) {
   using namespace fps::math;
   vector v1 = {1, 0, 0};
   vector v2 = {0, 1, 0};
   vector v3 = v1.cross(v2);
   EXPECT_EQ(v3.x, 0);
   EXPECT_EQ(v3.y, 0);
   EXPECT_EQ(v3.z, 1);

   vector v4 = {1, 2, 3};
   vector v5 = {4, 5, 6};
   vector v6 = v4.cross(v5);
   EXPECT_EQ(v6.x, -3);
   EXPECT_EQ(v6.y, 6);
   EXPECT_EQ(v6.z, -3);
}
