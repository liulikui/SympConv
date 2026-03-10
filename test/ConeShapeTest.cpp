#include "gtest/gtest.h"
#include "SympConv/ConeShape.h"
#include "TestUtils.h"

using namespace SympConv;
using namespace SympConvTest;

TEST(ConeShapeTest, GetSupport)
{
    // 创建一个圆锥体，顶点在(0, 0, 1)，底面中心在(0, 0, 0)，半径为1
    Cone cone(Vector3(0, 0, 1), Vector3(0, 0, 0), 1.0f);
    ConeShape shape(cone);
    
    // 测试不同方向的支持点
    
    // 方向向上（指向顶点）
    Vector3 support = shape.GetSupport(Vector3(0, 0, 1));
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, 0) && FloatEqual(support.z, 1));
    
    // 方向向下（指向底面）
    support = shape.GetSupport(Vector3(0, 0, -1));
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, 0) && FloatEqual(support.z, 0));
    
    // 方向沿x轴正方向
    support = shape.GetSupport(Vector3(1, 0, 0));
    EXPECT_TRUE(FloatEqual(support.x, 1) && FloatEqual(support.y, 0) && FloatEqual(support.z, 0));
    
    // 方向沿y轴正方向
    support = shape.GetSupport(Vector3(0, 1, 0));
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, 1) && FloatEqual(support.z, 0));
    
    // 方向沿x轴负方向
    support = shape.GetSupport(Vector3(-1, 0, 0));
    EXPECT_TRUE(FloatEqual(support.x, -1) && FloatEqual(support.y, 0) && FloatEqual(support.z, 0));
    
    // 方向沿y轴负方向
    support = shape.GetSupport(Vector3(0, -1, 0));
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, -1) && FloatEqual(support.z, 0));
    
    // 方向斜向上
    support = shape.GetSupport(Vector3(1, 0, 1));
    // 对于斜向上的方向，支持点应该是顶点
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, 0) && FloatEqual(support.z, 1));
    
    // 方向斜向下
    support = shape.GetSupport(Vector3(1, 0, -1));
    // 对于斜向下的方向，支持点应该在底面边缘
    EXPECT_TRUE(FloatEqual(support.x, 1) && FloatEqual(support.y, 0) && FloatEqual(support.z, 0));
}

TEST(ConeShapeTest, GetSupport_NotAtOrigin)
{
    // 创建一个圆锥体，顶点在(2, 3, 5)，底面中心在(2, 3, 4)，半径为1
    Cone cone(Vector3(2, 3, 5), Vector3(2, 3, 4), 1.0f);
    ConeShape shape(cone);
    
    // 测试不同方向的支持点
    
    // 方向向上（指向顶点）
    Vector3 support = shape.GetSupport(Vector3(0, 0, 1));
    EXPECT_TRUE(FloatEqual(support.x, 2) && FloatEqual(support.y, 3) && FloatEqual(support.z, 5));
    
    // 方向向下（指向底面）
    support = shape.GetSupport(Vector3(0, 0, -1));
    EXPECT_TRUE(FloatEqual(support.x, 2) && FloatEqual(support.y, 3) && FloatEqual(support.z, 4));
    
    // 方向沿x轴正方向
    support = shape.GetSupport(Vector3(1, 0, 0));
    EXPECT_TRUE(FloatEqual(support.x, 3) && FloatEqual(support.y, 3) && FloatEqual(support.z, 4));
    
    // 方向沿y轴正方向
    support = shape.GetSupport(Vector3(0, 1, 0));
    EXPECT_TRUE(FloatEqual(support.x, 2) && FloatEqual(support.y, 4) && FloatEqual(support.z, 4));
}

TEST(ConeShapeTest, GetSupport_Rotated)
{
    // 创建一个圆锥体，顶点在(1, 0, 0)，底面中心在(0, 0, 0)，半径为1（沿x轴方向）
    Cone cone(Vector3(1, 0, 0), Vector3(0, 0, 0), 1.0f);
    ConeShape shape(cone);
    
    // 测试不同方向的支持点
    
    // 方向沿x轴正方向（指向顶点）
    Vector3 support = shape.GetSupport(Vector3(1, 0, 0));
    EXPECT_TRUE(FloatEqual(support.x, 1) && FloatEqual(support.y, 0) && FloatEqual(support.z, 0));
    
    // 方向沿x轴负方向（指向底面）
    support = shape.GetSupport(Vector3(-1, 0, 0));
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, 0) && FloatEqual(support.z, 0));
    
    // 方向沿y轴正方向
    support = shape.GetSupport(Vector3(0, 1, 0));
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, 1) && FloatEqual(support.z, 0));
    
    // 方向沿z轴正方向
    support = shape.GetSupport(Vector3(0, 0, 1));
    EXPECT_TRUE(FloatEqual(support.x, 0) && FloatEqual(support.y, 0) && FloatEqual(support.z, 1));
}