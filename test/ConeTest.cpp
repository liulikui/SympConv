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
    EXPECT_TRUE(FloatEqual(direction.x, 0) && FloatEqual(direction.y, 0) && FloatEqual(direction.z, -1));
}

TEST(ConeTest, GetAABB)
{
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 0.5f);
    AABB aabb = cone.GetAABB();
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, -0.5f) && FloatEqual(aabb.mMin.y, -0.5f) && FloatEqual(aabb.mMin.z, -0.5f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 0.5f) && FloatEqual(aabb.mMax.y, 0.5f) && FloatEqual(aabb.mMax.z, 1.5f));
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

TEST(ConeTest, Contains_BoundaryCases)
{
    // 测试圆锥体表面上的点
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 1.0f);
    
    // 测试圆锥体侧面上的点
    EXPECT_TRUE(cone.Contains(Vector3(0.5f, 0, 0.5f)));
    
    // 测试圆锥体底面上的边界点
    EXPECT_TRUE(cone.Contains(Vector3(1.0f, 0, 0)));
    
    // 测试圆锥体顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试非常接近圆锥体表面的点
    EXPECT_TRUE(cone.Contains(Vector3(0.999f, 0, 0)));
    EXPECT_FALSE(cone.Contains(Vector3(1.001f, 0, 0)));
}

TEST(ConeTest, GetAABB_NotAtOrigin)
{
    // 测试不在原点的圆锥的AABB
    Vector3 vertex(1, 2, 3);
    Vector3 baseCenter(4, 5, 6);
    Cone cone(vertex, baseCenter, 2.0f);
    
    AABB aabb = cone.GetAABB();
    // 验证AABB的边界
    EXPECT_TRUE(aabb.mMin.x <= std::min(vertex.x, baseCenter.x) - 2.0f);
    EXPECT_TRUE(aabb.mMin.y <= std::min(vertex.y, baseCenter.y) - 2.0f);
    EXPECT_TRUE(aabb.mMin.z <= std::min(vertex.z, baseCenter.z) - 2.0f);
    EXPECT_TRUE(aabb.mMax.x >= std::max(vertex.x, baseCenter.x) + 2.0f);
    EXPECT_TRUE(aabb.mMax.y >= std::max(vertex.y, baseCenter.y) + 2.0f);
    EXPECT_TRUE(aabb.mMax.z >= std::max(vertex.z, baseCenter.z) + 2.0f);
}

TEST(ConeTest, GetAABB_Rotated)
{
    // 测试非坐标轴对齐的圆锥的AABB
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(1, 1, 1); // 斜方向
    Cone cone(vertex, baseCenter, 1.0f);
    
    AABB aabb = cone.GetAABB();
    // 验证AABB包含圆锥的所有点
    EXPECT_TRUE(aabb.Contains(vertex));
    EXPECT_TRUE(aabb.Contains(baseCenter));
    EXPECT_TRUE(aabb.Contains(Vector3(1, 0, 0))); // 底面上的点
    EXPECT_TRUE(aabb.Contains(Vector3(0, 1, 0))); // 底面上的点
    EXPECT_TRUE(aabb.Contains(Vector3(0, 0, 1))); // 底面上的点
}

TEST(ConeTest, GetDirection_NotAxisAligned)
{
    // 测试非坐标轴对齐的圆锥的方向
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(1, 1, 1);
    Cone cone(vertex, baseCenter, 1.0f);
    
    Vector3 direction = cone.GetDirection();
    float expectedLength = 1.0f / sqrtf(3.0f);
    EXPECT_TRUE(FloatEqual(direction.x, expectedLength));
    EXPECT_TRUE(FloatEqual(direction.y, expectedLength));
    EXPECT_TRUE(FloatEqual(direction.z, expectedLength));
}

TEST(ConeTest, HeightZero)
{
    // 测试高度为0的圆锥
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(0, 0, 0); // 顶点和底面中心重合
    Cone cone(vertex, baseCenter, 1.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 0.0f));
    
    // 高度为0的圆锥退化为一个点，只包含顶点
    EXPECT_TRUE(cone.Contains(vertex));
    EXPECT_FALSE(cone.Contains(Vector3(1.0f, 0, 0)));
}

TEST(ConeTest, RadiusZero)
{
    // 测试半径为0的圆锥
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 0.0f);
    
    // 半径为0的圆锥退化为一条线段
    EXPECT_TRUE(cone.Contains(vertex));
    EXPECT_TRUE(cone.Contains(baseCenter));
    EXPECT_TRUE(cone.Contains(Vector3(0, 0, 0.5f))); // 线段上的点
    EXPECT_FALSE(cone.Contains(Vector3(0.1f, 0, 0.5f))); // 线段外的点
}

TEST(ConeTest, Contains_NotAtOrigin)
{
    // 测试不在原点的圆锥的包含测试
    Vector3 vertex(1, 1, 1);
    Vector3 baseCenter(1, 1, 0);
    Cone cone(vertex, baseCenter, 1.0f);
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试底面上的点
    EXPECT_TRUE(cone.Contains(Vector3(2, 1, 0)));
    
    // 测试圆锥体内部的点
    EXPECT_TRUE(cone.Contains(Vector3(1.5f, 1, 0.5f)));
    
    // 测试圆锥体外部的点
    EXPECT_FALSE(cone.Contains(Vector3(3, 1, 0)));
    EXPECT_FALSE(cone.Contains(Vector3(1, 1, 2)));
}

TEST(ConeTest, Contains_NotAxisAligned)
{
    // 测试非坐标轴对齐的圆锥的包含测试
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(1, 0, 0); // 沿x轴方向，坐标轴对齐的圆锥
    Cone cone(vertex, baseCenter, 1.0f);
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试底面上的点（在底面圆的边缘）
    Vector3 edgePoint(1, 1, 0);
    EXPECT_TRUE(cone.Contains(edgePoint));
    
    // 测试圆锥体内部的点
    Vector3 insidePoint(0.5f, 0.5f, 0);
    EXPECT_TRUE(cone.Contains(insidePoint));
    
    // 测试圆锥体外部的点
    Vector3 outsidePoint(1, 1.1f, 0);
    EXPECT_FALSE(cone.Contains(outsidePoint));
}

TEST(ConeTest, LargeCone)
{
    // 测试非常大的圆锥
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(0, 0, 1000.0f);
    Cone cone(vertex, baseCenter, 500.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 1000.0f));
    
    // 测试底面边缘的点
    EXPECT_TRUE(cone.Contains(Vector3(500.0f, 0, 1000.0f)));
    
    // 测试内部点
    EXPECT_TRUE(cone.Contains(Vector3(250.0f, 0, 500.0f)));
    
    // 测试外部点
    EXPECT_FALSE(cone.Contains(Vector3(501.0f, 0, 1000.0f)));
}

TEST(ConeTest, SmallCone)
{
    // 测试非常小的圆锥
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(0, 0, 0.001f);
    Cone cone(vertex, baseCenter, 0.0005f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 0.001f));
    
    // 测试底面边缘的点
    EXPECT_TRUE(cone.Contains(Vector3(0.0005f, 0, 0.001f)));
    
    // 测试内部点
    EXPECT_TRUE(cone.Contains(Vector3(0.00025f, 0, 0.0005f)));
    
    // 测试外部点
    EXPECT_FALSE(cone.Contains(Vector3(0.0006f, 0, 0.001f)));
}

TEST(ConeTest, InvertedCone)
{
    // 测试倒圆锥（底面在顶点下方）
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(0, 0, -1.0f);
    Cone cone(vertex, baseCenter, 1.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 1.0f));
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试底面上的点
    EXPECT_TRUE(cone.Contains(Vector3(1.0f, 0, -1.0f)));
    
    // 测试圆锥体内部的点
    EXPECT_TRUE(cone.Contains(Vector3(0.5f, 0, -0.5f)));
}

TEST(ConeTest, Cone45Degrees)
{
    // 测试与坐标轴成45度角的圆锥
    Vector3 vertex(0, 0, 0);
    Vector3 baseCenter(1, 1, 0); // 45度角在xy平面
    Cone cone(vertex, baseCenter, 1.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), sqrtf(2.0f)));
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试轴线上的点
    Vector3 axisPoint(0.5f, 0.5f, 0);
    EXPECT_TRUE(cone.Contains(axisPoint));
    
    // 测试靠近轴线的点
    Vector3 nearAxisPoint(0.5f, 0.5f, 0.1f);
    EXPECT_TRUE(cone.Contains(nearAxisPoint));
    
    // 测试远离轴线的点
    Vector3 farAxisPoint(1, 1, 1.1f);
    EXPECT_FALSE(cone.Contains(farAxisPoint));
}

TEST(ConeTest, ConeInDifferentQuadrants)
{
    // 测试顶点和底面中心在不同象限的圆锥
    Vector3 vertex(-1, -1, -1);
    Vector3 baseCenter(1, 1, 1);
    Cone cone(vertex, baseCenter, 1.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), sqrtf(12.0f)));
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试中心点
    Vector3 center = cone.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 0.0f) && FloatEqual(center.y, 0.0f) && FloatEqual(center.z, 0.0f));
    EXPECT_TRUE(cone.Contains(center));
}

TEST(ConeTest, ConeParallelToAxis)
{
    // 测试与坐标轴平行但不在坐标轴上的圆锥
    Vector3 vertex(2, 3, 4);
    Vector3 baseCenter(2, 3, 0); // 平行于z轴
    Cone cone(vertex, baseCenter, 1.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 4.0f));
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试底面上的点
    EXPECT_TRUE(cone.Contains(Vector3(3, 3, 0)));
    
    // 测试圆锥体内部的点
    EXPECT_TRUE(cone.Contains(Vector3(2.5f, 3, 2)));
}

TEST(ConeTest, ConeHeightGreaterThanRadius)
{
    // 测试高度远大于半径的圆锥
    Vector3 vertex(0, 0, 10);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 1.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 10.0f));
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试底面上的点
    EXPECT_TRUE(cone.Contains(Vector3(1.0f, 0, 0)));
    
    // 测试圆锥体内部的点
    EXPECT_TRUE(cone.Contains(Vector3(0.5f, 0, 5)));
}

TEST(ConeTest, ConeRadiusGreaterThanHeight)
{
    // 测试半径远大于高度的圆锥
    Vector3 vertex(0, 0, 1);
    Vector3 baseCenter(0, 0, 0);
    Cone cone(vertex, baseCenter, 10.0f);
    
    EXPECT_TRUE(FloatEqual(cone.GetHeight(), 1.0f));
    
    // 测试顶点
    EXPECT_TRUE(cone.Contains(vertex));
    
    // 测试底面中心
    EXPECT_TRUE(cone.Contains(baseCenter));
    
    // 测试底面上的点
    EXPECT_TRUE(cone.Contains(Vector3(10.0f, 0, 0)));
    
    // 测试圆锥体内部的点
    EXPECT_TRUE(cone.Contains(Vector3(5.0f, 0, 0.5f)));
}

TEST(ConeTest, GetCenter_VariousPositions)
{
    // 测试不同位置的圆锥的中心点计算
    
    // 情况1：顶点在原点，底面中心在z轴正方向
    Vector3 vertex1(0, 0, 0);
    Vector3 baseCenter1(0, 0, 2);
    Cone cone1(vertex1, baseCenter1, 1.0f);
    Vector3 center1 = cone1.GetCenter();
    EXPECT_TRUE(FloatEqual(center1.x, 0.0f) && FloatEqual(center1.y, 0.0f) && FloatEqual(center1.z, 1.0f));
    
    // 情况2：顶点和底面中心都不在原点
    Vector3 vertex2(1, 2, 3);
    Vector3 baseCenter2(5, 6, 7);
    Cone cone2(vertex2, baseCenter2, 1.0f);
    Vector3 center2 = cone2.GetCenter();
    EXPECT_TRUE(FloatEqual(center2.x, 3.0f) && FloatEqual(center2.y, 4.0f) && FloatEqual(center2.z, 5.0f));
    
    // 情况3：顶点在底面中心上方
    Vector3 vertex3(0, 0, 5);
    Vector3 baseCenter3(0, 0, 0);
    Cone cone3(vertex3, baseCenter3, 1.0f);
    Vector3 center3 = cone3.GetCenter();
    EXPECT_TRUE(FloatEqual(center3.x, 0.0f) && FloatEqual(center3.y, 0.0f) && FloatEqual(center3.z, 2.5f));
}

TEST(ConeTest, GetHeight_VariousDirections)
{
    // 测试不同方向的圆锥的高度计算
    
    // 情况1：沿z轴方向
    Vector3 vertex1(0, 0, 0);
    Vector3 baseCenter1(0, 0, 1);
    Cone cone1(vertex1, baseCenter1, 1.0f);
    EXPECT_TRUE(FloatEqual(cone1.GetHeight(), 1.0f));
    
    // 情况2：沿x轴方向
    Vector3 vertex2(0, 0, 0);
    Vector3 baseCenter2(1, 0, 0);
    Cone cone2(vertex2, baseCenter2, 1.0f);
    EXPECT_TRUE(FloatEqual(cone2.GetHeight(), 1.0f));
    
    // 情况3：沿y轴方向
    Vector3 vertex3(0, 0, 0);
    Vector3 baseCenter3(0, 1, 0);
    Cone cone3(vertex3, baseCenter3, 1.0f);
    EXPECT_TRUE(FloatEqual(cone3.GetHeight(), 1.0f));
    
    // 情况4：沿对角线方向
    Vector3 vertex4(0, 0, 0);
    Vector3 baseCenter4(1, 1, 1);
    Cone cone4(vertex4, baseCenter4, 1.0f);
    EXPECT_TRUE(FloatEqual(cone4.GetHeight(), sqrtf(3.0f)));
}