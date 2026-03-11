#include <gtest/gtest.h>
#include "SympConv/Cylinder.h"
#include "SympConv/CylinderShape.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using fpnumber = SympConv::fpnumber;
using Cylinder = SympConv::Cylinder;
using CylinderShape = SympConv::CylinderShape;
using Vec3 = SympConv::Vector3;

TEST(CylinderShapeTest, GetLocalSupport) {
    // 创建一个高度为2，半径为1的圆柱体
    Cylinder cylinder(2.0f, 1.0f);
    CylinderShape shape(cylinder);
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(FloatEqual(support_x_pos.x, 1.0f));
    EXPECT_TRUE(FloatEqual(support_x_pos.y, 1.0f)); // 上顶面
    EXPECT_TRUE(FloatEqual(support_x_pos.z, 0.0f));
    
    // 测试负X方向
    Vec3 dir_x_neg(-1, 0, 0);
    Vec3 support_x_neg = shape.GetLocalSupport(dir_x_neg);
    EXPECT_TRUE(FloatEqual(support_x_neg.x, -1.0f));
    EXPECT_TRUE(FloatEqual(support_x_neg.y, 1.0f)); // 上顶面
    EXPECT_TRUE(FloatEqual(support_x_neg.z, 0.0f));
    
    // 测试正Y方向
    Vec3 dir_y_pos(0, 1, 0);
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(FloatEqual(support_y_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_pos.y, 1.0f)); // 上顶面中心
    EXPECT_TRUE(FloatEqual(support_y_pos.z, 0.0f));
    
    // 测试负Y方向
    Vec3 dir_y_neg(0, -1, 0);
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(FloatEqual(support_y_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_neg.y, -1.0f)); // 下底面中心
    EXPECT_TRUE(FloatEqual(support_y_neg.z, 0.0f));
    
    // 测试正Z方向
    Vec3 dir_z_pos(0, 0, 1);
    Vec3 support_z_pos = shape.GetLocalSupport(dir_z_pos);
    EXPECT_TRUE(FloatEqual(support_z_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_pos.y, 1.0f)); // 上顶面
    EXPECT_TRUE(FloatEqual(support_z_pos.z, 1.0f));
    
    // 测试负Z方向
    Vec3 dir_z_neg(0, 0, -1);
    Vec3 support_z_neg = shape.GetLocalSupport(dir_z_neg);
    EXPECT_TRUE(FloatEqual(support_z_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_neg.y, 1.0f)); // 上顶面
    EXPECT_TRUE(FloatEqual(support_z_neg.z, -1.0f));
    
    // 测试斜向方向（X正，Y正，Z正）
    Vec3 dir_diagonal(1, 1, 1);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    // XZ平面上的分量归一化后乘以半径
    fpnumber expected_xz = 1.0f / sqrt(2.0f);
    EXPECT_TRUE(FloatEqual(support_diagonal.x, expected_xz)); // 归一化后的X分量乘以半径
    EXPECT_TRUE(FloatEqual(support_diagonal.y, 1.0f)); // 上顶面
    EXPECT_TRUE(FloatEqual(support_diagonal.z, expected_xz)); // 归一化后的Z分量乘以半径
    
    // 测试斜向方向（X正，Y负，Z正）
    Vec3 dir_diagonal_neg_y(1, -1, 1);
    Vec3 support_diagonal_neg_y = shape.GetLocalSupport(dir_diagonal_neg_y);
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.x, expected_xz)); // 归一化后的X分量乘以半径
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.y, -1.0f)); // 下底面
    EXPECT_TRUE(FloatEqual(support_diagonal_neg_y.z, expected_xz)); // 归一化后的Z分量乘以半径
    
    // 测试零方向向量
    Vec3 dir_zero(0, 0, 0);
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    EXPECT_TRUE(FloatEqual(support_zero.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_zero.y, 1.0f)); // 上顶面中心
    EXPECT_TRUE(FloatEqual(support_zero.z, 0.0f));
}

TEST(CylinderShapeTest, GetLocalSupport_DifferentSize) {
    // 创建一个高度为4，半径为2的圆柱体
    Cylinder cylinder(4.0f, 2.0f);
    CylinderShape shape(cylinder);
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(FloatEqual(support_x_pos.x, 2.0f));
    EXPECT_TRUE(FloatEqual(support_x_pos.y, 2.0f)); // 上顶面
    EXPECT_TRUE(FloatEqual(support_x_pos.z, 0.0f));
    
    // 测试负Y方向
    Vec3 dir_y_neg(0, -1, 0);
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(FloatEqual(support_y_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_neg.y, -2.0f)); // 下底面中心
    EXPECT_TRUE(FloatEqual(support_y_neg.z, 0.0f));
    
    // 测试斜向方向
    Vec3 dir_diagonal(1, 1, 0);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    EXPECT_TRUE(FloatEqual(support_diagonal.x, 2.0f)); // 半径
    EXPECT_TRUE(FloatEqual(support_diagonal.y, 2.0f)); // 上顶面
    EXPECT_TRUE(FloatEqual(support_diagonal.z, 0.0f));
}

} // namespace SympConvTest
