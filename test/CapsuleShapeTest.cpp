#include <gtest/gtest.h>
#include "SympConv/Capsule.h"
#include "SympConv/CapsuleShape.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using fpnumber = SympConv::fpnumber;
using Capsule = SympConv::Capsule;
using CapsuleShape = SympConv::CapsuleShape;
using Vec3 = SympConv::Vector3;

TEST(CapsuleShapeTest, GetLocalSupport) {
    // 创建一个高度为2，半径为1的胶囊体
    Capsule capsule(2.0f, 1.0f);
    CapsuleShape shape(capsule);
    
    // 测试正Y方向
    Vec3 dir_up(0, 1, 0);
    Vec3 support_up = shape.GetLocalSupport(dir_up);
    EXPECT_EQ(support_up.x, 0.0f);
    EXPECT_EQ(support_up.y, 1.0f + 1.0f); // 高度的一半加上半径
    EXPECT_EQ(support_up.z, 0.0f);
    
    // 测试负Y方向
    Vec3 dir_down(0, -1, 0);
    Vec3 support_down = shape.GetLocalSupport(dir_down);
    EXPECT_EQ(support_down.x, 0.0f);
    EXPECT_EQ(support_down.y, -1.0f - 1.0f); // 负高度的一半减去半径
    EXPECT_EQ(support_down.z, 0.0f);
    
    // 测试正X方向
    Vec3 dir_right(1, 0, 0);
    Vec3 support_right = shape.GetLocalSupport(dir_right);
    EXPECT_EQ(support_right.x, 1.0f); // 半径
    EXPECT_EQ(support_right.y, 1.0f); // 上顶点
    EXPECT_EQ(support_right.z, 0.0f);
    
    // 测试正Z方向
    Vec3 dir_forward(0, 0, 1);
    Vec3 support_forward = shape.GetLocalSupport(dir_forward);
    EXPECT_EQ(support_forward.x, 0.0f);
    EXPECT_EQ(support_forward.y, 1.0f); // 上顶点
    EXPECT_EQ(support_forward.z, 1.0f); // 半径
    
    // 测试斜向方向
    Vec3 dir_diagonal(1, 1, 0);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    EXPECT_EQ(support_diagonal.x, 1.0f); // 半径
    EXPECT_EQ(support_diagonal.y, 1.0f); // 上顶点
    EXPECT_EQ(support_diagonal.z, 0.0f);
    
    // 测试负X方向
    Vec3 dir_left(-1, 0, 0);
    Vec3 support_left = shape.GetLocalSupport(dir_left);
    EXPECT_EQ(support_left.x, -1.0f); // 半径
    EXPECT_EQ(support_left.y, 1.0f); // 上顶点
    EXPECT_EQ(support_left.z, 0.0f);
    
    // 测试负Z方向
    Vec3 dir_backward(0, 0, -1);
    Vec3 support_backward = shape.GetLocalSupport(dir_backward);
    EXPECT_EQ(support_backward.x, 0.0f);
    EXPECT_EQ(support_backward.y, 1.0f); // 上顶点
    EXPECT_EQ(support_backward.z, -1.0f); // 半径
}

TEST(CapsuleShapeTest, GetLocalSupport_ZeroDirection) {
    // 创建一个高度为2，半径为1的胶囊体
    Capsule capsule(2.0f, 1.0f);
    CapsuleShape shape(capsule);
    
    // 测试零方向向量（应该返回上顶点）
    Vec3 dir_zero(0, 0, 0);
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    EXPECT_EQ(support_zero.x, 0.0f);
    EXPECT_EQ(support_zero.y, 1.0f + 1.0f); // 上顶点加上半径
    EXPECT_EQ(support_zero.z, 0.0f);
}

TEST(CapsuleShapeTest, GetLocalSupport_SmallHeight) {
    // 创建一个高度很小的胶囊体（接近球体）
    Capsule capsule(0.1f, 1.0f);
    CapsuleShape shape(capsule);
    
    // 测试正X方向
    Vec3 dir_right(1, 0, 0);
    Vec3 support_right = shape.GetLocalSupport(dir_right);
    EXPECT_NEAR(support_right.x, 1.0f, 1e-6); // 半径
    EXPECT_NEAR(support_right.y, 0.05f, 1e-6); // 上顶点
    EXPECT_NEAR(support_right.z, 0.0f, 1e-6);
    
    // 测试负Y方向
    Vec3 dir_down(0, -1, 0);
    Vec3 support_down = shape.GetLocalSupport(dir_down);
    EXPECT_NEAR(support_down.x, 0.0f, 1e-6);
    EXPECT_NEAR(support_down.y, -0.05f - 1.0f, 1e-6); // 下顶点减去半径
    EXPECT_NEAR(support_down.z, 0.0f, 1e-6);
}

TEST(CapsuleShapeTest, GetLocalInertiaTensor) {
    // 创建一个高度为2，半径为1的胶囊体
    Capsule capsule(2.0f, 1.0f);
    CapsuleShape shape(capsule);
    
    // 测试质量为1的情况
    fpnumber mass = 1.0f;
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于沿Y轴的胶囊体，Ix和Iz应该相等
    EXPECT_TRUE(FloatEqual(inertia.x, inertia.z));
}

TEST(CapsuleShapeTest, GetLocalInertiaTensor_DifferentSize) {
    // 创建一个高度为4，半径为2的胶囊体
    Capsule capsule(4.0f, 2.0f);
    CapsuleShape shape(capsule);
    
    // 测试质量为2的情况
    fpnumber mass = 2.0f;
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于沿Y轴的胶囊体，Ix和Iz应该相等
    EXPECT_TRUE(FloatEqual(inertia.x, inertia.z));
}

} // namespace SympConvTest
