#include <gtest/gtest.h>
#include "SympConv/Capsule.h"
#include "SympConv/CapsuleShape.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Capsule = SympConv::Capsule;
using CapsuleShape = SympConv::CapsuleShape;
using Vec3 = SympConv::Vector3;
using AABB = SympConv::AABB;

TEST(CapsuleShapeTest, GetLocalSupport) {
    // 创建一个高度为2，半径为1的胶囊体
    Capsule capsule(Real(2.0), Real(1.0));
    CapsuleShape shape(capsule);
    
    // 测试正Y方向
    Vec3 dir_up(Real(0), Real(1), Real(0));
    Vec3 support_up = shape.GetLocalSupport(dir_up);
    EXPECT_EQ(support_up.x, Real(0.0));
    EXPECT_EQ(support_up.y, Real(1.0) + Real(1.0)); // 高度的一半加上半径
    EXPECT_EQ(support_up.z, Real(0.0));
    
    // 测试负Y方向
    Vec3 dir_down(Real(0), Real(-1), Real(0));
    Vec3 support_down = shape.GetLocalSupport(dir_down);
    EXPECT_EQ(support_down.x, Real(0.0));
    EXPECT_EQ(support_down.y, -Real(1.0) - Real(1.0)); // 负高度的一半减去半径
    EXPECT_EQ(support_down.z, Real(0.0));
    
    // 测试正X方向
    Vec3 dir_right(Real(1), Real(0), Real(0));
    Vec3 support_right = shape.GetLocalSupport(dir_right);
    EXPECT_EQ(support_right.x, Real(1.0)); // 半径
    EXPECT_EQ(support_right.y, Real(1.0)); // 上顶点
    EXPECT_EQ(support_right.z, Real(0.0));
    
    // 测试正Z方向
    Vec3 dir_forward(Real(0), Real(0), Real(1));
    Vec3 support_forward = shape.GetLocalSupport(dir_forward);
    EXPECT_EQ(support_forward.x, Real(0.0));
    EXPECT_EQ(support_forward.y, Real(1.0)); // 上顶点
    EXPECT_EQ(support_forward.z, Real(1.0)); // 半径
    
    // 测试斜向方向
    Vec3 dir_diagonal(Real(1), Real(1), Real(0));
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    EXPECT_EQ(support_diagonal.x, Real(1.0)); // 半径
    EXPECT_EQ(support_diagonal.y, Real(1.0)); // 上顶点
    EXPECT_EQ(support_diagonal.z, Real(0.0));
    
    // 测试负X方向
    Vec3 dir_left(Real(-1), Real(0), Real(0));
    Vec3 support_left = shape.GetLocalSupport(dir_left);
    EXPECT_EQ(support_left.x, -Real(1.0)); // 半径
    EXPECT_EQ(support_left.y, Real(1.0)); // 上顶点
    EXPECT_EQ(support_left.z, Real(0.0));
    
    // 测试负Z方向
    Vec3 dir_backward(Real(0), Real(0), Real(-1));
    Vec3 support_backward = shape.GetLocalSupport(dir_backward);
    EXPECT_EQ(support_backward.x, Real(0.0));
    EXPECT_EQ(support_backward.y, Real(1.0)); // 上顶点
    EXPECT_EQ(support_backward.z, -Real(1.0)); // 半径
}

TEST(CapsuleShapeTest, GetLocalSupport_ZeroDirection) {
    // 创建一个高度为2，半径为1的胶囊体
    Capsule capsule(Real(2.0), Real(1.0));
    CapsuleShape shape(capsule);
    
    // 测试零方向向量（应该返回上顶点）
    Vec3 dir_zero(Real(0), Real(0), Real(0));
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    EXPECT_EQ(support_zero.x, Real(0.0));
    EXPECT_EQ(support_zero.y, Real(1.0) + Real(1.0)); // 上顶点加上半径
    EXPECT_EQ(support_zero.z, Real(0.0));
}

TEST(CapsuleShapeTest, GetLocalSupport_SmallHeight) {
    // 创建一个高度很小的胶囊体（接近球体）
    Capsule capsule(Real(0.1), Real(1.0));
    CapsuleShape shape(capsule);
    
    // 测试正X方向
    Vec3 dir_right(Real(1), Real(0), Real(0));
    Vec3 support_right = shape.GetLocalSupport(dir_right);
    EXPECT_NEAR(support_right.x, Real(1.0), 1e-6); // 半径
    EXPECT_NEAR(support_right.y, Real(0.05), 1e-6); // 上顶点
    EXPECT_NEAR(support_right.z, Real(0.0), 1e-6);
    
    // 测试负Y方向
    Vec3 dir_down(Real(0), Real(-1), Real(0));
    Vec3 support_down = shape.GetLocalSupport(dir_down);
    EXPECT_NEAR(support_down.x, Real(0.0), 1e-6);
    EXPECT_NEAR(support_down.y, -Real(0.05) - Real(1.0), 1e-6); // 下顶点减去半径
    EXPECT_NEAR(support_down.z, Real(0.0), 1e-6);
}

TEST(CapsuleShapeTest, GetLocalInertiaTensor) {
    // 创建一个高度为2，半径为1的胶囊体
    Capsule capsule(Real(2.0), Real(1.0));
    CapsuleShape shape(capsule);
    
    // 测试质量为1的情况
    Real mass = Real(1.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于沿Y轴的胶囊体，Ix和Iz应该相等
    EXPECT_TRUE(FloatEqual(inertia.x, inertia.z));
    
    // 验证Ix应该大于Iy（因为胶囊体沿Y轴，绕Y轴的转动惯量更小）
    EXPECT_TRUE(inertia.x > inertia.y);
}

TEST(CapsuleShapeTest, GetLocalInertiaTensor_SphereCase) {
    // 创建一个高度为0的胶囊体（近似球体）
    Capsule capsule(Real(0.0), Real(1.0));
    CapsuleShape shape(capsule);
    
    // 测试质量为1的情况
    Real mass = Real(1.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 对于球体，三个分量应该相等
    EXPECT_TRUE(FloatEqual(inertia.x, inertia.y));
    EXPECT_TRUE(FloatEqual(inertia.y, inertia.z));
    
    // 球体的惯性张量理论值为 (2/5) * m * r^2 = 0.4
    Real expected = Real(0.4);
    EXPECT_TRUE(FloatEqual(inertia.x, expected));
}

TEST(CapsuleShapeTest, GetLocalInertiaTensor_DifferentSize) {
    // 创建一个高度为4，半径为2的胶囊体
    Capsule capsule(Real(4.0), Real(2.0));
    CapsuleShape shape(capsule);
    
    // 测试质量为2的情况
    Real mass = Real(2.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于沿Y轴的胶囊体，Ix和Iz应该相等
    EXPECT_TRUE(FloatEqual(inertia.x, inertia.z));
}

TEST(CapsuleShapeTest, GetLocalBounds) {
    // 创建一个高度为2，半径为1的胶囊体
    Capsule capsule(Real(2.0), Real(1.0));
    CapsuleShape shape(capsule);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(FloatEqual(bounds.mMin.x, -Real(1.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.y, -Real(1.0) - Real(1.0))); // 半高加上半径
    EXPECT_TRUE(FloatEqual(bounds.mMin.z, -Real(1.0)));
    
    EXPECT_TRUE(FloatEqual(bounds.mMax.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.y, Real(1.0) + Real(1.0))); // 半高加上半径
    EXPECT_TRUE(FloatEqual(bounds.mMax.z, Real(1.0)));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(center.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(center.z, Real(0.0)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, Real(2.0))); // 直径
    EXPECT_TRUE(FloatEqual(size.y, Real(2.0) + Real(2.0))); // 总高度加上直径
    EXPECT_TRUE(FloatEqual(size.z, Real(2.0))); // 直径
}

TEST(CapsuleShapeTest, GetLocalBounds_DifferentSize) {
    // 创建一个高度为4，半径为2的胶囊体
    Capsule capsule(Real(4.0), Real(2.0));
    CapsuleShape shape(capsule);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(FloatEqual(bounds.mMin.x, -Real(2.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMin.y, -Real(2.0) - Real(2.0))); // 半高加上半径
    EXPECT_TRUE(FloatEqual(bounds.mMin.z, -Real(2.0)));
    
    EXPECT_TRUE(FloatEqual(bounds.mMax.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(bounds.mMax.y, Real(2.0) + Real(2.0))); // 半高加上半径
    EXPECT_TRUE(FloatEqual(bounds.mMax.z, Real(2.0)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, Real(4.0))); // 直径
    EXPECT_TRUE(FloatEqual(size.y, Real(4.0) + Real(4.0))); // 总高度加上直径
    EXPECT_TRUE(FloatEqual(size.z, Real(4.0))); // 直径
}

} // namespace SympConvTest
