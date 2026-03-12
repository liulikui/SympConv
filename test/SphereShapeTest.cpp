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
    SphereShape shape(Real(2.0));
    
    // 测试正X方向
    Vec3 dir_x_pos(Real(1), Real(0), Real(0));
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.z, Real(0.0)));
    
    // 测试负X方向
    Vec3 dir_x_neg(Real(-1), Real(0), Real(0));
    Vec3 support_x_neg = shape.GetLocalSupport(dir_x_neg);
    EXPECT_TRUE(SympConv::RealEqual(support_x_neg.x, -Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_x_neg.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_x_neg.z, Real(0.0)));
    
    // 测试正Y方向
    Vec3 dir_y_pos(Real(0), Real(1), Real(0));
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.z, Real(0.0)));
    
    // 测试负Y方向
    Vec3 dir_y_neg(Real(0), Real(-1), Real(0));
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(SympConv::RealEqual(support_y_neg.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_y_neg.y, -Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_y_neg.z, Real(0.0)));
    
    // 测试正Z方向
    Vec3 dir_z_pos(Real(0), Real(0), Real(1));
    Vec3 support_z_pos = shape.GetLocalSupport(dir_z_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_z_pos.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_pos.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_pos.z, Real(2.0)));
    
    // 测试负Z方向
    Vec3 dir_z_neg(Real(0), Real(0), Real(-1));
    Vec3 support_z_neg = shape.GetLocalSupport(dir_z_neg);
    EXPECT_TRUE(SympConv::RealEqual(support_z_neg.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_neg.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_neg.z, -Real(2.0)));
    
    // 测试斜向方向
    Vec3 dir_diagonal(Real(1), Real(1), Real(1));
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    Real expected_length = Real(2.0);
    Real actual_length = support_diagonal.Length();
    EXPECT_TRUE(SympConv::RealEqual(actual_length, expected_length));
    
    // 测试负斜向方向
    Vec3 dir_neg_diagonal(Real(-1), Real(-1), Real(-1));
    Vec3 support_neg_diagonal = shape.GetLocalSupport(dir_neg_diagonal);
    actual_length = support_neg_diagonal.Length();
    EXPECT_TRUE(SympConv::RealEqual(actual_length, expected_length));
    
    // 测试零方向向量
    Vec3 dir_zero(Real(0), Real(0), Real(0));
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    EXPECT_TRUE(SympConv::RealEqual(support_zero.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_zero.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_zero.z, Real(0.0)));
}

TEST(SphereShapeTest, GetLocalSupport_DifferentRadius) {
    // 创建一个半径为3.5的球体形状
    SphereShape shape(Real(3.5));
    
    // 测试正X方向
    Vec3 dir_x_pos(Real(1), Real(0), Real(0));
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.x, Real(3.5)));
    
    // 测试斜向方向
    Vec3 dir_diagonal(Real(1), Real(2), Real(2));
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    Real expected_length = Real(3.5);
    Real actual_length = support_diagonal.Length();
    EXPECT_TRUE(SympConv::RealEqual(actual_length, expected_length));
}

TEST(SphereShapeTest, GetLocalInertiaTensor) {
    // 创建一个半径为1的球体形状
    SphereShape shape(Real(1.0));
    
    // 测试质量为1的情况
    Real mass = Real(1.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于球体，三个分量应该相等
    EXPECT_TRUE(SympConv::RealEqual(inertia.x, inertia.y));
    EXPECT_TRUE(SympConv::RealEqual(inertia.y, inertia.z));
    
    // 验证理论值：(2/5) * 1 * 1² = 0.4
    Real expected = Real(0.4);
    EXPECT_TRUE(SympConv::RealEqual(inertia.x, expected));
}

TEST(SphereShapeTest, GetLocalInertiaTensor_DifferentSize) {
    // 创建一个半径为2的球体形状
    SphereShape shape(Real(2.0));
    
    // 测试质量为2的情况
    Real mass = Real(2.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于球体，三个分量应该相等
    EXPECT_TRUE(SympConv::RealEqual(inertia.x, inertia.y));
    EXPECT_TRUE(SympConv::RealEqual(inertia.y, inertia.z));
    
    // 验证理论值：(2/5) * 2 * 2² = 3.2
    Real expected = Real(3.2);
    EXPECT_TRUE(SympConv::RealEqual(inertia.x, expected));
}

TEST(SphereShapeTest, GetLocalBounds) {
    // 创建一个半径为1的球体形状
    SphereShape shape(Real(1.0));
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.x, -Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.y, -Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.z, -Real(1.0)));
    
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.z, Real(1.0)));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(SympConv::RealEqual(center.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(center.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(center.z, Real(0.0)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(SympConv::RealEqual(size.x, Real(2.0))); // 直径
    EXPECT_TRUE(SympConv::RealEqual(size.y, Real(2.0))); // 直径
    EXPECT_TRUE(SympConv::RealEqual(size.z, Real(2.0))); // 直径
}

TEST(SphereShapeTest, GetLocalBounds_DifferentSize) {
    // 创建一个半径为2的球体形状
    SphereShape shape(Real(2.0));
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.x, -Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.y, -Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.z, -Real(2.0)));
    
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.z, Real(2.0)));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(SympConv::RealEqual(center.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(center.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(center.z, Real(0.0)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(SympConv::RealEqual(size.x, Real(4.0))); // 直径
    EXPECT_TRUE(SympConv::RealEqual(size.y, Real(4.0))); // 直径
    EXPECT_TRUE(SympConv::RealEqual(size.z, Real(4.0))); // 直径
}

} // namespace SympConvTest
