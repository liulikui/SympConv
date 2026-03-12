#include <gtest/gtest.h>
#include "SympConv/Box.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Box = SympConv::Box;
using Vec3 = SympConv::Vector3;
using Transform = SympConv::Transform;

TEST(BoxTest, DefaultConstructor) {
    Box box;
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.y, Real(0.5)));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.z, Real(0.5)));
}

TEST(BoxTest, ParameterizedConstructor) {
    Vec3 halfExtents(Real(2.0), Real(1.5), Real(1.0));
    Box box(halfExtents);
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.y, Real(1.5)));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.z, Real(1.0)));
}

} // namespace SympConvTest