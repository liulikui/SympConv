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
    EXPECT_TRUE(FloatEqual(sphere.mCenter.x, 0.0f));
    EXPECT_TRUE(FloatEqual(sphere.mCenter.y, 0.0f));
    EXPECT_TRUE(FloatEqual(sphere.mCenter.z, 0.0f));
    EXPECT_TRUE(FloatEqual(sphere.mRadius, 1.0f));
}

TEST(SphereTest, ParameterizedConstructor) {
    Vec3 center(1.0f, 2.0f, 3.0f);
    Real radius = 2.5f;
    Sphere sphere(center, radius);
    EXPECT_TRUE(FloatEqual(sphere.mCenter.x, 1.0f));
    EXPECT_TRUE(FloatEqual(sphere.mCenter.y, 2.0f));
    EXPECT_TRUE(FloatEqual(sphere.mCenter.z, 3.0f));
    EXPECT_TRUE(FloatEqual(sphere.mRadius, 2.5f));
}

TEST(SphereTest, Contains) {
    Sphere sphere(Vec3(0, 0, 0), 1.0f);
    
    // 测试球心点
    EXPECT_TRUE(sphere.Contains(Vec3(0, 0, 0)));
    
    // 测试球面上的点
    EXPECT_TRUE(sphere.Contains(Vec3(1, 0, 0)));
    
    // 测试球内的点
    EXPECT_TRUE(sphere.Contains(Vec3(0.5, 0, 0)));
    
    // 测试球外的点
    EXPECT_FALSE(sphere.Contains(Vec3(1.1, 0, 0)));
}

TEST(SphereTest, DistanceTo) {
    Sphere sphere(Vec3(0, 0, 0), 1.0f);
    
    // 测试球心点
    EXPECT_TRUE(FloatEqual(sphere.DistanceTo(Vec3(0, 0, 0)), 0.0f));
    
    // 测试球面上的点
    EXPECT_TRUE(FloatEqual(sphere.DistanceTo(Vec3(1, 0, 0)), 0.0f));
    
    // 测试球外的点
    EXPECT_TRUE(FloatEqual(sphere.DistanceTo(Vec3(2, 0, 0)), 1.0f));
    
    // 测试球内的点
    EXPECT_TRUE(FloatEqual(sphere.DistanceTo(Vec3(0.5, 0, 0)), 0.0f));
}

} // namespace SympConvTest
