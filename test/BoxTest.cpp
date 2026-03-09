#include <gtest/gtest.h>
#include "SympConv/Box.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Box = SympConv::Box;
using Vec3 = SympConv::Vector3;
using Transform = SympConv::Transform;

TEST(BoxTest, DefaultConstructor) {
    Box box;
    EXPECT_TRUE(FloatEqual(box.mCenter.x, 0.0f));
    EXPECT_TRUE(FloatEqual(box.mCenter.y, 0.0f));
    EXPECT_TRUE(FloatEqual(box.mCenter.z, 0.0f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.x, 0.5f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.y, 0.5f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.z, 0.5f));
}

TEST(BoxTest, ParameterizedConstructor) {
    Vec3 center(1.0f, 2.0f, 3.0f);
    Vec3 halfExtents(2.0f, 1.5f, 1.0f);
    Box box(center, halfExtents);
    EXPECT_TRUE(FloatEqual(box.mCenter.x, 1.0f));
    EXPECT_TRUE(FloatEqual(box.mCenter.y, 2.0f));
    EXPECT_TRUE(FloatEqual(box.mCenter.z, 3.0f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.x, 2.0f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.y, 1.5f));
    EXPECT_TRUE(FloatEqual(box.mHalfExtents.z, 1.0f));
}

TEST(BoxTest, ContainsPoint) {
    Vec3 center(0.0f, 0.0f, 0.0f);
    Vec3 halfExtents(1.0f, 1.0f, 1.0f);
    Box box(center, halfExtents);
    
    // 测试点在盒子内
    Vec3 insidePoint(0.0f, 0.0f, 0.0f);
    EXPECT_TRUE(box.Contains(insidePoint));
    
    // 测试点在盒子表面
    Vec3 surfacePoint(1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(box.Contains(surfacePoint));
    
    // 测试点在盒子外
    Vec3 outsidePoint(1.1f, 0.0f, 0.0f);
    EXPECT_FALSE(box.Contains(outsidePoint));
}

TEST(BoxTest, GetVertices) {
    Vec3 center(0.0f, 0.0f, 0.0f);
    Vec3 halfExtents(1.0f, 1.0f, 1.0f);
    Box box(center, halfExtents);
    
    Vec3 vertices[8];
    box.GetVertices(vertices);
    
    // 检查顶点坐标
    EXPECT_TRUE(FloatEqual(vertices[0].x, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[0].y, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[0].z, -1.0f));
    
    EXPECT_TRUE(FloatEqual(vertices[1].x, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[1].y, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[1].z, -1.0f));
    
    EXPECT_TRUE(FloatEqual(vertices[2].x, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[2].y, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[2].z, -1.0f));
    
    EXPECT_TRUE(FloatEqual(vertices[3].x, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[3].y, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[3].z, -1.0f));
    
    EXPECT_TRUE(FloatEqual(vertices[4].x, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[4].y, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[4].z, 1.0f));
    
    EXPECT_TRUE(FloatEqual(vertices[5].x, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[5].y, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[5].z, 1.0f));
    
    EXPECT_TRUE(FloatEqual(vertices[6].x, -1.0f));
    EXPECT_TRUE(FloatEqual(vertices[6].y, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[6].z, 1.0f));
    
    EXPECT_TRUE(FloatEqual(vertices[7].x, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[7].y, 1.0f));
    EXPECT_TRUE(FloatEqual(vertices[7].z, 1.0f));
}

TEST(BoxTest, GetAABB) {
    Vec3 center(1.0f, 2.0f, 3.0f);
    Vec3 halfExtents(2.0f, 1.5f, 1.0f);
    Box box(center, halfExtents);
    
    auto aabb = box.GetAABB();
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, -1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.y, 0.5f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.z, 2.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 3.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.y, 3.5f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.z, 4.0f));
}

} // namespace SympConvTest