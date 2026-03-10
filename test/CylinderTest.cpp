#include <gtest/gtest.h>
#include "SympConv/Cylinder.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "TestUtils.h"

namespace SympConvTest {

using Vec3 = SympConv::Vector3;
using Cylinder = SympConv::Cylinder;

TEST(CylinderTest, DefaultConstructor) {
    Cylinder cylinder;
    
    // 检查默认值
    EXPECT_TRUE(FloatEqual(cylinder.mStart.x, 0.0f));
    EXPECT_TRUE(FloatEqual(cylinder.mStart.y, 0.0f));
    EXPECT_TRUE(FloatEqual(cylinder.mStart.z, 0.0f));
    
    EXPECT_TRUE(FloatEqual(cylinder.mEnd.x, 0.0f));
    EXPECT_TRUE(FloatEqual(cylinder.mEnd.y, 0.0f));
    EXPECT_TRUE(FloatEqual(cylinder.mEnd.z, 1.0f));
    
    EXPECT_TRUE(FloatEqual(cylinder.mRadius, 0.5f));
}

TEST(CylinderTest, ParameterizedConstructor) {
    Vec3 start(1.0f, 2.0f, 3.0f);
    Vec3 end(4.0f, 5.0f, 6.0f);
    float radius = 1.5f;
    
    Cylinder cylinder(start, end, radius);
    
    // 检查参数是否正确设置
    EXPECT_TRUE(FloatEqual(cylinder.mStart.x, start.x));
    EXPECT_TRUE(FloatEqual(cylinder.mStart.y, start.y));
    EXPECT_TRUE(FloatEqual(cylinder.mStart.z, start.z));
    
    EXPECT_TRUE(FloatEqual(cylinder.mEnd.x, end.x));
    EXPECT_TRUE(FloatEqual(cylinder.mEnd.y, end.y));
    EXPECT_TRUE(FloatEqual(cylinder.mEnd.z, end.z));
    
    EXPECT_TRUE(FloatEqual(cylinder.mRadius, radius));
}

TEST(CylinderTest, GetCenter) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(2.0f, 2.0f, 2.0f);
    Cylinder cylinder(start, end, 1.0f);
    
    Vec3 center = cylinder.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 1.0f));
    EXPECT_TRUE(FloatEqual(center.y, 1.0f));
    EXPECT_TRUE(FloatEqual(center.z, 1.0f));
}

TEST(CylinderTest, GetLength) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 3.0f);
    Cylinder cylinder(start, end, 1.0f);
    
    float length = cylinder.GetLength();
    EXPECT_TRUE(FloatEqual(length, 3.0f));
}

TEST(CylinderTest, GetDirection) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 1.0f);
    Cylinder cylinder(start, end, 1.0f);
    
    Vec3 direction = cylinder.GetDirection();
    EXPECT_TRUE(FloatEqual(direction.x, 0.0f));
    EXPECT_TRUE(FloatEqual(direction.y, 0.0f));
    EXPECT_TRUE(FloatEqual(direction.z, 1.0f));
}

TEST(CylinderTest, GetAABB) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Cylinder cylinder(start, end, radius);
    
    auto aabb = cylinder.GetAABB();
    
    // 检查AABB的最小和最大坐标
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, -1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.y, -1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.z, -1.0f));
    
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.y, 1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.z, 3.0f));
}

TEST(CylinderTest, Contains) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Cylinder cylinder(start, end, radius);
    
    // 测试圆柱体内的点
    Vec3 insidePoint(0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(cylinder.Contains(insidePoint));
    
    // 测试圆柱体表面的点
    Vec3 surfacePoint(1.0f, 0.0f, 1.0f);
    EXPECT_TRUE(cylinder.Contains(surfacePoint));
    
    // 测试圆柱体端点的点
    Vec3 endpoint(0.0f, 0.0f, 0.0f);
    EXPECT_TRUE(cylinder.Contains(endpoint));
    
    // 测试圆柱体外的点
    Vec3 outsidePoint(1.1f, 0.0f, 1.0f);
    EXPECT_FALSE(cylinder.Contains(outsidePoint));
    
    // 测试圆柱体轴线外的点
    Vec3 axisOutsidePoint(0.0f, 0.0f, 3.0f);
    EXPECT_FALSE(cylinder.Contains(axisOutsidePoint));
}

TEST(CylinderTest, Contains_BoundaryCases) {
    // 测试几乎为点的圆柱体
    Vec3 start(1.0f, 1.0f, 1.0f);
    Vec3 end(1.0f, 1.0f, 1.0f); // 起点和终点相同
    float radius = 0.5f;
    Cylinder cylinder(start, end, radius);
    
    // 测试端点本身
    EXPECT_TRUE(cylinder.Contains(start));
    
    // 测试半径边界上的点
    Vec3 boundaryPoint(1.5f, 1.0f, 1.0f);
    EXPECT_TRUE(cylinder.Contains(boundaryPoint));
    
    // 测试半径外的点
    Vec3 outsidePoint(1.6f, 1.0f, 1.0f);
    EXPECT_FALSE(cylinder.Contains(outsidePoint));
}

TEST(CylinderTest, GetAABB_Rotated) {
    // 测试旋转的圆柱体的AABB
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(2.0f, 2.0f, 2.0f);
    float radius = 1.0f;
    Cylinder cylinder(start, end, radius);
    
    auto aabb = cylinder.GetAABB();
    
    // 检查AABB的最小和最大坐标
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, -1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.y, -1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.z, -1.0f));
    
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 3.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.y, 3.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.z, 3.0f));
}

TEST(CylinderTest, GetDirection_Boundary) {
    // 测试长度为0的圆柱体的方向向量
    Vec3 start(1.0f, 1.0f, 1.0f);
    Vec3 end(1.0f, 1.0f, 1.0f); // 起点和终点相同
    Cylinder cylinder(start, end, 1.0f);
    
    Vec3 direction = cylinder.GetDirection();
    // 长度为0的向量归一化后应该是零向量
    EXPECT_TRUE(FloatEqual(direction.x, 0.0f));
    EXPECT_TRUE(FloatEqual(direction.y, 0.0f));
    EXPECT_TRUE(FloatEqual(direction.z, 0.0f));
}

TEST(CylinderTest, RadiusZero) {
    // 测试半径为0的圆柱体
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    Cylinder cylinder(start, end, 0.0f);
    
    // 只有轴线上的点应该被包含
    EXPECT_TRUE(cylinder.Contains(start));
    EXPECT_TRUE(cylinder.Contains(end));
    EXPECT_TRUE(cylinder.Contains(Vec3(0.0f, 0.0f, 1.0f)));
    
    // 偏离轴线的点不应该被包含
    EXPECT_FALSE(cylinder.Contains(Vec3(0.1f, 0.0f, 1.0f)));
}

TEST(CylinderTest, DifferentDirections) {
    // 测试不同方向的圆柱体
    
    // X轴方向
    Cylinder cylinderX(Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_TRUE(FloatEqual(cylinderX.GetDirection().x, 1.0f));
    EXPECT_TRUE(FloatEqual(cylinderX.GetDirection().y, 0.0f));
    EXPECT_TRUE(FloatEqual(cylinderX.GetDirection().z, 0.0f));
    
    // Y轴方向
    Cylinder cylinderY(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 2.0f, 0.0f), 1.0f);
    EXPECT_TRUE(FloatEqual(cylinderY.GetDirection().x, 0.0f));
    EXPECT_TRUE(FloatEqual(cylinderY.GetDirection().y, 1.0f));
    EXPECT_TRUE(FloatEqual(cylinderY.GetDirection().z, 0.0f));
    
    // 斜方向
    Cylinder cylinderDiag(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), 1.0f);
    Vec3 dirDiag = cylinderDiag.GetDirection();
    float expectedDir = 1.0f / sqrtf(3.0f);
    EXPECT_TRUE(FloatEqual(dirDiag.x, expectedDir));
    EXPECT_TRUE(FloatEqual(dirDiag.y, expectedDir));
    EXPECT_TRUE(FloatEqual(dirDiag.z, expectedDir));
}

} // namespace SympConvTest
