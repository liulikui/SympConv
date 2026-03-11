#include <gtest/gtest.h>
#include "SympConv/Sphere.h"
#include "SympConv/SphereShape.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using fpnumber = SympConv::fpnumber;
using Sphere = SympConv::Sphere;
using SphereShape = SympConv::SphereShape;
using Vec3 = SympConv::Vector3;

TEST(SphereShapeTest, GetLocalSupport) {
    // 创建一个半径为2的球体形状
    SphereShape shape(2.0f);
    
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
    EXPECT_TRUE(FloatEqual(support_y_pos.y, 2.0f));
    EXPECT_TRUE(FloatEqual(support_y_pos.z, 0.0f));
    
    // 测试负Y方向
    Vec3 dir_y_neg(0, -1, 0);
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(FloatEqual(support_y_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_y_neg.y, -2.0f));
    EXPECT_TRUE(FloatEqual(support_y_neg.z, 0.0f));
    
    // 测试正Z方向
    Vec3 dir_z_pos(0, 0, 1);
    Vec3 support_z_pos = shape.GetLocalSupport(dir_z_pos);
    EXPECT_TRUE(FloatEqual(support_z_pos.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_pos.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_pos.z, 2.0f));
    
    // 测试负Z方向
    Vec3 dir_z_neg(0, 0, -1);
    Vec3 support_z_neg = shape.GetLocalSupport(dir_z_neg);
    EXPECT_TRUE(FloatEqual(support_z_neg.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_neg.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_z_neg.z, -2.0f));
    
    // 测试斜向方向
    Vec3 dir_diagonal(1, 1, 1);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    fpnumber expected_length = 2.0f;
    fpnumber actual_length = support_diagonal.Length();
    EXPECT_TRUE(FloatEqual(actual_length, expected_length));
    
    // 测试负斜向方向
    Vec3 dir_neg_diagonal(-1, -1, -1);
    Vec3 support_neg_diagonal = shape.GetLocalSupport(dir_neg_diagonal);
    actual_length = support_neg_diagonal.Length();
    EXPECT_TRUE(FloatEqual(actual_length, expected_length));
    
    // 测试零方向向量
    Vec3 dir_zero(0, 0, 0);
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    EXPECT_TRUE(FloatEqual(support_zero.x, 0.0f));
    EXPECT_TRUE(FloatEqual(support_zero.y, 0.0f));
    EXPECT_TRUE(FloatEqual(support_zero.z, 0.0f));
}

TEST(SphereShapeTest, GetLocalSupport_DifferentRadius) {
    // 创建一个半径为3.5的球体形状
    SphereShape shape(3.5f);
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(FloatEqual(support_x_pos.x, 3.5f));
    
    // 测试斜向方向
    Vec3 dir_diagonal(1, 2, 2);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    fpnumber expected_length = 3.5f;
    fpnumber actual_length = support_diagonal.Length();
    EXPECT_TRUE(FloatEqual(actual_length, expected_length));
}

} // namespace SympConvTest
