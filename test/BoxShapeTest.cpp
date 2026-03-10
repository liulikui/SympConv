#include "gtest/gtest.h"
#include "SympConv/BoxShape.h"

using namespace SympConv;

TEST(BoxShapeTest, GetSupport) {
    // 创建一个中心在原点，半边长为1的立方体
    TBox<float> box(Vector3f(0, 0, 0), Vector3f(1, 1, 1), TTransform<float>());
    TBoxShape<float> boxShape(box);

    // 测试正x方向的支持点
    Vector3 support = boxShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正y方向的支持点
    support = boxShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试正z方向的支持点
    support = boxShape.GetSupport(Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 1.0f);

    // 测试负x方向的支持点
    support = boxShape.GetSupport(Vector3(-1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, -1.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负y方向的支持点
    support = boxShape.GetSupport(Vector3(0, -1, 0));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, -1.0f);
    EXPECT_FLOAT_EQ(support.z, 0.0f);

    // 测试负z方向的支持点
    support = boxShape.GetSupport(Vector3(0, 0, -1));
    EXPECT_FLOAT_EQ(support.x, 0.0f);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, -1.0f);

    // 测试对角线方向的支持点
    support = boxShape.GetSupport(Vector3(1, 1, 1));
    EXPECT_FLOAT_EQ(support.x, 1.0f);
    EXPECT_FLOAT_EQ(support.y, 1.0f);
    EXPECT_FLOAT_EQ(support.z, 1.0f);
}

TEST(BoxShapeTest, GetSupport_NotAtOrigin) {
    // 创建一个中心在(2, 3, 4)，半边长为1的立方体
    TBox<float> box(Vector3f(2, 3, 4), Vector3f(1, 1, 1), TTransform<float>());
    TBoxShape<float> boxShape(box);

    // 测试正x方向的支持点
    Vector3 support = boxShape.GetSupport(Vector3(1, 0, 0));
    EXPECT_FLOAT_EQ(support.x, 3.0f);
    EXPECT_FLOAT_EQ(support.y, 3.0f);
    EXPECT_FLOAT_EQ(support.z, 4.0f);

    // 测试正y方向的支持点
    support = boxShape.GetSupport(Vector3(0, 1, 0));
    EXPECT_FLOAT_EQ(support.x, 2.0f);
    EXPECT_FLOAT_EQ(support.y, 4.0f);
    EXPECT_FLOAT_EQ(support.z, 4.0f);

    // 测试正z方向的支持点
    support = boxShape.GetSupport(Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(support.x, 2.0f);
    EXPECT_FLOAT_EQ(support.y, 3.0f);
    EXPECT_FLOAT_EQ(support.z, 5.0f);
}

TEST(BoxShapeTest, GetSupport_Rotated) {
    // 创建一个中心在原点，半边长为1，绕y轴旋转45度的立方体
    TTransform<float> transform;
    transform.mOrientation = TQuaternion<float>::FromEulerAngles(0, 45, 0);
    TBox<float> box(Vector3f(0, 0, 0), Vector3f(1, 1, 1), transform);
    TBoxShape<float> boxShape(box);

    // 测试正x方向的支持点（旋转后）
    Vector3 support = boxShape.GetSupport(Vector3(1, 0, 0));
    // 旋转45度后，x方向的支持点应该在(√2/2, 0, √2/2)方向
    float expected = 1.0f / std::sqrt(2.0f);
    EXPECT_FLOAT_EQ(support.x, expected);
    EXPECT_FLOAT_EQ(support.y, 0.0f);
    EXPECT_FLOAT_EQ(support.z, expected);
}
