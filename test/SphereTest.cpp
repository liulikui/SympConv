#include <gtest/gtest.h>
#include "SympConv/Sphere.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

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
    float radius = 2.5f;
    Sphere sphere(center, radius);
    EXPECT_TRUE(FloatEqual(sphere.mCenter.x, 1.0f));
    EXPECT_TRUE(FloatEqual(sphere.mCenter.y, 2.0f));
    EXPECT_TRUE(FloatEqual(sphere.mCenter.z, 3.0f));
    EXPECT_TRUE(FloatEqual(sphere.mRadius, 2.5f));
}

TEST(SphereTest, ContainsPoint) {
    Vec3 center(0.0f, 0.0f, 0.0f);
    float radius = 1.0f;
    Sphere sphere(center, radius);
    
    // 测试点在球体内
    Vec3 insidePoint(0.0f, 0.0f, 0.0f);
    EXPECT_TRUE(sphere.Contains(insidePoint));
    
    // 测试点在球体表面
    Vec3 surfacePoint(1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(sphere.Contains(surfacePoint));
    
    // 测试点在球体外
    Vec3 outsidePoint(1.1f, 0.0f, 0.0f);
    EXPECT_FALSE(sphere.Contains(outsidePoint));
}

TEST(SphereTest, DistanceTo) {
    Vec3 center(0.0f, 0.0f, 0.0f);
    float radius = 1.0f;
    Sphere sphere(center, radius);
    
    // 测试点在球体内
    Vec3 insidePoint(0.0f, 0.0f, 0.0f);
    EXPECT_TRUE(FloatEqual(sphere.DistanceTo(insidePoint), 0.0f));
    
    // 测试点在球体表面
    Vec3 surfacePoint(1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(FloatEqual(sphere.DistanceTo(surfacePoint), 0.0f));
    
    // 测试点在球体外
    Vec3 outsidePoint(2.0f, 0.0f, 0.0f);
    EXPECT_TRUE(FloatEqual(sphere.DistanceTo(outsidePoint), 1.0f));
}

TEST(SphereTest, GetAABB) {
    Vec3 center(1.0f, 2.0f, 3.0f);
    float radius = 2.0f;
    Sphere sphere(center, radius);
    
    auto aabb = sphere.GetAABB();
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, -1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.y, 0.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.z, 1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 3.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.y, 4.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.z, 5.0f));
}

} // namespace SympConvTest