#include <gtest/gtest.h>
#include "SympConv/Box.h"
#include "SympConv/BoxShape.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using fpnumber = SympConv::fpnumber;
using Box = SympConv::Box;
using BoxShape = SympConv::BoxShape;
using Vec3 = SympConv::Vector3;

TEST(BoxShapeTest, GetLocalSupport) {
    // 创建一个半长为(2, 1.5, 1)的盒子
    Vec3 halfExtents(2.0f, 1.5f, 1.0f);
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(FloatEqual(support_x_pos.x, 2.0f));
    EXPECT_TRUE(FloatEqual(support_x_pos.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_x_pos.z, 0.0f));
    
    // 测试负X方向
    Vec3 dir_x_neg(-1, 0, 0);
    Vec3 support_x_neg = shape.GetLocalSupport(dir_x_neg);
    EXPECT_TRUE(FloatEqual(support_x_neg.x, -2.0f));
    EXPECT_TRUE(FloatEqual(support_x_neg.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_x_neg.z, 0.0f));
    
    // 测试正Y方向
    Vec3 dir_y_pos(0, 1, 0);
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(FloatEqual(support_y_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_pos.y, 1.5f));
    EXPECT_TRUE(FloatEqual(support_y_pos.z, 0.0f));
    
    // 测试负Y方向
    Vec3 dir_y_neg(0, -1, 0);
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(FloatEqual(support_y_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_neg.y, -1.5f));
    EXPECT_TRUE(FloatEqual(support_y_neg.z, 0.0f));
    
    // 测试正Z方向
    Vec3 dir_z_pos(0, 0, 1);
    Vec3 support_z_pos = shape.GetLocalSupport(dir_z_pos);
    EXPECT_TRUE(FloatEqual(support_z_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_pos.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_pos.z, 1.0f));
    
    // 测试负Z方向
    Vec3 dir_z_neg(0, 0, -1);
    Vec3 support_z_neg = shape.GetLocalSupport(dir_z_neg);
    EXPECT_TRUE(FloatEqual(support_z_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_neg.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_neg.z, -1.0f));
    
    // 测试斜向方向
    Vec3 dir_diagonal(1, 1, 1);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    EXPECT_TRUE(FloatEqual(support_diagonal.x, 2.0f));
    EXPECT_TRUE(FloatEqual(support_diagonal.y, 1.5f));
    EXPECT_TRUE(FloatEqual(support_diagonal.z, 1.0f));
    
    // 测试负斜向方向
    Vec3 dir_neg_diagonal(-1, -1, -1);
    Vec3 support_neg_diagonal = shape.GetLocalSupport(dir_neg_diagonal);
    EXPECT_TRUE(FloatEqual(support_neg_diagonal.x, -2.0f));
    EXPECT_TRUE(FloatEqual(support_neg_diagonal.y, -1.5f));
    EXPECT_TRUE(FloatEqual(support_neg_diagonal.z, -1.0f));
    
    // 测试零方向向量
    Vec3 dir_zero(0, 0, 0);
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    EXPECT_TRUE(FloatEqual(support_zero.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_zero.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_zero.z, 0.0f));
}

TEST(BoxShapeTest, GetLocalSupport_MixedDirections) {
    // 创建一个半长为(3, 2, 1.5)的盒子
    Vec3 halfExtents(3.0f, 2.0f, 1.5f);
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 测试X正、Y负、Z正方向
    Vec3 dir_mixed1(1, -1, 1);
    Vec3 support_mixed1 = shape.GetLocalSupport(dir_mixed1);
    EXPECT_TRUE(FloatEqual(support_mixed1.x, 3.0f));
    EXPECT_TRUE(FloatEqual(support_mixed1.y, -2.0f));
    EXPECT_TRUE(FloatEqual(support_mixed1.z, 1.5f));
    
    // 测试X负、Y正、Z负方向
    Vec3 dir_mixed2(-1, 1, -1);
    Vec3 support_mixed2 = shape.GetLocalSupport(dir_mixed2);
    EXPECT_TRUE(FloatEqual(support_mixed2.x, -3.0f));
    EXPECT_TRUE(FloatEqual(support_mixed2.y, 2.0f));
    EXPECT_TRUE(FloatEqual(support_mixed2.z, -1.5f));
}

TEST(BoxShapeTest, GetLocalInertiaTensor) {
    // 创建一个半长为(1, 1, 1)的盒子（边长为2x2x2）
    Vec3 halfExtents(1.0f, 1.0f, 1.0f);
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 测试质量为12的情况，此时惯性张量应该为(8, 8, 8)
    // Ix = (1/12) * 12 * (2² + 2²) = 8
    fpnumber mass = 12.0f;
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    EXPECT_TRUE(FloatEqual(inertia.x, 8.0f));
    EXPECT_TRUE(FloatEqual(inertia.y, 8.0f));
    EXPECT_TRUE(FloatEqual(inertia.z, 8.0f));
    
    // 测试质量为6的情况，此时惯性张量应该为(4, 4, 4)
    // Ix = (1/12) * 6 * (2² + 2²) = 4
    mass = 6.0f;
    inertia = shape.GetLocalInertiaTensor(mass);
    EXPECT_TRUE(FloatEqual(inertia.x, 4.0f));
    EXPECT_TRUE(FloatEqual(inertia.y, 4.0f));
    EXPECT_TRUE(FloatEqual(inertia.z, 4.0f));
}

TEST(BoxShapeTest, GetLocalInertiaTensor_DifferentSize) {
    // 创建一个半长为(2, 3, 4)的盒子（边长为4x6x8）
    Vec3 halfExtents(2.0f, 3.0f, 4.0f);
    Box box(halfExtents);
    BoxShape shape(box);
    
    // 计算期望的惯性张量
    // Ix = (1/12) * m * (height² + depth²) = (1/12) * 12 * (6² + 8²) = (36 + 64) = 100
    // Iy = (1/12) * m * (width² + depth²) = (1/12) * 12 * (4² + 8²) = (16 + 64) = 80
    // Iz = (1/12) * m * (width² + height²) = (1/12) * 12 * (4² + 6²) = (16 + 36) = 52
    fpnumber mass = 12.0f;
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    EXPECT_TRUE(FloatEqual(inertia.x, 100.0f));
    EXPECT_TRUE(FloatEqual(inertia.y, 80.0f));
    EXPECT_TRUE(FloatEqual(inertia.z, 52.0f));
}

} // namespace SympConvTest
