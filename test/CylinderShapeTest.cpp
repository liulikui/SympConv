#include "gtest/gtest.h"
#include "SympConv/CylinderShape.h"

using namespace SympConv;

TEST(CylinderShapeTest, GetSupport) {
    // 创建一个中心在原点，长度为2，半径为1，沿y轴方向的圆柱体
    TCylinder<float> cylinder(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 1.0f);
    TCylinderShape<float> cylinderShape(cylinder);

    // 测试正x方向的支持点
    Vector3 support = cylinderShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正y方向的支持点
    support = cylinderShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正z方向的支持点
    support = cylinderShape.GetSupport(Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 1.0f);

    // 测试负x方向的支持点
    support = cylinderShape.GetSupport(Vector3(-1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, -1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负y方向的支持点
    support = cylinderShape.GetSupport(Vector3(0, -1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, -1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负z方向的支持点
    support = cylinderShape.GetSupport(Vector3(0, 0, -1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, -1.0f);

    // 测试斜方向的支持点
    support = cylinderShape.GetSupport(Vector3(1, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);
}

TEST(CylinderShapeTest, GetSupport_NotAtOrigin) {
    // 创建一个中心在(2, 3, 4)，长度为2，半径为1，沿y轴方向的圆柱体
    TCylinder<float> cylinder(Vector3f(2, 2, 4), Vector3f(2, 4, 4), 1.0f);
    TCylinderShape<float> cylinderShape(cylinder);

    // 测试正x方向的支持点
    Vector3 support = cylinderShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 3.0f);
    EXPECT_FLOAT_EQ(support.y, 3.0f);
    EXPECT_FLOAT_EQ(support.z, 4.0f);

    // 测试正y方向的支持点
    support = cylinderShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 2.0f);
    EXPECT_FLOAT_EQ(support.y, 4.0f);
    EXPECT_FLOAT_EQ(support.z, 4.0f);

    // 测试正z方向的支持点
    support = cylinderShape.GetSupport(Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(support.x, 2.0f);
    EXPECT_FLOAT_EQ(support.y, 3.0f);
    EXPECT_FLOAT_EQ(support.z, 5.0f);
}

TEST(CylinderShapeTest, GetSupport_Rotated) {
    // 创建一个中心在原点，长度为2，半径为1，沿x轴方向的圆柱体（旋转90度）
    TCylinder<float> cylinder(Vector3f(-1, 0, 0), Vector3f(1, 0, 0), 1.0f);
    TCylinderShape<float> cylinderShape(cylinder);

    // 测试正x方向的支持点
    Vector3 support = cylinderShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正y方向的支持点
    support = cylinderShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);
}
