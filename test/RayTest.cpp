#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include <cmath>
#include "SympConv/Ray.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "SympConv/Transform.h"
#include "SympConv/Quaternion.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Ray = SympConv::Ray;
using Vec3 = SympConv::Vector3;
using Transform = SympConv::Transform;

TEST(RayTest, DefaultConstructor) {
    Ray defaultRay;
    EXPECT_TRUE(SympConv::RealEqual(defaultRay.mOrigin.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(defaultRay.mDirection.z, Real(1.0)));
}

TEST(RayTest, ParameterizedConstructor) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    EXPECT_TRUE(SympConv::RealEqual(ray.mOrigin.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(ray.mDirection.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(ray.tMin, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(ray.tMax, Real(1.0)));
}

TEST(RayTest, GetPoint) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    Vec3 point = ray.GetPoint(Real(5.0));
    EXPECT_TRUE(SympConv::RealEqual(point.x, Real(5.0)));
    EXPECT_TRUE(SympConv::RealEqual(point.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(point.z, Real(0.0)));
}

TEST(RayTest, DistanceTo) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    Real distance = ray.DistanceTo(Vec3(Real(0.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(distance, Real(1.0)));
}

TEST(RayTest, Transform) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    Transform transform(Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Ray transformedRay = ray.Transform(transform);
    EXPECT_TRUE(SympConv::RealEqual(transformedRay.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(transformedRay.mDirection.x, Real(1.0)));
}

TEST(RayTest, InverseTransform) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    Transform transform(Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Ray transformedRay = ray.Transform(transform);
    Ray inverseTransformedRay = transformedRay.InverseTransform(transform);
    EXPECT_TRUE(SympConv::RealEqual(inverseTransformedRay.mOrigin.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(inverseTransformedRay.mDirection.x, Real(1.0)));
}

TEST(RayTest, TransformWithRotation) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    // 创建一个绕Y轴旋转90度的变换
    auto rotation = SympConv::Quaternion::FromEulerAngles(Real(0.0), Real(M_PI / 2.0), Real(0.0));
    Transform transform = Transform::Rotation(rotation);
    Ray transformedRay = ray.Transform(transform);
    // 方向应该从(1,0,0)变为(0,0,-1)
    EXPECT_TRUE(SympConv::RealEqual(transformedRay.mDirection.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(transformedRay.mDirection.z, Real(-1.0)));
}

TEST(RayTest, TransformWithScaling) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)), Real(0.0), Real(1.0));
    // 创建一个缩放2倍的变换
    Transform transform = Transform::Scaling(Vec3(Real(2.0), Real(2.0), Real(2.0)));
    Ray transformedRay = ray.Transform(transform);
    // 方向向量应该被缩放
    EXPECT_TRUE(SympConv::RealEqual(transformedRay.mDirection.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(transformedRay.mDirection.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(transformedRay.mDirection.z, Real(2.0)));
}

TEST(RayTest, Normalize) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(2.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    ray.Normalize();
    EXPECT_TRUE(SympConv::RealEqual(ray.mDirection.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(ray.mDirection.Length(), Real(1.0)));
}

TEST(RayTest, Normalized) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(0.0), Real(3.0), Real(0.0)), Real(0.0), Real(1.0));
    Ray normalizedRay = ray.Normalized();
    EXPECT_TRUE(SympConv::RealEqual(normalizedRay.mDirection.y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(normalizedRay.mDirection.Length(), Real(1.0)));
    // 原始射线应该保持不变
    EXPECT_TRUE(SympConv::RealEqual(ray.mDirection.y, Real(3.0)));
}



TEST(RayTest, DistanceTo_BoundaryCases) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    
    // 点就是射线原点
    Real distance1 = ray.DistanceTo(Vec3(Real(0.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(distance1, Real(0.0)));
    
    // 点在射线上但超出射线段范围
    Real distance2 = ray.DistanceTo(Vec3(Real(5.0), Real(0.0), Real(0.0)));
    // 应该返回点到射线段终点的距离
    EXPECT_TRUE(SympConv::RealEqual(distance2, Real(4.0)));
    
    // 点在射线反方向
    Real distance3 = ray.DistanceTo(Vec3(Real(-1.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(distance3, std::sqrt(Real(2.0))));
}

TEST(RayTest, CopyConstructor) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)), Real(0.0), Real(1.0));
    Ray copy(original);
    EXPECT_TRUE(SympConv::RealEqual(copy.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(copy.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(copy.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(copy.mDirection.x, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(copy.mDirection.y, Real(5.0)));
    EXPECT_TRUE(SympConv::RealEqual(copy.mDirection.z, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(copy.tMin, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(copy.tMax, Real(1.0)));
}

TEST(RayTest, MoveConstructor) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)), Real(0.0), Real(1.0));
    Ray moved(std::move(original));
    EXPECT_TRUE(SympConv::RealEqual(moved.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(moved.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(moved.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(moved.mDirection.x, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(moved.mDirection.y, Real(5.0)));
    EXPECT_TRUE(SympConv::RealEqual(moved.mDirection.z, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(moved.tMin, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(moved.tMax, Real(1.0)));
}

TEST(RayTest, AssignmentOperator) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)), Real(0.0), Real(1.0));
    Ray assigned;
    assigned = original;
    EXPECT_TRUE(SympConv::RealEqual(assigned.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mDirection.x, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mDirection.y, Real(5.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mDirection.z, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.tMin, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.tMax, Real(1.0)));
}

TEST(RayTest, MoveAssignmentOperator) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)), Real(0.0), Real(1.0));
    Ray assigned;
    assigned = std::move(original);
    EXPECT_TRUE(SympConv::RealEqual(assigned.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mDirection.x, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mDirection.y, Real(5.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.mDirection.z, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.tMin, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(assigned.tMax, Real(1.0)));
}

TEST(RayTest, Normalize_ZeroVector) {
    // 测试归一化零向量的情况
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(0.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    ray.Normalize();
    // 零向量归一化后应该还是零向量
    EXPECT_TRUE(SympConv::RealEqual(ray.mDirection.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(ray.mDirection.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(ray.mDirection.z, Real(0.0)));
}

TEST(RayTest, ChainedOperations) {
    // 测试链式操作
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(2.0), Real(2.0), Real(2.0)), Real(0.0), Real(1.0));
    Vec3 point = ray.Normalize().GetPoint(Real(1.0));
    // 归一化后方向向量长度为1，所以t=1时的点应该在单位球面上
    EXPECT_TRUE(SympConv::RealEqual(point.Length(), Real(1.0)));
}

TEST(RayTest, DistanceTo_WithTMinTMax) {
    // 测试射线段到点的距离，考虑tMin和tMax
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(1.0), Real(3.0));
    
    // 点在射线段内部 (t=2.0)
    Real distance1 = ray.DistanceTo(Vec3(Real(2.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(distance1, Real(1.0)));
    
    // 点在射线段起点之前 (t=0.5)
    Real distance2 = ray.DistanceTo(Vec3(Real(0.5), Real(1.0), Real(0.0)));
    // 应该返回点到起点(1,0,0)的距离
    EXPECT_TRUE(SympConv::RealEqual(distance2, std::sqrt(Real(0.25 + 1.0))));
    
    // 点在射线段终点之后 (t=4.0)
    Real distance3 = ray.DistanceTo(Vec3(Real(4.0), Real(1.0), Real(0.0)));
    // 应该返回点到终点(3,0,0)的距离
    EXPECT_TRUE(SympConv::RealEqual(distance3, std::sqrt(Real(1.0 + 1.0))));
}

TEST(RayTest, DistanceToRay_WithTMinTMax) {
    // 测试两条射线段之间的距离
    // ray1: 从(0,0,0)沿x轴到(2,0,0)
    // ray2: 从(1,-1,0)沿y轴到(1,1,0)
    // 这两条射线段在(1,0,0)处相交
    Ray ray1(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(2.0));
    Ray ray2(Vec3(Real(1.0), Real(-1.0), Real(0.0)), Vec3(Real(0.0), Real(1.0), Real(0.0)), Real(0.0), Real(2.0));
    
    // 两条射线段应该相交，距离为0
    Real distance = ray1.DistanceTo(ray2);
    EXPECT_TRUE(SympConv::RealEqual(distance, Real(0.0)));
    
    // 测试不相交的射线段
    Ray ray3(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)), Real(0.0), Real(1.0));
    Ray ray4(Vec3(Real(2.0), Real(1.0), Real(0.0)), Vec3(Real(0.0), Real(1.0), Real(0.0)), Real(0.0), Real(1.0));
    
    // 两条射线段不相交，距离应该是√2
    Real distance2 = ray3.DistanceTo(ray4);
    EXPECT_TRUE(SympConv::RealEqual(distance2, std::sqrt(Real(2.0))));
}

} // namespace SympConvTest
