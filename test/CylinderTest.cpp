#include <gtest/gtest.h>
#include "SympConv/Cylinder.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Cylinder = SympConv::Cylinder;
using Vec3 = SympConv::Vector3;

TEST(CylinderTest, DefaultConstructor) {
    Cylinder cylinder;
    EXPECT_TRUE(FloatEqual(cylinder.mHalfHeight, Real(0.5)));
    EXPECT_TRUE(FloatEqual(cylinder.mRadius, Real(0.5)));
}

TEST(CylinderTest, ParameterizedConstructor) {
    Cylinder cylinder(Real(2.0), Real(1.0));
    EXPECT_TRUE(FloatEqual(cylinder.mHalfHeight, Real(1.0)));
    EXPECT_TRUE(FloatEqual(cylinder.mRadius, Real(1.0)));
}

} // namespace SympConvTest
