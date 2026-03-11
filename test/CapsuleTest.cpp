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
    EXPECT_EQ(capsule1.mHalfHeight, 0.5f);
    EXPECT_EQ(capsule1.mRadius, 0.5f);
    
    // 带参数的构造函数
    Capsule capsule2(2.0f, 1.0f);
    EXPECT_EQ(capsule2.mHalfHeight, 1.0f);
    EXPECT_EQ(capsule2.mRadius, 1.0f);
}

} // namespace SympConvTest
