#include <gtest/gtest.h>
#include "SympConv/Plane.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/CollisionDetection.h"
#include "TestUtils.h"

namespace SympConvTest {

using Plane = SympConv::Plane;
using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;

TEST(PlaneTest, DefaultConstructor) {
    Plane plane;
    EXPECT_TRUE(FloatEqual(plane.x, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.y, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.z, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.w, 0.0f));
}

TEST(PlaneTest, ParameterizedConstructor) {
    Plane plane(1.0f, 0.0f, 0.0f, -1.0f);
    EXPECT_TRUE(FloatEqual(plane.x, 1.0f));
    EXPECT_TRUE(FloatEqual(plane.y, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.z, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.w, -1.0f));
}

TEST(PlaneTest, NormalAndPointConstructor) {
    Vec3 normal(0.0f, 1.0f, 0.0f);
    Vec3 point(0.0f, 5.0f, 0.0f);
    Plane plane(normal, point);
    
    EXPECT_TRUE(FloatEqual(plane.x, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.y, 1.0f));
    EXPECT_TRUE(FloatEqual(plane.z, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.w, -5.0f));
}

TEST(PlaneTest, ThreePointsConstructor) {
    Vec3 p1(0.0f, 0.0f, 0.0f);
    Vec3 p2(1.0f, 0.0f, 0.0f);
    Vec3 p3(0.0f, 1.0f, 0.0f);
    Plane plane(p1, p2, p3);
    
    EXPECT_TRUE(FloatEqual(plane.x, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.y, 0.0f));
    EXPECT_TRUE(FloatEqual(plane.z, 1.0f));
    EXPECT_TRUE(FloatEqual(plane.w, 0.0f));
}

TEST(PlaneTest, DistanceTo) {
    Plane plane(0.0f, 1.0f, 0.0f, -5.0f); // y=5平面
    
    Vec3 point1(0.0f, 5.0f, 0.0f); // 在平面上
    EXPECT_TRUE(FloatEqual(plane.DistanceTo(point1), 0.0f));
    
    Vec3 point2(0.0f, 6.0f, 0.0f); // 在平面上方
    EXPECT_TRUE(FloatEqual(plane.DistanceTo(point2), 1.0f));
    
    Vec3 point3(0.0f, 4.0f, 0.0f); // 在平面下方
    EXPECT_TRUE(FloatEqual(plane.DistanceTo(point3), -1.0f));
}

TEST(PlaneTest, Contains) {
    Plane plane(0.0f, 1.0f, 0.0f, -5.0f); // y=5平面
    
    Vec3 point1(0.0f, 5.0f, 0.0f); // 在平面上
    EXPECT_TRUE(plane.Contains(point1));
    
    Vec3 point2(0.0f, 5.000001f, 0.0f); // 接近平面
    EXPECT_TRUE(plane.Contains(point2));
    
    Vec3 point3(0.0f, 6.0f, 0.0f); // 远离平面
    EXPECT_FALSE(plane.Contains(point3));
}

TEST(PlaneTest, GetNormal) {
    Plane plane(1.0f, 2.0f, 3.0f, 4.0f);
    Vec3 normal = plane.GetNormal();
    
    EXPECT_TRUE(FloatEqual(normal.x, 1.0f));
    EXPECT_TRUE(FloatEqual(normal.y, 2.0f));
    EXPECT_TRUE(FloatEqual(normal.z, 3.0f));
}

TEST(PlaneTest, Normalize) {
    Plane plane(2.0f, 0.0f, 0.0f, -2.0f); // 未归一化的平面
    Plane normalized = plane.Normalize();
    
    EXPECT_TRUE(FloatEqual(normalized.x, 1.0f));
    EXPECT_TRUE(FloatEqual(normalized.y, 0.0f));
    EXPECT_TRUE(FloatEqual(normalized.z, 0.0f));
    EXPECT_TRUE(FloatEqual(normalized.w, -1.0f));
}

TEST(PlaneTest, IntersectsRay) {
    Plane plane(0.0f, 1.0f, 0.0f, -5.0f); // y=5平面
    
    // 从下方射向平面的射线
    Ray ray1(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    float t1;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray1, plane, t1));
    EXPECT_TRUE(FloatEqual(t1, 5.0f));
    
    // 从上方射向平面的射线（反方向）
    Ray ray2(Vec3(0.0f, 10.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
    float t2;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray2, plane, t2));
    EXPECT_TRUE(FloatEqual(t2, 5.0f));
    
    // 平行于平面的射线
    Ray ray3(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float t3;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray3, plane, t3));
}



} // namespace SympConvTest