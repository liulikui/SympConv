#include <gtest/gtest.h>
#include "SympConv/Plane.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/CollisionDetection.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Plane = SympConv::Plane;
using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;

TEST(PlaneTest, DefaultConstructor) {
    Plane plane;
    EXPECT_TRUE(FloatEqual(plane.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.w, Real(0.0)));
}

TEST(PlaneTest, ParameterizedConstructor) {
    Plane plane(Real(1.0), Real(0.0), Real(0.0), Real(-1.0));
    EXPECT_TRUE(FloatEqual(plane.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(plane.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.w, Real(-1.0)));
}

TEST(PlaneTest, NormalAndPointConstructor) {
    Vec3 normal(Real(0.0), Real(1.0), Real(0.0));
    Vec3 point(Real(0.0), Real(5.0), Real(0.0));
    Plane plane(normal, point);
    
    EXPECT_TRUE(FloatEqual(plane.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(plane.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.w, Real(-5.0)));
}

TEST(PlaneTest, ThreePointsConstructor) {
    Vec3 p1(Real(0.0), Real(0.0), Real(0.0));
    Vec3 p2(Real(1.0), Real(0.0), Real(0.0));
    Vec3 p3(Real(0.0), Real(1.0), Real(0.0));
    Plane plane(p1, p2, p3);
    
    EXPECT_TRUE(FloatEqual(plane.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(plane.z, Real(1.0)));
    EXPECT_TRUE(FloatEqual(plane.w, Real(0.0)));
}

TEST(PlaneTest, DistanceTo) {
    Plane plane(Real(0.0), Real(1.0), Real(0.0), Real(-5.0)); // y=5平面
    
    Vec3 point1(Real(0.0), Real(5.0), Real(0.0)); // 在平面上
    EXPECT_TRUE(FloatEqual(plane.DistanceTo(point1), Real(0.0)));
    
    Vec3 point2(Real(0.0), Real(6.0), Real(0.0)); // 在平面上方
    EXPECT_TRUE(FloatEqual(plane.DistanceTo(point2), Real(1.0)));
    
    Vec3 point3(Real(0.0), Real(4.0), Real(0.0)); // 在平面下方
    EXPECT_TRUE(FloatEqual(plane.DistanceTo(point3), Real(-1.0)));
}

TEST(PlaneTest, Contains) {
    Plane plane(Real(0.0), Real(1.0), Real(0.0), Real(-5.0)); // y=5平面
    
    Vec3 point1(Real(0.0), Real(5.0), Real(0.0)); // 在平面上
    EXPECT_TRUE(plane.Contains(point1));
    
    Vec3 point2(Real(0.0), Real(5.0), Real(0.0)); // 正好在平面上
    EXPECT_TRUE(plane.Contains(point2));
    
    Vec3 point3(Real(0.0), Real(6.0), Real(0.0)); // 远离平面
    EXPECT_FALSE(plane.Contains(point3));
}

TEST(PlaneTest, GetNormal) {
    Plane plane(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Vec3 normal = plane.GetNormal();
    
    EXPECT_TRUE(FloatEqual(normal.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(normal.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(normal.z, Real(3.0)));
}

TEST(PlaneTest, Normalize) {
    Plane plane(Real(2.0), Real(0.0), Real(0.0), Real(-2.0)); // 未归一化的平面
    Plane normalized = plane.Normalize();
    
    EXPECT_TRUE(FloatEqual(normalized.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(normalized.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(normalized.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(normalized.w, Real(-1.0)));
}

TEST(PlaneTest, IntersectsRay) {
    Plane plane(Real(0.0), Real(1.0), Real(0.0), Real(-5.0)); // y=5平面
    
    // 从下方射向平面的射线
    Ray ray1(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(0.0), Real(1.0), Real(0.0)));
    Real t1;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray1, plane, t1));
    EXPECT_TRUE(FloatEqual(t1, Real(5.0)));
    
    // 从上方射向平面的射线（反方向）
    Ray ray2(Vec3(Real(0.0), Real(10.0), Real(0.0)), Vec3(Real(0.0), Real(-1.0), Real(0.0)));
    Real t2;
    EXPECT_TRUE(SympConv::RayIntersectsPlane(ray2, plane, t2));
    EXPECT_TRUE(FloatEqual(t2, Real(5.0)));
    
    // 平行于平面的射线
    Ray ray3(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    Real t3;
    EXPECT_FALSE(SympConv::RayIntersectsPlane(ray3, plane, t3));
}

} // namespace SympConvTest