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
    EXPECT_TRUE(SympConv::RealEqual(cylinder.mHalfHeight, Real(0.5)));
    EXPECT_TRUE(SympConv::RealEqual(cylinder.mRadius, Real(0.5)));
}

TEST(CylinderTest, ParameterizedConstructor) {
    Cylinder cylinder(Real(2.0), Real(1.0));
    EXPECT_TRUE(SympConv::RealEqual(cylinder.mHalfHeight, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(cylinder.mRadius, Real(1.0)));
}

TEST(CylinderTest, GetVolume) {
    // 测试默认构造的圆柱体体积
    Cylinder cylinder1;
    Real volume1 = cylinder1.GetVolume();
    // 计算预期体积：π * r² * h = π * 0.5² * 1 = 0.25π ≈ 0.7854
    EXPECT_TRUE(SympConv::RealEqual(volume1, Real(M_PI * 0.5 * 0.5 * 1.0)));
    
    // 测试自定义尺寸的圆柱体体积
    Cylinder cylinder2(Real(4.0), Real(2.0)); // 高度4，半径2，半高2
    Real volume2 = cylinder2.GetVolume();
    // 计算预期体积：π * 2² * 4 = 16π ≈ 50.2655
    EXPECT_TRUE(SympConv::RealEqual(volume2, Real(M_PI * 2.0 * 2.0 * 4.0)));
    
    // 测试零高度圆柱体
    Cylinder cylinder3(Real(0.0), Real(2.0)); // 高度0，半径2
    Real volume3 = cylinder3.GetVolume();
    // 零高度圆柱体体积应该为0
    EXPECT_TRUE(SympConv::RealEqual(volume3, Real(0.0)));
    
    // 测试零半径圆柱体
    Cylinder cylinder4(Real(5.0), Real(0.0)); // 高度5，半径0
    Real volume4 = cylinder4.GetVolume();
    // 零半径圆柱体体积应该为0
    EXPECT_TRUE(SympConv::RealEqual(volume4, Real(0.0)));
}

} // namespace SympConvTest
