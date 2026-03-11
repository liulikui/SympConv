#include <gtest/gtest.h>
#include "SympConv/Cylinder.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using fpnumber = SympConv::fpnumber;
using Cylinder = SympConv::Cylinder;
using Vec3 = SympConv::Vector3;

TEST(CylinderTest, DefaultConstructor) {
    Cylinder cylinder;
    EXPECT_TRUE(FloatEqual(cylinder.mHeight, 1.0f));
    EXPECT_TRUE(FloatEqual(cylinder.mRadius, 0.5f));
}

TEST(CylinderTest, ParameterizedConstructor) {
    Cylinder cylinder(2.0f, 1.0f);
    EXPECT_TRUE(FloatEqual(cylinder.mHeight, 2.0f));
    EXPECT_TRUE(FloatEqual(cylinder.mRadius, 1.0f));
}

} // namespace SympConvTest
