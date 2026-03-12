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
    EXPECT_TRUE(SympConv::RealEqual(cone.mHeight, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(cone.mRadius, Real(0.5)));
}

TEST(ConeTest, ParameterizedConstructor) {
    Cone cone(Real(2.0), Real(1.0));
    EXPECT_TRUE(SympConv::RealEqual(cone.mHeight, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(cone.mRadius, Real(1.0)));
}

TEST(ConeTest, GetVolume) {
    // 测试默认构造的圆锥体体积
    Cone cone1;
    Real volume1 = cone1.GetVolume();
    // 计算预期体积：(1/3) * π * r² * h = (1/3) * π * 0.5² * 1 = (1/3) * π * 0.25 * 1 = 0.0833π ≈ 0.2618
    EXPECT_TRUE(SympConv::RealEqual(volume1, Real((1.0/3.0) * M_PI * 0.5 * 0.5 * 1.0)));
    
    // 测试自定义尺寸的圆锥体体积
    Cone cone2(Real(6.0), Real(3.0)); // 高度6，半径3
    Real volume2 = cone2.GetVolume();
    // 计算预期体积：(1/3) * π * 3² * 6 = (1/3) * π * 9 * 6 = 18π ≈ 56.5487
    EXPECT_TRUE(SympConv::RealEqual(volume2, Real((1.0/3.0) * M_PI * 3.0 * 3.0 * 6.0)));
    
    // 测试零高度圆锥体
    Cone cone3(Real(0.0), Real(2.0)); // 高度0，半径2
    Real volume3 = cone3.GetVolume();
    // 零高度圆锥体体积应该为0
    EXPECT_TRUE(SympConv::RealEqual(volume3, Real(0.0)));
    
    // 测试零半径圆锥体
    Cone cone4(Real(5.0), Real(0.0)); // 高度5，半径0
    Real volume4 = cone4.GetVolume();
    // 零半径圆锥体体积应该为0
    EXPECT_TRUE(SympConv::RealEqual(volume4, Real(0.0)));
}

} // namespace SympConvTest
