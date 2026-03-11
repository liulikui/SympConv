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
    EXPECT_TRUE(FloatEqual(cone.mHeight, 1.0f));
    EXPECT_TRUE(FloatEqual(cone.mRadius, 0.5f));
}

TEST(ConeTest, ParameterizedConstructor) {
    Cone cone(2.0f, 1.0f);
    EXPECT_TRUE(FloatEqual(cone.mHeight, 2.0f));
    EXPECT_TRUE(FloatEqual(cone.mRadius, 1.0f));
}

} // namespace SympConvTest
