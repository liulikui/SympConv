#include <gtest/gtest.h>
#include "SympConv/CollisionDetection.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/AABB.h"
#include "SympConv/Plane.h"
#include "SympConv/Box.h"
#include "SympConv/Sphere.h"
#include "TestUtils.h"

namespace SympConvTest {

using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;
using AABB = SympConv::AABB;
using Plane = SympConv::Plane;
using Box = SympConv::Box;
using Sphere = SympConv::Sphere;
using Capsule = SympConv::Capsule;

TEST(CollisionDetectionTest, RayIntersectsAABB) {
    // 创建一个AABB
    AABB aabb(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    
    // 从原点发射向AABB的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float tMin1, tMax1;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray1, aabb, tMin1, tMax1));
    EXPECT_TRUE(FloatEqual(tMin1, 1.0f));
    EXPECT_TRUE(FloatEqual(tMax1, 3.0f));
    
    // 从AABB内部发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float tMin2, tMax2;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray2, aabb, tMin2, tMax2));
    EXPECT_TRUE(FloatEqual(tMin2, 0.0f));
    EXPECT_TRUE(FloatEqual(tMax2, 1.0f));
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float tMin3, tMax3;
    EXPECT_FALSE(SympConv::RayIntersectsAABB(ray3, aabb, tMin3, tMax3));
}

TEST(CollisionDetectionTest, RayIntersectsPlane) {
    // 创建一个平面（z=0）
    Plane plane(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
    
    // 从z=-1向z=1发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray1, plane, t1));
    EXPECT_TRUE(FloatEqual(t1, 1.0f));
    
    // 平行于平面的射线
    Ray ray2(Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
    float t2;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray2, plane, t2));
    
    // 从平面后面发射的射线（反方向）
    Ray ray3(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray3, plane, t3));
}

TEST(CollisionDetectionTest, RayIntersectsSphere) {
    // 球体中心在原点，半径1
    Vec3 center(0.0f, 0.0f, 0.0f);
    float radius = 1.0f;
    
    // 从z=-2向z=2发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1, t2;
    EXPECT_TRUE(SympConv::RayIntersectsSphere(ray1, center, radius, t1, t2));
    EXPECT_TRUE(FloatEqual(t1, 1.0f));
    EXPECT_TRUE(FloatEqual(t2, 3.0f));
    
    // 不相交的射线
    Ray ray2(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3, t4;
    EXPECT_FALSE(SympConv::RayIntersectsSphere(ray2, center, radius, t3, t4));
}

TEST(CollisionDetectionTest, RayIntersectsTriangle) {
    // 三角形在z=0平面上
    Vec3 v0(0.0f, 0.0f, 0.0f);
    Vec3 v1(1.0f, 0.0f, 0.0f);
    Vec3 v2(0.0f, 1.0f, 0.0f);
    
    // 从z=-1向三角形中心发射的射线
    Ray ray1(Vec3(0.3f, 0.3f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t, u, v;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray1, v0, v1, v2, t, u, v));
    EXPECT_TRUE(FloatEqual(t, 1.0f));
    EXPECT_TRUE(FloatEqual(u, 0.3f));
    EXPECT_TRUE(FloatEqual(v, 0.3f));
    
    // 从三角形外部发射的射线
    Ray ray2(Vec3(2.0f, 2.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t2, u2, v2_;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(ray2, v0, v1, v2, t2, u2, v2_));
}

TEST(CollisionDetectionTest, RayIntersectsSegment) {
    // 线段从(0,0,0)到(1,1,0)
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(1.0f, 1.0f, 0.0f);
    
    // 从(0.5, 0.5, -1)向z轴正方向发射的射线
    Ray ray1(Vec3(0.5f, 0.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t, s;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray1, start, end, t, s));
    EXPECT_TRUE(FloatEqual(t, 1.0f));
    EXPECT_TRUE(FloatEqual(s, 0.5f));
    
    // 不相交的射线
    Ray ray2(Vec3(2.0f, 2.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t2, s2;
    EXPECT_FALSE(SympConv::RayIntersectsSegment(ray2, start, end, t2, s2));
}

TEST(CollisionDetectionTest, AABBIntersectsAABB) {
    // 两个相交的AABB
    AABB aabb1(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB aabb2(Vec3(0.0f, 0.0f, 0.0f), Vec3(2.0f, 2.0f, 2.0f));
    EXPECT_TRUE(SympConv::AABBIntersectsAABB(aabb1, aabb2));
    
    // 两个不相交的AABB
    AABB aabb3(Vec3(-2.0f, -2.0f, -2.0f), Vec3(-1.0f, -1.0f, -1.0f));
    EXPECT_FALSE(SympConv::AABBIntersectsAABB(aabb1, aabb3));
}

TEST(CollisionDetectionTest, PointInAABB) {
    AABB aabb(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    
    // AABB内部的点
    Vec3 point1(0.0f, 0.0f, 0.0f);
    EXPECT_TRUE(SympConv::PointInAABB(point1, aabb));
    
    // AABB边界上的点
    Vec3 point2(1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(SympConv::PointInAABB(point2, aabb));
    
    // AABB外部的点
    Vec3 point3(2.0f, 0.0f, 0.0f);
    EXPECT_FALSE(SympConv::PointInAABB(point3, aabb));
}

TEST(CollisionDetectionTest, PointOnPlane) {
    // z=0平面
    Plane plane(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
    
    // 平面上的点
    Vec3 point1(1.0f, 1.0f, 0.0f);
    EXPECT_TRUE(SympConv::PointOnPlane(point1, plane));
    
    // 平面上方的点
    Vec3 point2(1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(SympConv::PointOnPlanePositiveSide(point2, plane));
    EXPECT_FALSE(SympConv::PointOnPlaneNegativeSide(point2, plane));
    
    // 平面下方的点
    Vec3 point3(1.0f, 1.0f, -1.0f);
    EXPECT_FALSE(SympConv::PointOnPlanePositiveSide(point3, plane));
    EXPECT_TRUE(SympConv::PointOnPlaneNegativeSide(point3, plane));
}

TEST(CollisionDetectionTest, RayIntersectsBox) {
    // 创建一个盒子
    Box box(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    
    // 从盒子前面发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsBox(ray1, box, t1));
    
    // 从盒子后面发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, -1.0f));
    float t2;
    EXPECT_TRUE(SympConv::RayIntersectsBox(ray2, box, t2));
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsBox(ray3, box, t3));
}

TEST(CollisionDetectionTest, BoxIntersectsBox) {
    // 创建两个相交的盒子
    Box box1(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Box box2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::BoxIntersectsBox(box1, box2));
    
    // 创建两个不相交的盒子
    Box box3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(SympConv::BoxIntersectsBox(box1, box3));
}

TEST(CollisionDetectionTest, SphereIntersectsBox) {
    // 创建一个球体和一个盒子
    Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Box box(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::SphereIntersectsBox(sphere, box));
    
    // 创建一个不相交的球体和盒子
    Sphere sphere2(Vec3(3.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_FALSE(SympConv::SphereIntersectsBox(sphere2, box));
}

TEST(CollisionDetectionTest, SphereIntersectsSphere) {
    // 创建两个相交的球体
    Sphere sphere1(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Sphere sphere2(Vec3(1.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_TRUE(SympConv::SphereIntersectsSphere(sphere1, sphere2));
    
    // 创建两个不相交的球体
    Sphere sphere3(Vec3(3.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_FALSE(SympConv::SphereIntersectsSphere(sphere1, sphere3));
}

TEST(CollisionDetectionTest, RayIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 从胶囊体前面发射的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(ray1, capsule, t1));
    
    // 从胶囊体后面发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 4.0f), Vec3(0.0f, 0.0f, -1.0f));
    float t2;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(ray2, capsule, t2));
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsCapsule(ray3, capsule, t3));
}

TEST(CollisionDetectionTest, PlaneIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 与胶囊体相交的平面
    Plane plane1(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
    EXPECT_TRUE(SympConv::PlaneIntersectsCapsule(plane1, capsule));
    
    // 与胶囊体不相交的平面
    Plane plane2(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 2.0f, 1.0f));
    EXPECT_FALSE(SympConv::PlaneIntersectsCapsule(plane2, capsule));
}

TEST(CollisionDetectionTest, BoxIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 与胶囊体相交的盒子
    Box box1(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::BoxIntersectsCapsule(box1, capsule));
    
    // 与胶囊体不相交的盒子
    Box box2(Vec3(2.0f, 2.0f, 1.0f), Vec3(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(SympConv::BoxIntersectsCapsule(box2, capsule));
}

TEST(CollisionDetectionTest, SphereIntersectsCapsule) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(0.0f, 0.0f, 2.0f);
    float radius = 1.0f;
    Capsule capsule(start, end, radius);
    
    // 与胶囊体相交的球体
    Sphere sphere1(Vec3(0.0f, 0.0f, 1.0f), 0.5f);
    EXPECT_TRUE(SympConv::SphereIntersectsCapsule(sphere1, capsule));
    
    // 与胶囊体不相交的球体
    Sphere sphere2(Vec3(2.0f, 2.0f, 1.0f), 0.5f);
    EXPECT_FALSE(SympConv::SphereIntersectsCapsule(sphere2, capsule));
}

TEST(CollisionDetectionTest, CapsuleIntersectsCapsule) {
    // 相交的两个胶囊体
    Vec3 start1(0.0f, 0.0f, 0.0f);
    Vec3 end1(0.0f, 0.0f, 2.0f);
    float radius1 = 1.0f;
    Capsule capsule1(start1, end1, radius1);
    
    Vec3 start2(1.0f, 0.0f, 1.0f);
    Vec3 end2(1.0f, 0.0f, 3.0f);
    float radius2 = 1.0f;
    Capsule capsule2(start2, end2, radius2);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule1, capsule2));
    
    // 不相交的两个胶囊体
    Vec3 start3(3.0f, 0.0f, 0.0f);
    Vec3 end3(3.0f, 0.0f, 2.0f);
    float radius3 = 0.5f;
    Capsule capsule3(start3, end3, radius3);
    
    EXPECT_FALSE(SympConv::CapsuleIntersectsCapsule(capsule1, capsule3));
}

} // namespace SympConvTest