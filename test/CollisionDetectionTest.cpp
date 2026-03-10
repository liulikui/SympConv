#include <gtest/gtest.h>
#include "SympConv/CollisionDetection.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/AABB.h"
#include "SympConv/Plane.h"
#include "SympConv/Box.h"
#include "SympConv/Sphere.h"
#include "SympConv/Quaternion.h"
#include "TestUtils.h"

namespace SympConvTest {

using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;
using AABB = SympConv::AABB;
using Plane = SympConv::Plane;
using Box = SympConv::Box;
using Sphere = SympConv::Sphere;
using Capsule = SympConv::Capsule;
using Segment = SympConv::Segment;
using Quaternion = SympConv::Quaternion;

TEST(CollisionDetectionTest, RayIntersectsAABB) {
    // 创建一个AABB
    AABB aabb(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
    
    // 从原点发射向AABB的射线
    Ray ray1(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, 1.0f));
    float tMin1, tMax1;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray1, aabb, tMin1, tMax1));
    EXPECT_TRUE(FloatEqual(tMin1, 1.0f));
    EXPECT_TRUE(FloatEqual(tMax1, 3.0f));
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(tMin1);
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, -1.0f)); // 应该在AABB的后表面
    
    // 从AABB内部发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float tMin2, tMax2;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray2, aabb, tMin2, tMax2));
    EXPECT_TRUE(FloatEqual(tMin2, 0.0f));
    EXPECT_TRUE(FloatEqual(tMax2, 1.0f));
    // 验证相交点
    Vec3 intersectPoint2 = ray2.GetPoint(tMax2);
    EXPECT_TRUE(FloatEqual(intersectPoint2.x, 1.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.z, 0.0f)); // 应该在AABB的右表面
    
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
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(t1);
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, 0.0f)); // 应该在平面上
    
    // 平行于平面的射线
    Ray ray2(Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
    float t2;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray2, plane, t2));
    
    // 从平面后面发射的射线（反方向）
    Ray ray3(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray3, plane, t3));
    
    // 非AliasAxis情况：倾斜平面和倾斜射线相交
    Plane inclinedPlane(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
    Ray inclinedRay(Vec3(1.0f, 1.0f, -1.0f), Vec3(-1.0f, -1.0f, 1.0f));
    float t4;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(inclinedRay, inclinedPlane, t4));
    // 验证相交点
    Vec3 intersectPoint4 = inclinedRay.GetPoint(t4);
    // 相交点应该在平面上，满足平面方程：x + y + z = 0
    EXPECT_TRUE(FloatEqual(intersectPoint4.x + intersectPoint4.y + intersectPoint4.z, 0.0f));
    
    // 非AliasAxis情况：倾斜平面和倾斜射线不相交（平行）
    Ray parallelRay(Vec3(1.0f, 1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f));
    float t5;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(parallelRay, inclinedPlane, t5));
    
    // 非AliasAxis情况：倾斜平面和倾斜射线不相交（方向相反）
    Ray oppositeRay(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t6;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(oppositeRay, inclinedPlane, t6));
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
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(t1);
    Vec3 intersectPoint2 = ray1.GetPoint(t2);
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, -1.0f)); // 应该在球体的后表面
    EXPECT_TRUE(FloatEqual(intersectPoint2.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.z, 1.0f)); // 应该在球体的前表面
    // 验证相交点到球心的距离等于半径
    EXPECT_TRUE(FloatEqual((intersectPoint1 - center).Length(), radius));
    EXPECT_TRUE(FloatEqual((intersectPoint2 - center).Length(), radius));
    
    // 不相交的射线
    Ray ray2(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3, t4;
    EXPECT_FALSE(SympConv::RayIntersectsSphere(ray2, center, radius, t3, t4));
}

TEST(CollisionDetectionTest, RayIntersectsTriangle) {
    // 1. 三角形在z=0平面上
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
    // 验证相交点
    Vec3 intersectPoint = ray1.GetPoint(t);
    EXPECT_TRUE(FloatEqual(intersectPoint.x, 0.3f));
    EXPECT_TRUE(FloatEqual(intersectPoint.y, 0.3f));
    EXPECT_TRUE(FloatEqual(intersectPoint.z, 0.0f)); // 应该在z=0平面上
    // 验证相交点是否在三角形内部（通过重心坐标）
    EXPECT_TRUE(u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f);
    
    // 从三角形外部发射的射线
    Ray ray2(Vec3(2.0f, 2.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t2, u2, v2_;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(ray2, v0, v1, v2, t2, u2, v2_));
    
    // 2. 三角形在x=0平面上
    Vec3 v3(0.0f, 0.0f, 0.0f);
    Vec3 v4(0.0f, 1.0f, 0.0f);
    Vec3 v5(0.0f, 0.0f, 1.0f);
    Ray ray3(Vec3(-1.0f, 0.3f, 0.3f), Vec3(1.0f, 0.0f, 0.0f));
    float t3, u3, v3_;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray3, v3, v4, v5, t3, u3, v3_));
    
    // 3. 三角形在y=0平面上
    Vec3 v6(0.0f, 0.0f, 0.0f);
    Vec3 v7(1.0f, 0.0f, 0.0f);
    Vec3 v8(0.0f, 0.0f, 1.0f);
    Ray ray4(Vec3(0.3f, -1.0f, 0.3f), Vec3(0.0f, 1.0f, 0.0f));
    float t4, u4, v4_;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray4, v6, v7, v8, t4, u4, v4_));
    
    // 4. 非AliasAxis的三角形
    Vec3 v9(0.0f, 0.0f, 0.0f);
    Vec3 v10(1.0f, 1.0f, 0.0f);
    Vec3 v11(0.0f, 1.0f, 1.0f);
    Ray ray5(Vec3(0.3f, 0.3f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t5, u5, v5_;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray5, v9, v10, v11, t5, u5, v5_));
    
    // 5. 三角形和射线平行的情况
    Vec3 v12(0.0f, 0.0f, 0.0f);
    Vec3 v13(1.0f, 0.0f, 0.0f);
    Vec3 v14(0.0f, 1.0f, 0.0f);
    Ray ray6(Vec3(0.5f, 0.5f, 1.0f), Vec3(1.0f, 1.0f, 0.0f)); // 平行于z=0平面
    float t6, u6, v6_;
    EXPECT_FALSE(SympConv::RayIntersectsTriangle(ray6, v12, v13, v14, t6, u6, v6_));
    
    // 6. 细长的三角形
    Vec3 v15(0.0f, 0.0f, 0.0f);
    Vec3 v16(10.0f, 0.1f, 0.0f);
    Vec3 v17(0.0f, 0.1f, 0.0f);
    Ray ray7(Vec3(5.0f, 0.05f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t7, u7, v7_;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray7, v15, v16, v17, t7, u7, v7_));
    
    // 7. 非AliasAxis的射线
    Vec3 v18(0.0f, 0.0f, 0.0f);
    Vec3 v19(1.0f, 0.0f, 0.0f);
    Vec3 v20(0.0f, 1.0f, 0.0f);
    Ray ray8(Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, -1.0f)); // 斜向射线
    float t8, u8, v8_;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray8, v18, v19, v20, t8, u8, v8_));
    
    // 8. 很长的射线
    Vec3 v21(0.0f, 0.0f, 0.0f);
    Vec3 v22(1.0f, 0.0f, 0.0f);
    Vec3 v23(0.0f, 1.0f, 0.0f);
    Ray ray9(Vec3(0.5f, 0.5f, -1000.0f), Vec3(0.0f, 0.0f, 1.0f)); // 长射线
    float t9, u9, v9_;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray9, v21, v22, v23, t9, u9, v9_));
    
    // 9. 射线和三角形都不是AliasAxis的情况
    Vec3 v24(1.0f, 1.0f, 1.0f);
    Vec3 v25(2.0f, 2.0f, 1.0f);
    Vec3 v26(1.0f, 2.0f, 2.0f);
    Ray ray10(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)); // 斜向射线
    float t10, u10, v10_;
    EXPECT_TRUE(SympConv::RayIntersectsTriangle(ray10, v24, v25, v26, t10, u10, v10_));
}

TEST(CollisionDetectionTest, RayIntersectsSegment) {
    // 1. 线段在z=0平面上
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(1.0f, 1.0f, 0.0f);
    Segment segment(start, end);
    
    // 从(0.5, 0.5, -1)向z轴正方向发射的射线
    Ray ray1(Vec3(0.5f, 0.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t, s;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray1, segment, t, s));
    EXPECT_TRUE(FloatEqual(t, 1.0f));
    EXPECT_TRUE(FloatEqual(s, 0.5f));
    // 验证相交点
    Vec3 intersectPoint = ray1.GetPoint(t);
    EXPECT_TRUE(FloatEqual(intersectPoint.x, 0.5f));
    EXPECT_TRUE(FloatEqual(intersectPoint.y, 0.5f));
    EXPECT_TRUE(FloatEqual(intersectPoint.z, 0.0f)); // 应该在线段上
    // 验证相交点是否在线段上（通过参数s）
    EXPECT_TRUE(s >= 0.0f && s <= 1.0f);
    
    // 不相交的射线
    Ray ray2(Vec3(2.0f, 2.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t2, s2;
    EXPECT_FALSE(SympConv::RayIntersectsSegment(ray2, segment, t2, s2));
    
    // 2. 线段在x=0平面上
    Vec3 start2(0.0f, 0.0f, 0.0f);
    Vec3 end2(0.0f, 1.0f, 1.0f);
    Segment segment2(start2, end2);
    Ray ray3(Vec3(-1.0f, 0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f));
    float t3, s3;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray3, segment2, t3, s3));
    
    // 3. 线段在y=0平面上
    Vec3 start3(0.0f, 0.0f, 0.0f);
    Vec3 end3(1.0f, 0.0f, 1.0f);
    Segment segment3(start3, end3);
    Ray ray4(Vec3(0.5f, -1.0f, 0.5f), Vec3(0.0f, 1.0f, 0.0f));
    float t4, s4;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray4, segment3, t4, s4));
    
    // 4. 非AliasAxis的线段
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(1.0f, 1.0f, 1.0f);
    Segment segment4(start4, end4);
    Ray ray5(Vec3(0.5f, 0.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 调整射线起点，确保与线段相交
    float t5, s5;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray5, segment4, t5, s5));
    
    // 5. 线段和射线平行的情况
    Vec3 start5(0.0f, 0.0f, 0.0f);
    Vec3 end5(1.0f, 1.0f, 0.0f);
    Segment segment5(start5, end5);
    Ray ray6(Vec3(2.0f, 2.0f, 1.0f), Vec3(1.0f, 1.0f, 0.0f)); // 平行于线段
    float t6, s6;
    EXPECT_FALSE(SympConv::RayIntersectsSegment(ray6, segment5, t6, s6));
    
    // 6. 细长的线段
    Vec3 start6(0.0f, 0.0f, 0.0f);
    Vec3 end6(10.0f, 0.1f, 0.0f);
    Segment segment6(start6, end6);
    Ray ray7(Vec3(5.0f, 0.05f, -1.0f), Vec3(0.0f, 0.0f, 1.0f));
    float t7, s7;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray7, segment6, t7, s7));
    
    // 7. 非AliasAxis的射线
    Vec3 start7(0.0f, 0.0f, 0.0f);
    Vec3 end7(1.0f, 1.0f, 0.0f);
    Segment segment7(start7, end7);
    Ray ray8(Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, -1.0f)); // 斜向射线
    float t8, s8;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray8, segment7, t8, s8));
    
    // 8. 很长的射线
    Vec3 start8(0.0f, 0.0f, 0.0f);
    Vec3 end8(1.0f, 1.0f, 0.0f);
    Segment segment8(start8, end8);
    Ray ray9(Vec3(0.5f, 0.5f, -1000.0f), Vec3(0.0f, 0.0f, 1.0f)); // 长射线
    float t9, s9;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray9, segment8, t9, s9));
    
    // 9. 射线和线段都不是AliasAxis的情况
    Vec3 start9(1.0f, 1.0f, 0.0f);
    Vec3 end9(2.0f, 2.0f, 0.0f);
    Segment segment9(start9, end9);
    Ray ray10(Vec3(1.5f, 1.5f, -1.0f), Vec3(0.0f, 0.0f, 1.0f)); // 调整线段和射线，确保相交
    float t10, s10;
    EXPECT_TRUE(SympConv::RayIntersectsSegment(ray10, segment9, t10, s10));
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
    // 验证相交点
    Vec3 intersectPoint1 = ray1.GetPoint(t1);
    // 盒子的范围是从(-1,-1,-1)到(1,1,1)，所以相交点应该在z=-1处
    EXPECT_TRUE(FloatEqual(intersectPoint1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint1.z, -1.0f));
    
    // 从盒子后面发射的射线
    Ray ray2(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, -1.0f));
    float t2;
    EXPECT_TRUE(SympConv::RayIntersectsBox(ray2, box, t2));
    // 验证相交点
    Vec3 intersectPoint2 = ray2.GetPoint(t2);
    // 相交点应该在z=1处
    EXPECT_TRUE(FloatEqual(intersectPoint2.x, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(intersectPoint2.z, 1.0f));
    
    // 不相交的射线
    Ray ray3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsBox(ray3, box, t3));
    
    // 非AliasAxis情况：射线与旋转的盒子相交
    Box rotatedBox(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    Ray ray4(Vec3(2.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
    float t4;
    EXPECT_TRUE(SympConv::RayIntersectsBox(ray4, rotatedBox, t4));
    
    // 非AliasAxis情况：射线与旋转的盒子不相交
    Ray ray5(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    float t5;
    EXPECT_FALSE(SympConv::RayIntersectsBox(ray5, rotatedBox, t5));
}

TEST(CollisionDetectionTest, BoxIntersectsBox) {
    // 创建两个相交的盒子
    Box box1(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Box box2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::BoxIntersectsBox(box1, box2));
    
    // 创建两个不相交的盒子
    Box box3(Vec3(2.0f, 2.0f, 2.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(SympConv::BoxIntersectsBox(box1, box3));
    
    // 非AliasAxis情况：两个旋转的盒子相交
    Box rotatedBox1(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    Box rotatedBox2(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.7071f, 0.0f, 0.7071f)); // 绕x轴旋转45度
    EXPECT_TRUE(SympConv::BoxIntersectsBox(rotatedBox1, rotatedBox2));
    
    // 非AliasAxis情况：两个旋转的盒子不相交
    Box rotatedBox3(Vec3(3.0f, 3.0f, 3.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    EXPECT_FALSE(SympConv::BoxIntersectsBox(rotatedBox1, rotatedBox3));
}

TEST(CollisionDetectionTest, SphereIntersectsBox) {
    // 创建一个球体和一个盒子
    Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f);
    Box box(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::SphereIntersectsBox(sphere, box));
    
    // 创建一个不相交的球体和盒子
    Sphere sphere2(Vec3(3.0f, 0.0f, 0.0f), 1.0f);
    EXPECT_FALSE(SympConv::SphereIntersectsBox(sphere2, box));
    
    // 非AliasAxis情况：球体与旋转的盒子相交
    Box rotatedBox(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    Sphere sphere3(Vec3(1.0f, 0.0f, 0.0f), 0.5f);
    EXPECT_TRUE(SympConv::SphereIntersectsBox(sphere3, rotatedBox));
    
    // 非AliasAxis情况：球体与旋转的盒子不相交
    Sphere sphere4(Vec3(2.0f, 2.0f, 2.0f), 0.5f);
    EXPECT_FALSE(SympConv::SphereIntersectsBox(sphere4, rotatedBox));
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
    
    // 非AliasAxis情况：射线与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.2f; // 减小半径，使胶囊体更细
    Capsule capsule4(start4, end4, radius4);
    
    Ray ray4(Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
    float t4;
    EXPECT_TRUE(SympConv::RayIntersectsCapsule(ray4, capsule4, t4));
    
    // 非AliasAxis情况：射线与倾斜的胶囊体不相交
    Ray ray5(Vec3(5.0f, 5.0f, 5.0f), Vec3(1.0f, 0.0f, 0.0f)); // 方向沿着x轴正方向，远离胶囊体
    float t5;
    EXPECT_FALSE(SympConv::RayIntersectsCapsule(ray5, capsule4, t5));
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
    
    // 非AliasAxis情况：平面与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Plane plane3(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::PlaneIntersectsCapsule(plane3, capsule4));
    
    // 非AliasAxis情况：平面与倾斜的胶囊体不相交
    Plane plane4(Vec3(1.0f, 1.0f, 1.0f), Vec3(3.0f, 3.0f, 3.0f));
    EXPECT_FALSE(SympConv::PlaneIntersectsCapsule(plane4, capsule4));
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
    
    // 非AliasAxis情况：盒子与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Box box3(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(SympConv::BoxIntersectsCapsule(box3, capsule4));
    
    // 非AliasAxis情况：盒子与倾斜的胶囊体不相交
    Box box4(Vec3(3.0f, 3.0f, 3.0f), Vec3(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(SympConv::BoxIntersectsCapsule(box4, capsule4));
    
    // 非AliasAxis情况：旋转的盒子与倾斜的胶囊体相交
    Box rotatedBox(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.7071f, 0.7071f)); // 绕y轴旋转45度
    EXPECT_TRUE(SympConv::BoxIntersectsCapsule(rotatedBox, capsule4));
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
    
    // 非AliasAxis情况：球体与倾斜的胶囊体相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 2.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Sphere sphere3(Vec3(1.0f, 1.0f, 1.0f), 0.3f);
    EXPECT_TRUE(SympConv::SphereIntersectsCapsule(sphere3, capsule4));
    
    // 非AliasAxis情况：球体与倾斜的胶囊体不相交
    Sphere sphere4(Vec3(3.0f, 3.0f, 3.0f), 0.3f);
    EXPECT_FALSE(SympConv::SphereIntersectsCapsule(sphere4, capsule4));
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
    
    // 非AliasAxis情况：两个胶囊体在x-y平面内相交
    Vec3 start4(0.0f, 0.0f, 0.0f);
    Vec3 end4(2.0f, 2.0f, 0.0f);
    float radius4 = 0.5f;
    Capsule capsule4(start4, end4, radius4);
    
    Vec3 start5(2.0f, 0.0f, 0.0f);
    Vec3 end5(0.0f, 2.0f, 0.0f);
    float radius5 = 0.5f;
    Capsule capsule5(start5, end5, radius5);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule4, capsule5));
    
    // 非AliasAxis情况：两个胶囊体在空间中交叉
    Vec3 start6(0.0f, 0.0f, 0.0f);
    Vec3 end6(2.0f, 2.0f, 2.0f);
    float radius6 = 0.5f;
    Capsule capsule6(start6, end6, radius6);
    
    Vec3 start7(2.0f, 0.0f, 0.0f);
    Vec3 end7(0.0f, 2.0f, 2.0f);
    float radius7 = 0.5f;
    Capsule capsule7(start7, end7, radius7);
    
    EXPECT_TRUE(SympConv::CapsuleIntersectsCapsule(capsule6, capsule7));
    
    // 非AliasAxis情况：两个胶囊体倾斜不相交
    Vec3 start8(0.0f, 0.0f, 0.0f);
    Vec3 end8(1.0f, 1.0f, 1.0f);
    float radius8 = 0.3f;
    Capsule capsule8(start8, end8, radius8);
    
    Vec3 start9(2.0f, 2.0f, 2.0f);
    Vec3 end9(3.0f, 3.0f, 3.0f);
    float radius9 = 0.3f;
    Capsule capsule9(start9, end9, radius9);
    
    EXPECT_FALSE(SympConv::CapsuleIntersectsCapsule(capsule8, capsule9));
}

} // namespace SympConvTest