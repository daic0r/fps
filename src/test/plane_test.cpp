#include <gtest/gtest.h>
#include "../math/plane.h"

TEST(PlaneTests, BasicAssertions) {
   using namespace fps::math;

   plane p{ vec3f{ 0.0f, 0.0f, 1.0f }, vec3f{ 0.0f, 0.0f, -1.0f } };

   EXPECT_TRUE(p.is_point_on_plane(vec3f{ 0.0f, 10.0f, -1.0f }));
   EXPECT_TRUE(p.is_point_on_plane(vec3f{ 0.0f, -10.0f, -1.0f }));

   EXPECT_EQ(p.get_orientation(vec3f{ 0.0f, 0.0f, 10.0f }), Orientation::IN_FRONT);
   EXPECT_EQ(p.get_orientation(vec3f{ 10.0f, 50.0f, 1.0f }), Orientation::IN_FRONT);

   EXPECT_EQ(p.get_orientation(vec3f{ 0.0f, 0.0f, -10.0f }), Orientation::BEHIND);
   EXPECT_EQ(p.get_orientation(vec3f{ 10.0f, 50.0f, -1.0001f }), Orientation::BEHIND);

   EXPECT_EQ(p.get_orientation(vec3f{ 10.0f, 50.0f, -1.0f }), Orientation::ON);
}

