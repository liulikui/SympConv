#include <gtest/gtest.h>
#include "SympConv/Box.h"
#include "SympConv/Vector.h"
#include "SympConv/Transform.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Box = SympConv::Box;
using Vec3 = SympConv::Vector3;
using Transform = SympConv::Transform;

TEST(BoxTest, DefaultConstructor) {
    Box box;
    EXPECT_TRUE(SympConv::RealEqual(box.mHalfExtents.x, Real(0.5)));
    EXPECT_TRUE(SympConv::RealEqual(box.mHalfExtents.y, Real(0.5)));
    EXPECT_TRUE(SympConv::RealEqual(box.mHalfExtents.z, Real(0.5)));
}

TEST(BoxTest, ParameterizedConstructor) {
    Vec3 halfExtents(Real(2.0), Real(1.5), Real(1.0));
    Box box(halfExtents);
    EXPECT_TRUE(SympConv::RealEqual(box.mHalfExtents.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(box.mHalfExtents.y, Real(1.5)));
    EXPECT_TRUE(SympConv::RealEqual(box.mHalfExtents.z, Real(1.0)));
}

TEST(BoxTest, GetVolume) {
    // 测试默认构造的盒子体积
    Box box1;
    Real volume1 = box1.GetVolume();
    // 计算预期体积：8 * 半长x * 半长y * 半长z = 8 * 0.5 * 0.5 * 0.5 = 1.0
    EXPECT_TRUE(SympConv::RealEqual(volume1, Real(1.0)));
    
    // 测试自定义尺寸的盒子体积
    Vec3 halfExtents(Real(2.0), Real(1.5), Real(1.0));
    Box box2(halfExtents);
    Real volume2 = box2.GetVolume();
    // 计算预期体积：8 * 2.0 * 1.5 * 1.0 = 24.0
    EXPECT_TRUE(SympConv::RealEqual(volume2, Real(24.0)));
    
    // 测试零体积盒子（所有半长为0）
    Vec3 zeroExtents(Real(0.0), Real(0.0), Real(0.0));
    Box box3(zeroExtents);
    Real volume3 = box3.GetVolume();
    // 零体积盒子体积应该为0
    EXPECT_TRUE(SympConv::RealEqual(volume3, Real(0.0)));
    
    // 测试部分零半长的盒子
    Vec3 partialZeroExtents(Real(2.0), Real(0.0), Real(3.0));
    Box box4(partialZeroExtents);
    Real volume4 = box4.GetVolume();
    // 部分零半长的盒子体积应该为0
    EXPECT_TRUE(SympConv::RealEqual(volume4, Real(0.0)));
}

} // namespace SympConvTest