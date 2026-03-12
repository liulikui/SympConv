#include <gtest/gtest.h>
#include "SympConv/Sphere.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Sphere = SympConv::Sphere;
using Vec3 = SympConv::Vector3;

TEST(SphereTest, DefaultConstructor) {
    Sphere sphere;
    EXPECT_TRUE(SympConv::RealEqual(sphere.mCenter.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(sphere.mCenter.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(sphere.mCenter.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(sphere.mRadius, Real(1.0)));
}

TEST(SphereTest, ParameterizedConstructor) {
    Vec3 center(Real(1.0), Real(2.0), Real(3.0));
    Real radius = Real(2.5);
    Sphere sphere(center, radius);
    EXPECT_TRUE(SympConv::RealEqual(sphere.mCenter.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(sphere.mCenter.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(sphere.mCenter.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(sphere.mRadius, Real(2.5)));
}

TEST(SphereTest, Contains) {
    Sphere sphere(Vec3(Real(0), Real(0), Real(0)), Real(1.0));
    
    // 测试球心点
    EXPECT_TRUE(sphere.Contains(Vec3(Real(0), Real(0), Real(0))));
    
    // 测试球面上的点
    EXPECT_TRUE(sphere.Contains(Vec3(Real(1), Real(0), Real(0))));
    
    // 测试球内的点
    EXPECT_TRUE(sphere.Contains(Vec3(Real(0.5), Real(0), Real(0))));
    
    // 测试球外的点
    EXPECT_FALSE(sphere.Contains(Vec3(Real(1.1), Real(0), Real(0))));
}

TEST(SphereTest, DistanceTo) {
    Sphere sphere(Vec3(Real(0), Real(0), Real(0)), Real(1.0));
    
    // 测试球心点
    EXPECT_TRUE(SympConv::RealEqual(sphere.DistanceTo(Vec3(Real(0), Real(0), Real(0))), Real(0.0)));
    
    // 测试球面上的点
    EXPECT_TRUE(SympConv::RealEqual(sphere.DistanceTo(Vec3(Real(1), Real(0), Real(0))), Real(0.0)));
    
    // 测试球外的点
    EXPECT_TRUE(SympConv::RealEqual(sphere.DistanceTo(Vec3(Real(2), Real(0), Real(0))), Real(1.0)));
    
    // 测试球内的点
    EXPECT_TRUE(SympConv::RealEqual(sphere.DistanceTo(Vec3(Real(0.5), Real(0), Real(0))), Real(0.0)));
}

} // namespace SympConvTest
