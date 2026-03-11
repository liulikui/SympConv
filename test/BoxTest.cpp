#include <gtest/gtest.h>
#include "SympConv/Box.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using fpnumber = SympConv::fpnumber;
using Box = SympConv::Box;
using Vec3 = SympConv::Vector3;
using Transform = SympConv::Transform;

TEST(BoxTest, DefaultConstructor) {
    Box box;
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.x, 0.5f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.y, 0.5f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.z, 0.5f));
}

TEST(BoxTest, ParameterizedConstructor) {
    Vec3 halfExtents(fpnumber(2.0), fpnumber(1.5), fpnumber(1.0));
    Box box(halfExtents);
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.x, 2.0f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.y, 1.5f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.z, 1.0f));
}

} // namespace SympConvTest