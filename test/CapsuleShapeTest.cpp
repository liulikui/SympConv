#include "gtest/gtest.h"
#include "SympConv/CapsuleShape.h"

using namespace SympConv;

TEST(CapsuleShapeTest, GetSupport) {
    // 创建一个中心在原点，长度为2，半径为1，沿y轴方向的胶囊体
    TCapsule<float> capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 1.0f);
    TCapsuleShape<float> capsuleShape(capsule);

    // 测试正x方向的支持点
    Vector3 support = capsuleShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正y方向的支持点
    support = capsuleShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 2.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正z方向的支持点
    support = capsuleShape.GetSupport(Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 1.0f);

    // 测试负x方向的支持点
    support = capsuleShape.GetSupport(Vector3(-1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, -1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负y方向的支持点
    support = capsuleShape.GetSupport(Vector3(0, -1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, -2.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负z方向的支持点
    support = capsuleShape.GetSupport(Vector3(0, 0, -1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, -1.0f);

    // 测试斜方向的支持点
    support = capsuleShape.GetSupport(Vector3(1, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);
}
