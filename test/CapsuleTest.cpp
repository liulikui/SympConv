#include <gtest/gtest.h>
#include "SympConv/Capsule.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/Plane.h"
#include "SympConv/Box.h"
#include "SympConv/Sphere.h"
#include "SympConv/CollisionDetection.h"
#include "TestUtils.h"

namespace SympConvTest {

using Capsule = SympConv::Capsule;
using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;
using Plane = SympConv::Plane;
using Box = SympConv::Box;
using Sphere = SympConv::Sphere;

TEST(CapsuleTest, DefaultConstructor) {
    Capsule capsule;
    EXPECT_TRUE(FloatEqual(capsule.mStart.x, 0.0f));
    EXPECT_TRUE(FloatEqual(capsule.mStart.y, 0.0f));
    EXPECT_TRUE(FloatEqual(capsule.mStart.z, 0.0f));
    EXPECT_TRUE(FloatEqual(capsule.mEnd.x, 0.0f));
    EXPECT_TRUE(FloatEqual(capsule.mEnd.y, 0.0f));
    EXPECT_TRUE(FloatEqual(capsule.mEnd.z, 1.0f));
    EXPECT_TRUE(FloatEqual(capsule.mRadius, 0.5f));
}

TEST(CapsuleTest, ParameterizedConstructor) {
    Vec3 start(1.0f, 2.0f, 3.0f);
    Vec3 end(4.0f, 5.0f, 6.0f);
    float radius = 1.5f;
    Capsule capsule(start, end, radius);
    EXPECT_TRUE(FloatEqual(capsule.mStart.x, 1.0f));
    EXPECT_TRUE(FloatEqual(capsule.mEnd.x, 4.0f));
    EXPECT_TRUE(FloatEqual(capsule.mRadius, 1.5f));
}

TEST(CapsuleTest, GetCenter) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(2.0f, 2.0f, 2.0f);
    Capsule capsule(start, end, 0.5f);
    Vec3 center = capsule.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 1.0f));
    EXPECT_TRUE(FloatEqual(center.y, 1.0f));
    EXPECT_TRUE(FloatEqual(center.z, 1.0f));
}

TEST(CapsuleTest, GetLength) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 3.0f);
    Capsule capsule(start, end, 0.5f);
    float length = capsule.GetLength();
    EXPECT_TRUE(FloatEqual(length, 3.0f));
}

TEST(CapsuleTest, GetDirection) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 1.0f);
    Capsule capsule(start, end, 0.5f);
    Vec3 direction = capsule.GetDirection();
    EXPECT_TRUE(FloatEqual(direction.x, 0.0f));
    EXPECT_TRUE(FloatEqual(direction.y, 0.0f));
    EXPECT_TRUE(FloatEqual(direction.z, 1.0f));
}

TEST(CapsuleTest, GetAABB) {
    Vec3 start(1.0f, 1.0f, 1.0f);
    Vec3 end(3.0f, 3.0f, 3.0f);
    float radius = 0.5f;
    Capsule capsule(start, end, radius);
    auto aabb = capsule.GetAABB();
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, 0.5f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 3.5f));
}

TEST(CapsuleTest, Contains) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 测试点在胶囊体内
    Vec3 insidePoint(0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(capsule.Contains(insidePoint));
    
    // 测试点在胶囊体表面
    Vec3 surfacePoint(1.0f, 0.0f, 1.0f);
    EXPECT_TRUE(capsule.Contains(surfacePoint));
    
    // 测试点在胶囊体外
    Vec3 outsidePoint(1.1f, 0.0f, 1.0f);
    EXPECT_FALSE(capsule.Contains(outsidePoint));
}

} // namespace SympConvTest
