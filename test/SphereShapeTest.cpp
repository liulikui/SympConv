#include <gtest/gtest.h>
#include "SympConv/Sphere.h"
#include "SympConv/SphereShape.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Sphere = SympConv::Sphere;
using SphereShape = SympConv::SphereShape;
using Vec3 = SympConv::Vector3;
using AABB = SympConv::AABB;

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
    Real expected_length = 2.0f;
    Real actual_length = support_diagonal.Length();
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
    Real expected_length = 3.5f;
    Real actual_length = support_diagonal.Length();
    EXPECT_TRUE(FloatEqual(actual_length, expected_length));
}

TEST(SphereShapeTest, GetLocalInertiaTensor) {
    // 创建一个半径为1的球体形状
    SphereShape shape(1.0f);
    
    // 测试质量为1的情况
    Real mass = 1.0f;
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于球体，三个分量应该相等
    EXPECT_TRUE(FloatEqual(inertia.x, inertia.y));
    EXPECT_TRUE(FloatEqual(inertia.y, inertia.z));
    
    // 验证理论值：(2/5) * 1 * 1² = 0.4
    Real expected = 0.4f;
    EXPECT_TRUE(FloatEqual(inertia.x, expected));
}

TEST(SphereShapeTest, GetLocalInertiaTensor_DifferentSize) {
    // 创建一个半径为2的球体形状
    SphereShape shape(2.0f);
    
    // 测试质量为2的情况
    Real mass = 2.0f;
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于球体，三个分量应该相等
    EXPECT_TRUE(FloatEqual(inertia.x, inertia.y));
    EXPECT_TRUE(FloatEqual(inertia.y, inertia.z));
    
    // 验证理论值：(2/5) * 2 * 2² = 3.2
    Real expected = 3.2f;
    EXPECT_TRUE(FloatEqual(inertia.x, expected));
}

TEST(SphereShapeTest, GetLocalBounds) {
    // 创建一个半径为1的球体形状
    SphereShape shape(1.0f);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(FloatEqual(bounds.mMin.x, -1.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMin.y, -1.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMin.z, -1.0f));
    
    EXPECT_TRUE(FloatEqual(bounds.mMax.x, 1.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMax.y, 1.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMax.z, 1.0f));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 0.0f));
    EXPECT_TRUE(FloatEqual(center.y, 0.0f));
    EXPECT_TRUE(FloatEqual(center.z, 0.0f));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, 2.0f)); // 直径
    EXPECT_TRUE(FloatEqual(size.y, 2.0f)); // 直径
    EXPECT_TRUE(FloatEqual(size.z, 2.0f)); // 直径
}

TEST(SphereShapeTest, GetLocalBounds_DifferentSize) {
    // 创建一个半径为2的球体形状
    SphereShape shape(2.0f);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(FloatEqual(bounds.mMin.x, -2.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMin.y, -2.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMin.z, -2.0f));
    
    EXPECT_TRUE(FloatEqual(bounds.mMax.x, 2.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMax.y, 2.0f));
    EXPECT_TRUE(FloatEqual(bounds.mMax.z, 2.0f));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 0.0f));
    EXPECT_TRUE(FloatEqual(center.y, 0.0f));
    EXPECT_TRUE(FloatEqual(center.z, 0.0f));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, 4.0f)); // 直径
    EXPECT_TRUE(FloatEqual(size.y, 4.0f)); // 直径
    EXPECT_TRUE(FloatEqual(size.z, 4.0f)); // 直径
}

} // namespace SympConvTest
