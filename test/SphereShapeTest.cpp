#include "gtest/gtest.h"
#include "SympConv/SphereShape.h"

using namespace SympConv;

TEST(SphereShapeTest, GetSupport) {
    // 创建一个中心在原点，半径为1的球体
    TSphere<float> sphere(Vector3f(0, 0, 0), 1.0f);
    TSphereShape<float> sphereShape(sphere);

    // 测试正x方向的支持点
    Vector3 support = sphereShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正y方向的支持点
    support = sphereShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正z方向的支持点
    support = sphereShape.GetSupport(Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 1.0f);

    // 测试负x方向的支持点
    support = sphereShape.GetSupport(Vector3(-1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, -1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负y方向的支持点
    support = sphereShape.GetSupport(Vector3(0, -1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, -1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负z方向的支持点
    support = sphereShape.GetSupport(Vector3(0, 0, -1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, -1.0f);

    // 测试对角线方向的支持点
    support = sphereShape.GetSupport(Vector3(1, 1, 1));
    float expected = 1.0f / std::sqrt(3.0f);
    EXPECT_FLOAT_EQ(support.x, expected);
    EXPECT_FLOAT_EQ(support.y, expected);
    EXPECT_FLOAT_EQ(support.z, expected);
}

TEST(SphereShapeTest, GetSupport_NotAtOrigin) {
    // 创建一个中心在(2, 3, 4)，半径为1的球体
    TSphere<float> sphere(Vector3f(2, 3, 4), 1.0f);
    TSphereShape<float> sphereShape(sphere);

    // 测试正x方向的支持点
    Vector3 support = sphereShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 3.0f);
    EXPECT_FLOAT_EQ(support.y, 3.0f);
    EXPECT_FLOAT_EQ(support.z, 4.0f);

    // 测试正y方向的支持点
    support = sphereShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 2.0f);
    EXPECT_FLOAT_EQ(support.y, 4.0f);
    EXPECT_FLOAT_EQ(support.z, 4.0f);

    // 测试正z方向的支持点
    support = sphereShape.GetSupport(Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(support.x, 2.0f);
    EXPECT_FLOAT_EQ(support.y, 3.0f);
    EXPECT_FLOAT_EQ(support.z, 5.0f);

    // 测试对角线方向的支持点
    support = sphereShape.GetSupport(Vector3(1, 1, 1));
    float expected = 1.0f / std::sqrt(3.0f);
    EXPECT_FLOAT_EQ(support.x, 2.0f + expected);
    EXPECT_FLOAT_EQ(support.y, 3.0f + expected);
    EXPECT_FLOAT_EQ(support.z, 4.0f + expected);
}
