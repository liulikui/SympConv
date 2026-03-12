#include <gtest/gtest.h>
#include "SympConv/Capsule.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Capsule = SympConv::Capsule;
using Vec3 = SympConv::Vector3;

TEST(CapsuleTest, Constructor) {
    // 默认构造函数
    Capsule capsule1;
    EXPECT_EQ(capsule1.mHalfHeight, Real(0.5));
    EXPECT_EQ(capsule1.mRadius, Real(0.5));
    
    // 带参数的构造函数
    Capsule capsule2(Real(2.0), Real(1.0));
    EXPECT_EQ(capsule2.mHalfHeight, Real(1.0));
    EXPECT_EQ(capsule2.mRadius, Real(1.0));
}

} // namespace SympConvTest
