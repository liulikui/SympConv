#include <gtest/gtest.h>
#include "SympConv/Cone.h"
#include "SympConv/ConeShape.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using fpnumber = SympConv::fpnumber;
using Cone = SympConv::Cone;
using ConeShape = SympConv::ConeShape;
using Vec3 = SympConv::Vector3;

TEST(ConeShapeTest, GetLocalSupport) {
    // 创建一个高度为2，半径为1的圆锥体
    Cone cone(2.0f, 1.0f);
    ConeShape shape(cone);
    
    // 测试正Y方向（顶点方向）
    Vec3 dir_y_pos(0, 1, 0);
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(FloatEqual(support_y_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_pos.y, 2.0f)); // 顶点
    EXPECT_TRUE(FloatEqual(support_y_pos.z, 0.0f));
    
    // 测试负Y方向（底面方向）
    Vec3 dir_y_neg(0, -1, 0);
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(FloatEqual(support_y_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_neg.y, 0.0f)); // 底面中心
    EXPECT_TRUE(FloatEqual(support_y_neg.z, 0.0f));
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(FloatEqual(support_x_pos.x, 1.0f)); // 底面边缘
    EXPECT_TRUE(FloatEqual(support_x_pos.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_x_pos.z, 0.0f));
    
    // 测试正Z方向
    Vec3 dir_z_pos(0, 0, 1);
    Vec3 support_z_pos = shape.GetLocalSupport(dir_z_pos);
    EXPECT_TRUE(FloatEqual(support_z_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_pos.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_pos.z, 1.0f)); // 底面边缘
    
    // 测试负X方向
    Vec3 dir_x_neg(-1, 0, 0);
    Vec3 support_x_neg = shape.GetLocalSupport(dir_x_neg);
    EXPECT_TRUE(FloatEqual(support_x_neg.x, -1.0f)); // 底面边缘
    EXPECT_TRUE(FloatEqual(support_x_neg.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_x_neg.z, 0.0f));
    
    // 测试负Z方向
    Vec3 dir_z_neg(0, 0, -1);
    Vec3 support_z_neg = shape.GetLocalSupport(dir_z_neg);
    EXPECT_TRUE(FloatEqual(support_z_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_neg.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_neg.z, -1.0f)); // 底面边缘
    
    // 测试斜向方向（X正，Y正，Z正）
    Vec3 dir_diagonal(1, 1, 1);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    EXPECT_TRUE(FloatEqual(support_diagonal.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_diagonal.y, 2.0f)); // 顶点
    EXPECT_TRUE(FloatEqual(support_diagonal.z, 0.0f));
    
    // 测试斜向方向（X正，Y负，Z正）
    Vec3 dir_diagonal_neg_y(1, -1, 1);
    Vec3 support_diagonal_neg_y = shape.GetLocalSupport(dir_diagonal_neg_y);
    // X和Z分量应该是归一化后的方向乘以半径
    fpnumber expected_xz = 1.0f / sqrt(2.0f);
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.x, expected_xz));
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.z, expected_xz));
    
    // 测试零方向向量
    Vec3 dir_zero(0, 0, 0);
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    // 零方向向量默认返回底面中心
    EXPECT_TRUE(FloatEqual(support_zero.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_zero.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_zero.z, 0.0f));
}

TEST(ConeShapeTest, GetLocalSupport_DifferentSize) {
    // 创建一个高度为4，半径为2的圆锥体
    Cone cone(4.0f, 2.0f);
    ConeShape shape(cone);
    
    // 测试正Y方向（顶点方向）
    Vec3 dir_y_pos(0, 1, 0);
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(FloatEqual(support_y_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_pos.y, 4.0f)); // 顶点
    EXPECT_TRUE(FloatEqual(support_y_pos.z, 0.0f));
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(FloatEqual(support_x_pos.x, 2.0f)); // 底面边缘
    EXPECT_TRUE(FloatEqual(support_x_pos.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_x_pos.z, 0.0f));
    
    // 测试斜向方向（X正，Y负，Z正）
    Vec3 dir_diagonal_neg_y(1, -1, 1);
    Vec3 support_diagonal_neg_y = shape.GetLocalSupport(dir_diagonal_neg_y);
    // X和Z分量应该是归一化后的方向乘以半径
    fpnumber expected_xz = 2.0f / sqrt(2.0f);
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.x, expected_xz));
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.z, expected_xz));
}

} // namespace SympConvTest
