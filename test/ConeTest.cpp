#include "gtest/gtest.h"
#include "SympConv/Cone.h"
#include "TestUtils.h"

using namespace SympConv;
using namespace SympConvTest;

TEST(ConeTest, DefaultConstructor)
{
    Cone cone;
    EXPECT_TRUE(FloatEqual(cone.mVertex.x, 0) && FloatEqual(cone.mVertex.y, 0) && FloatEqual(cone.mVertex.z, 1));
    EXPECT_TRUE(FloatEqual(cone.mBaseCenter.x, 0) && FloatEqual(cone.mBaseCenter.y, 0) && FloatEqual(cone.mBaseCenter.z, 0));
    EXPECT_TRUE(FloatEqual(cone.mRadius, 0.5f));
}

TEST(ConeTest, ParameterizedConstructor)
{
    Vector3 vertex(1, 2, 3);
    Vector3 baseCenter(4, 5, 6);
    float radius = 1.5f;
    Cone cone(vertex, baseCenter, radius);
    EXPECT_TRUE(FloatEqual(cone.mVertex.x, vertex.x) && FloatEqual(cone.mVertex.y, vertex.y) && FloatEqual(cone.mVertex.z, vertex.z));
    EXPECT_TRUE(FloatEqual(cone.mBaseCenter.x, baseCenter.x) && FloatEqual(cone.mBaseCenter.y, baseCenter.y) && FloatEqual(cone.mBaseCenter.z, baseCenter.z));
    EXPECT_TRUE(FloatEqual(cone.mRadius, radius));
}

TEST(ConeTest, GetCenter)
{
    Vector3 vertex(1, 2, 3);
    Vector3 baseCenter(4, 5, 6);
    Cone cone(vertex, baseCenter, 1.5f);
    Vector3 center = cone.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 2.5f) && FloatEqual(center.y, 3.5f) && FloatEqual(center.z, 4.5f));
}

TEST(ConeTest, GetHeight)
{
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 0.5f);
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 1.0f));
}

TEST(ConeTest, GetDirection)
{
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 0.5f);
    Vector3 direction = cone.GetDirection();
    EXPECT_TRUE(FloatEqual(direction.x, 0) && FloatEqual(direction.y, 0) && FloatEqual(direction.z, 1));
}

TEST(ConeTest, GetAABB)
{
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 0.5f);
    AABB aabb = cone.GetAABB();
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, -0.5f) && FloatEqual(aabb.mMin.y, -0.5f) && FloatEqual(aabb.mMin.z, 0));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 0.5f) && FloatEqual(aabb.mMax.y, 0.5f) && FloatEqual(aabb.mMax.z, 1));
}

TEST(ConeTest, Contains)
{
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 1.0f);
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试底面上的点
    EXPECT_TRUE(cone.Contains(Vector3(1, 0, 0)));
    
    // 测试圆锥体内部的点
    EXPECT_TRUE(cone.Contains(Vector3(0.5f, 0, 0.5f)));
    
    // 测试圆锥体外部的点
    EXPECT_FALSE(cone.Contains(Vector3(2, 0, 0)));
    EXPECT_FALSE(cone.Contains(Vector3(0, 0, 2)));
}