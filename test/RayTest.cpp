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
    EXPECT_TRUE(FloatEqual(defaultRay.mOrigin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(defaultRay.mDirection.z, Real(1.0)));
}

TEST(RayTest, ParameterizedConstructor) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(ray.mOrigin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(ray.mDirection.x, Real(1.0)));
}

TEST(RayTest, GetPoint) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    Vec3 point = ray.GetPoint(Real(5.0));
    EXPECT_TRUE(FloatEqual(point.x, Real(5.0)));
    EXPECT_TRUE(FloatEqual(point.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(point.z, Real(0.0)));
}

TEST(RayTest, DistanceTo) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    Real distance = ray.DistanceTo(Vec3(Real(0.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance, Real(1.0)));
}

TEST(RayTest, Transform) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    Transform transform(Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Ray transformedRay = ray.Transform(transform);
    EXPECT_TRUE(FloatEqual(transformedRay.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(transformedRay.mDirection.x, Real(1.0)));
}

TEST(RayTest, InverseTransform) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    Transform transform(Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Ray transformedRay = ray.Transform(transform);
    Ray inverseTransformedRay = transformedRay.InverseTransform(transform);
    EXPECT_TRUE(FloatEqual(inverseTransformedRay.mOrigin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(inverseTransformedRay.mDirection.x, Real(1.0)));
}

TEST(RayTest, TransformWithRotation) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    // 创建一个绕Y轴旋转90度的变换
    auto rotation = SympConv::Quaternion::FromEulerAngles(Real(0.0), Real(M_PI / 2.0), Real(0.0));
    Transform transform = Transform::Rotation(rotation);
    Ray transformedRay = ray.Transform(transform);
    // 方向应该从(1,0,0)变为(0,0,-1)
    EXPECT_TRUE(FloatEqual(transformedRay.mDirection.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(transformedRay.mDirection.z, Real(-1.0)));
}

TEST(RayTest, TransformWithScaling) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    // 创建一个缩放2倍的变换
    Transform transform = Transform::Scaling(Vec3(Real(2.0), Real(2.0), Real(2.0)));
    Ray transformedRay = ray.Transform(transform);
    // 方向向量应该被缩放
    EXPECT_TRUE(FloatEqual(transformedRay.mDirection.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(transformedRay.mDirection.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(transformedRay.mDirection.z, Real(2.0)));
}

TEST(RayTest, Normalize) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(2.0), Real(0.0), Real(0.0)));
    ray.Normalize();
    EXPECT_TRUE(FloatEqual(ray.mDirection.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(ray.mDirection.Length(), Real(1.0)));
}

TEST(RayTest, Normalized) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(0.0), Real(3.0), Real(0.0)));
    Ray normalizedRay = ray.Normalized();
    EXPECT_TRUE(FloatEqual(normalizedRay.mDirection.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(normalizedRay.mDirection.Length(), Real(1.0)));
    // 原始射线应该保持不变
    EXPECT_TRUE(FloatEqual(ray.mDirection.y, Real(3.0)));
}

TEST(RayTest, DistanceToSegment) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    
    // 线段与射线平行，线段在射线正方向
    Real distance1 = ray.DistanceToSegment(Vec3(Real(2.0), Real(1.0), Real(0.0)), Vec3(Real(4.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance1, Real(1.0)));
    
    // 线段与射线相交
    Real distance2 = ray.DistanceToSegment(Vec3(Real(0.5), Real(1.0), Real(0.0)), Vec3(Real(0.5), Real(-1.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance2, Real(0.0)));
    
    // 线段在射线反方向
    Real distance3 = ray.DistanceToSegment(Vec3(Real(-2.0), Real(1.0), Real(0.0)), Vec3(Real(-1.0), Real(1.0), Real(0.0)));
    // 预期距离应该是√2
    EXPECT_NEAR(distance3, std::sqrt(Real(2.0)), 1e-5);
}

TEST(RayTest, DistanceToSegment_EdgeCases) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    
    // 线段退化为点
    Real distance1 = ray.DistanceToSegment(Vec3(Real(1.0), Real(1.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance1, Real(1.0)));
    
    // 射线原点在线段上
    Real distance2 = ray.DistanceToSegment(Vec3(Real(-1.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance2, Real(0.0)));
}

TEST(RayTest, DistanceToSegment_ObliqueAngle) {
    // 测试射线与线段成斜角的情况
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(0.0)));
    Real distance = ray.DistanceToSegment(Vec3(Real(2.0), Real(0.0), Real(0.0)), Vec3(Real(4.0), Real(0.0), Real(0.0)));
    // 计算预期距离：从射线到线段的垂直距离
    // 射线方向向量为(1,1,0)，线段在y=0平面
    // 预期距离应该是√2
    EXPECT_NEAR(distance, std::sqrt(Real(2.0)), 1e-5);
}

TEST(RayTest, DistanceToSegment_ParallelNonOverlapping) {
    // 测试射线与线段平行但不重叠的情况
    Ray ray(Vec3(Real(0.0), Real(1.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    Real distance = ray.DistanceToSegment(Vec3(Real(2.0), Real(2.0), Real(0.0)), Vec3(Real(4.0), Real(2.0), Real(0.0)));
    // 平行线段，距离应该是y方向的差值1.0
    EXPECT_TRUE(FloatEqual(distance, Real(1.0)));
}

TEST(RayTest, DistanceTo_BoundaryCases) {
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    
    // 点就是射线原点
    Real distance1 = ray.DistanceTo(Vec3(Real(0.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance1, Real(0.0)));
    
    // 点在射线上
    Real distance2 = ray.DistanceTo(Vec3(Real(5.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance2, Real(0.0)));
    
    // 点在射线反方向
    Real distance3 = ray.DistanceTo(Vec3(Real(-1.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(distance3, std::sqrt(Real(2.0))));
}

TEST(RayTest, CopyConstructor) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)));
    Ray copy(original);
    EXPECT_TRUE(FloatEqual(copy.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(copy.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(copy.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(copy.mDirection.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(copy.mDirection.y, Real(5.0)));
    EXPECT_TRUE(FloatEqual(copy.mDirection.z, Real(6.0)));
}

TEST(RayTest, MoveConstructor) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)));
    Ray moved(std::move(original));
    EXPECT_TRUE(FloatEqual(moved.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(moved.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(moved.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(moved.mDirection.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(moved.mDirection.y, Real(5.0)));
    EXPECT_TRUE(FloatEqual(moved.mDirection.z, Real(6.0)));
}

TEST(RayTest, AssignmentOperator) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)));
    Ray assigned;
    assigned = original;
    EXPECT_TRUE(FloatEqual(assigned.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(assigned.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(assigned.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(assigned.mDirection.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(assigned.mDirection.y, Real(5.0)));
    EXPECT_TRUE(FloatEqual(assigned.mDirection.z, Real(6.0)));
}

TEST(RayTest, MoveAssignmentOperator) {
    Ray original(Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(4.0), Real(5.0), Real(6.0)));
    Ray assigned;
    assigned = std::move(original);
    EXPECT_TRUE(FloatEqual(assigned.mOrigin.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(assigned.mOrigin.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(assigned.mOrigin.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(assigned.mDirection.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(assigned.mDirection.y, Real(5.0)));
    EXPECT_TRUE(FloatEqual(assigned.mDirection.z, Real(6.0)));
}

TEST(RayTest, Normalize_ZeroVector) {
    // 测试归一化零向量的情况
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(0.0), Real(0.0), Real(0.0)));
    ray.Normalize();
    // 零向量归一化后应该还是零向量
    EXPECT_TRUE(FloatEqual(ray.mDirection.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(ray.mDirection.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(ray.mDirection.z, Real(0.0)));
}

TEST(RayTest, ChainedOperations) {
    // 测试链式操作
    Ray ray(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(2.0), Real(2.0), Real(2.0)));
    Vec3 point = ray.Normalize().GetPoint(Real(1.0));
    // 归一化后方向向量长度为1，所以t=1时的点应该在单位球面上
    EXPECT_TRUE(FloatEqual(point.Length(), Real(1.0)));
}

} // namespace SympConvTest
