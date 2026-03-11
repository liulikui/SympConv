#include <gtest/gtest.h>
#include "SympConv/Ray.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "SympConv/Transform.h"
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

} // namespace SympConvTest
