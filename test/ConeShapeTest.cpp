#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include <cmath>
#include "SympConv/Cone.h"
#include "SympConv/ConeShape.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "SympConv/Ray.h"
#include "SympConv/RayCast.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Cone = SympConv::Cone;
using ConeShape = SympConv::ConeShape;
using Vec3 = SympConv::Vector3;
using AABB = SympConv::AABB;
using Ray = SympConv::Ray;
using RayCastResult = SympConv::RayCastResult;

TEST(ConeShapeTest, GetLocalSupport) {
    // 创建一个高度为2，半径为1的圆锥体
    Cone cone(Real(2.0), Real(1.0));
    ConeShape shape(cone);
    
    // 测试正Y方向（顶点方向）
    Vec3 dir_y_pos(0, 1, 0);
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.y, Real(2.0))); // 顶点
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.z, Real(0.0)));
    
    // 测试负Y方向（底面方向）
    Vec3 dir_y_neg(0, -1, 0);
    Vec3 support_y_neg = shape.GetLocalSupport(dir_y_neg);
    EXPECT_TRUE(SympConv::RealEqual(support_y_neg.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_y_neg.y, Real(0.0))); // 底面中心
    EXPECT_TRUE(SympConv::RealEqual(support_y_neg.z, Real(0.0)));
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.x, Real(1.0))); // 底面边缘
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.z, Real(0.0)));
    
    // 测试正Z方向
    Vec3 dir_z_pos(0, 0, 1);
    Vec3 support_z_pos = shape.GetLocalSupport(dir_z_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_z_pos.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_pos.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_pos.z, Real(1.0))); // 底面边缘
    
    // 测试负X方向
    Vec3 dir_x_neg(-1, 0, 0);
    Vec3 support_x_neg = shape.GetLocalSupport(dir_x_neg);
    EXPECT_TRUE(SympConv::RealEqual(support_x_neg.x, Real(-1.0))); // 底面边缘
    EXPECT_TRUE(SympConv::RealEqual(support_x_neg.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_x_neg.z, Real(0.0)));
    
    // 测试负Z方向
    Vec3 dir_z_neg(0, 0, -1);
    Vec3 support_z_neg = shape.GetLocalSupport(dir_z_neg);
    EXPECT_TRUE(SympConv::RealEqual(support_z_neg.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_neg.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_z_neg.z, Real(-1.0))); // 底面边缘
    
    // 测试斜向方向（X正，Y正，Z正）
    Vec3 dir_diagonal(1, 1, 1);
    Vec3 support_diagonal = shape.GetLocalSupport(dir_diagonal);
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal.y, Real(2.0))); // 顶点
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal.z, Real(0.0)));
    
    // 测试斜向方向（X正，Y负，Z正）
    Vec3 dir_diagonal_neg_y(1, -1, 1);
    Vec3 support_diagonal_neg_y = shape.GetLocalSupport(dir_diagonal_neg_y);
    // X和Z分量应该是归一化后的方向乘以半径
    Real expected_xz = Real(1.0) / sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal_neg_y.x, expected_xz));
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal_neg_y.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal_neg_y.z, expected_xz));
    
    // 测试零方向向量
    Vec3 dir_zero(0, 0, 0);
    Vec3 support_zero = shape.GetLocalSupport(dir_zero);
    // 零方向向量默认返回底面中心
    EXPECT_TRUE(SympConv::RealEqual(support_zero.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_zero.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_zero.z, Real(0.0)));
}

TEST(ConeShapeTest, GetLocalSupport_DifferentSize) {
    // 创建一个高度为4，半径为2的圆锥体
    Cone cone(Real(4.0), Real(2.0));
    ConeShape shape(cone);
    
    // 测试正Y方向（顶点方向）
    Vec3 dir_y_pos(0, 1, 0);
    Vec3 support_y_pos = shape.GetLocalSupport(dir_y_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.y, Real(4.0))); // 顶点
    EXPECT_TRUE(SympConv::RealEqual(support_y_pos.z, Real(0.0)));
    
    // 测试正X方向
    Vec3 dir_x_pos(1, 0, 0);
    Vec3 support_x_pos = shape.GetLocalSupport(dir_x_pos);
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.x, Real(2.0))); // 底面边缘
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_x_pos.z, Real(0.0)));
    
    // 测试斜向方向（X正，Y负，Z正）
    Vec3 dir_diagonal_neg_y(1, -1, 1);
    Vec3 support_diagonal_neg_y = shape.GetLocalSupport(dir_diagonal_neg_y);
    // X和Z分量应该是归一化后的方向乘以半径
    Real expected_xz = Real(2.0) / sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal_neg_y.x, expected_xz));
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal_neg_y.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(support_diagonal_neg_y.z, expected_xz));
}

TEST(ConeShapeTest, GetLocalInertiaTensor) {
    // 创建一个高度为2，半径为1的圆锥体
    Cone cone(Real(2.0), Real(1.0));
    ConeShape shape(cone);
    
    // 测试质量为1的情况
    Real mass = Real(1.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于沿Y轴的圆锥体，Ix和Iz应该相等
    EXPECT_TRUE(SympConv::RealEqual(inertia.x, inertia.z));
    
    // 验证Iy应该小于Ix和Iz（因为圆锥体绕对称轴的转动惯量更小）
    EXPECT_TRUE(inertia.y < inertia.x);
}

TEST(ConeShapeTest, GetLocalInertiaTensor_DifferentSize) {
    // 创建一个高度为4，半径为2的圆锥体
    Cone cone(Real(4.0), Real(2.0));
    ConeShape shape(cone);
    
    // 测试质量为2的情况
    Real mass = Real(2.0);
    Vec3 inertia = shape.GetLocalInertiaTensor(mass);
    
    // 验证惯性张量的各个分量都大于0
    EXPECT_TRUE(inertia.x > 0);
    EXPECT_TRUE(inertia.y > 0);
    EXPECT_TRUE(inertia.z > 0);
    
    // 对于沿Y轴的圆锥体，Ix和Iz应该相等
    EXPECT_TRUE(SympConv::RealEqual(inertia.x, inertia.z));
    
    // 验证Iy应该小于Ix和Iz
    EXPECT_TRUE(inertia.y < inertia.x);
}

TEST(ConeShapeTest, GetLocalBounds) {
    // 创建一个高度为2，半径为1的圆锥体
    Cone cone(Real(2.0), Real(1.0));
    ConeShape shape(cone);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.y, Real(0.0))); // 底面
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.z, Real(-1.0)));
    
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.y, Real(2.0))); // 高度
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.z, Real(1.0)));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(SympConv::RealEqual(center.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(center.y, Real(1.0))); // 高度的一半
    EXPECT_TRUE(SympConv::RealEqual(center.z, Real(0.0)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(SympConv::RealEqual(size.x, Real(2.0))); // 直径
    EXPECT_TRUE(SympConv::RealEqual(size.y, Real(2.0))); // 高度
    EXPECT_TRUE(SympConv::RealEqual(size.z, Real(2.0))); // 直径
}

TEST(ConeShapeTest, GetLocalBounds_DifferentSize) {
    // 创建一个高度为4，半径为2的圆锥体
    Cone cone(Real(4.0), Real(2.0));
    ConeShape shape(cone);
    
    // 获取本地坐标系中的AABB
    AABB bounds = shape.GetLocalBounds();
    
    // 验证AABB的最小和最大点
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.x, Real(-2.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.y, Real(0.0))); // 底面
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMin.z, Real(-2.0)));
    
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.y, Real(4.0))); // 高度
    EXPECT_TRUE(SympConv::RealEqual(bounds.mMax.z, Real(2.0)));
    
    // 验证AABB的中心
    Vec3 center = bounds.GetCenter();
    EXPECT_TRUE(SympConv::RealEqual(center.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(center.y, Real(2.0))); // 高度的一半
    EXPECT_TRUE(SympConv::RealEqual(center.z, Real(0.0)));
    
    // 验证AABB的大小
    Vec3 size = bounds.GetSize();
    EXPECT_TRUE(SympConv::RealEqual(size.x, Real(4.0))); // 直径
    EXPECT_TRUE(SympConv::RealEqual(size.y, Real(4.0))); // 高度
    EXPECT_TRUE(SympConv::RealEqual(size.z, Real(4.0))); // 直径
}

TEST(ConeShapeTest, RayCast) {
    // 创建一个高度为2，底面半径为1的圆锥体，底面在y=0，顶点在(0, 2, 0)
    Cone cone(1, 2); // 半径为1，高度为2
    ConeShape coneShape(cone);
    
    // 测试从正面命中圆锥体侧面
    Ray ray1(Vec3(2, 1, 0), Vec3(-1, 0, 0), 10.0);
    RayCastResult result1;
    EXPECT_TRUE(coneShape.RayCast(ray1, result1));
    EXPECT_TRUE(SympConv::RealEqual(result1.mPoint.x, 1.0 - result1.mPoint.y / 2.0));
    
    // 测试命中圆锥体的底面
    Ray ray2(Vec3(0.5, -1, 0), Vec3(0, 1, 0), 10.0);
    RayCastResult result2;
    EXPECT_TRUE(coneShape.RayCast(ray2, result2));
    EXPECT_TRUE(SympConv::RealEqual(result2.mPoint.y, 0.0));
    EXPECT_TRUE(SympConv::RealEqual(result2.mNormal.y, -1.0));
    
    // 测试射线错过圆锥体
    Ray ray3(Vec3(2, 3, 0), Vec3(1, 1, 0), 10.0);
    RayCastResult result3;
    EXPECT_FALSE(coneShape.RayCast(ray3, result3));
}

} // namespace SympConvTest
