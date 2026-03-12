#include <gtest/gtest.h>
#include "SympConv/Cone.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Cone = SympConv::Cone;
using Vec3 = SympConv::Vector3;

TEST(ConeTest, DefaultConstructor) {
    Cone cone;
    EXPECT_TRUE(FloatEqual(cone.mHeight, Real(1.0)));
    EXPECT_TRUE(FloatEqual(cone.mRadius, Real(0.5)));
}

TEST(ConeTest, ParameterizedConstructor) {
    Cone cone(Real(2.0), Real(1.0));
    EXPECT_TRUE(FloatEqual(cone.mHeight, Real(2.0)));
    EXPECT_TRUE(FloatEqual(cone.mRadius, Real(1.0)));
}

} // namespace SympConvTest
