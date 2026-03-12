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

TEST(CapsuleTest, GetVolume) {
    // 测试默认构造的胶囊体体积
    Capsule capsule1;
    Real volume1 = capsule1.GetVolume();
    // 计算预期体积：圆柱体体积 + 两个半球体体积
    // 圆柱体：π * r² * h = π * 0.5² * 1 = π * 0.25 * 1 = 0.25π
    // 两个半球体：(4/3) * π * r³ = (4/3) * π * 0.5³ = (4/3) * π * 0.125 = 0.1667π
    // 总体积：0.25π + 0.1667π = 0.4167π ≈ 1.309
    EXPECT_TRUE(SympConv::RealEqual(volume1, Real(0.25 * M_PI + (4.0/3.0) * M_PI * 0.125)));
    
    // 测试自定义尺寸的胶囊体体积
    Capsule capsule2(Real(4.0), Real(2.0)); // 高度4，半径2，半高2
    Real volume2 = capsule2.GetVolume();
    // 圆柱体：π * 2² * 4 = π * 4 * 4 = 16π
    // 两个半球体：(4/3) * π * 2³ = (4/3) * π * 8 = 10.6667π
    // 总体积：16π + 10.6667π = 26.6667π ≈ 83.7758
    EXPECT_TRUE(SympConv::RealEqual(volume2, Real(16 * M_PI + (4.0/3.0) * M_PI * 8)));
    
    // 测试零高度胶囊体（退化为球体）
    Capsule capsule3(Real(0.0), Real(1.0)); // 高度0，半径1
    Real volume3 = capsule3.GetVolume();
    // 球体体积：(4/3) * π * r³ = (4/3) * π * 1³ = 4.1888π
    EXPECT_TRUE(SympConv::RealEqual(volume3, Real((4.0/3.0) * M_PI * 1.0)));
}

} // namespace SympConvTest
