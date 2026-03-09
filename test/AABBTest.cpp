#include <gtest/gtest.h>
#include "SympConv/AABB.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/Transform.h"
#include "SympConv/CollisionDetection.h"
#include "TestUtils.h"

namespace SympConvTest {

using AABB = SympConv::AABB;
using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;
using Transform = SympConv::Transform;

TEST(AABBTest, DefaultConstructor) {
    AABB emptyAABB;
    EXPECT_TRUE(emptyAABB.IsEmpty());
}

TEST(AABBTest, ParameterizedConstructor) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 1.0f));
}

TEST(AABBTest, PointConstructor) {
    AABB pointAABB(Vec3(2.0f, 3.0f, 4.0f));
    EXPECT_TRUE(FloatEqual(pointAABB.mMin.x, 2.0f));
    EXPECT_TRUE(FloatEqual(pointAABB.mMax.x, 2.0f));
}

TEST(AABBTest, TriangleConstructor) {
    AABB triangleAABB(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 2.0f, 3.0f), Vec3(2.0f, 1.0f, 0.0f));
    EXPECT_TRUE(FloatEqual(triangleAABB.mMin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(triangleAABB.mMax.x, 2.0f));
}

TEST(AABBTest, Reset) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    aabb.Reset();
    EXPECT_TRUE(aabb.IsEmpty());
}

TEST(AABBTest, ExpandPoint) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    aabb.Expand(Vec3(2.0f, 2.0f, 2.0f));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 2.0f));
}

TEST(AABBTest, ExpandAABB) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB aabb2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.5f, 1.5f, 1.5f));
    aabb.Expand(aabb2);
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 1.5f));
}

TEST(AABBTest, ExpandSphere) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    aabb.Expand(Vec3(3.0f, 3.0f, 3.0f), 1.0f);
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, 4.0f));
}

TEST(AABBTest, GetCenter) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Vec3 center = aabb.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 0.5f));
    EXPECT_TRUE(FloatEqual(center.y, 0.5f));
    EXPECT_TRUE(FloatEqual(center.z, 0.5f));
}

TEST(AABBTest, GetSize) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Vec3 size = aabb.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, 1.0f));
    EXPECT_TRUE(FloatEqual(size.y, 1.0f));
    EXPECT_TRUE(FloatEqual(size.z, 1.0f));
}

TEST(AABBTest, GetRadius) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    float radius = aabb.GetRadius();
    EXPECT_TRUE(FloatEqual(radius, std::sqrt(3.0f) / 2.0f));
}

TEST(AABBTest, GetSurfaceArea) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    float surfaceArea = aabb.GetSurfaceArea();
    EXPECT_TRUE(FloatEqual(surfaceArea, 6.0f));
}

TEST(AABBTest, GetVolume) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    float volume = aabb.GetVolume();
    EXPECT_TRUE(FloatEqual(volume, 1.0f));
}

TEST(AABBTest, ContainsPoint) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    EXPECT_TRUE(aabb.Contains(Vec3(0.5f, 0.5f, 0.5f)));
    EXPECT_FALSE(aabb.Contains(Vec3(2.0f, 2.0f, 2.0f)));
}

TEST(AABBTest, ContainsAABB) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB smallAABB(Vec3(0.25f, 0.25f, 0.25f), Vec3(0.75f, 0.75f, 0.75f));
    EXPECT_TRUE(aabb.Contains(smallAABB));
}

TEST(AABBTest, IntersectsAABB) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB aabb2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.5f, 1.5f, 1.5f));
    EXPECT_TRUE(aabb.Intersects(aabb2));
}

TEST(AABBTest, IntersectsRay) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Ray ray(Vec3(-1.0f, 0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray, aabb));
}

TEST(AABBTest, IntersectsRayWithParams) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Ray ray(Vec3(-1.0f, 0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f));
    float tMin, tMax;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray, aabb, tMin, tMax));
    EXPECT_TRUE(FloatEqual(tMin, 1.0f));
    EXPECT_TRUE(FloatEqual(tMax, 2.0f));
}

TEST(AABBTest, Intersect) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB aabb2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.5f, 1.5f, 1.5f));
    AABB intersection = aabb.Intersect(aabb2);
    EXPECT_TRUE(FloatEqual(intersection.mMin.x, 0.5f));
    EXPECT_TRUE(FloatEqual(intersection.mMax.x, 1.0f));
}

TEST(AABBTest, Union) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB aabb2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.5f, 1.5f, 1.5f));
    AABB unionAABB = aabb.Union(aabb2);
    EXPECT_TRUE(FloatEqual(unionAABB.mMin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(unionAABB.mMax.x, 1.5f));
}

TEST(AABBTest, Transform) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    Transform transform(Vec3(1.0f, 1.0f, 1.0f));
    AABB transformedAABB = aabb.Transform(transform);
    EXPECT_TRUE(FloatEqual(transformedAABB.mMin.x, 1.0f));
    EXPECT_TRUE(FloatEqual(transformedAABB.mMax.x, 2.0f));
}

TEST(AABBTest, DistanceTo) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    float distanceInside = aabb.DistanceTo(Vec3(0.5f, 0.5f, 0.5f));
    EXPECT_TRUE(FloatEqual(distanceInside, 0.0f));
    float distanceOutside = aabb.DistanceTo(Vec3(2.0f, 2.0f, 2.0f));
    EXPECT_TRUE(FloatEqual(distanceOutside, std::sqrt(3.0f)));
}

TEST(AABBTest, Scale) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB scaledAABB = aabb.Scale(2.0f);
    EXPECT_TRUE(FloatEqual(scaledAABB.mMin.x, -0.5f));
    EXPECT_TRUE(FloatEqual(scaledAABB.mMax.x, 1.5f));
}

TEST(AABBTest, Translate) {
    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
    AABB translatedAABB = aabb.Translate(Vec3(1.0f, 2.0f, 3.0f));
    EXPECT_TRUE(FloatEqual(translatedAABB.mMin.x, 1.0f));
    EXPECT_TRUE(FloatEqual(translatedAABB.mMax.x, 2.0f));
}

TEST(AABBTest, FromPoints) {
    Vec3 points[] = {Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 2.0f, 3.0f), Vec3(2.0f, 1.0f, 0.0f)};
    AABB fromPointsAABB = AABB::FromPoints(points, 3);
    EXPECT_TRUE(FloatEqual(fromPointsAABB.mMin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(fromPointsAABB.mMax.x, 2.0f));
}

TEST(AABBTest, FromSphere) {
    AABB fromSphereAABB = AABB::FromSphere(Vec3(1.0f, 1.0f, 1.0f), 1.0f);
    EXPECT_TRUE(FloatEqual(fromSphereAABB.mMin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(fromSphereAABB.mMax.x, 2.0f));
}

} // namespace SympConvTest
