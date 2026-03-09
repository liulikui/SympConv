#include <gtest/gtest.h>
#include "SympConv/Ray.h"
#include "SympConv/Vector.h"
#include "SympConv/AABB.h"
#include "SympConv/Transform.h"
#include "TestUtils.h"

namespace SympConvTest {

using Ray = SympConv::TRay<float>;
using Vec3 = SympConv::TVector3<float>;
using Transform = SympConv::TTransform<float>;

TEST(RayTest, DefaultConstructor) {
    Ray defaultRay;
    EXPECT_TRUE(FloatEqual(defaultRay.mOrigin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(defaultRay.mDirection.z, 1.0f));
}

TEST(RayTest, ParameterizedConstructor) {
    Ray ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(FloatEqual(ray.mOrigin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(ray.mDirection.x, 1.0f));
}

TEST(RayTest, GetPoint) {
    Ray ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    Vec3 point = ray.GetPoint(5.0f);
    EXPECT_TRUE(FloatEqual(point.x, 5.0f));
    EXPECT_TRUE(FloatEqual(point.y, 0.0f));
    EXPECT_TRUE(FloatEqual(point.z, 0.0f));
}

TEST(RayTest, DistanceTo) {
    Ray ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    float distance = ray.DistanceTo(Vec3(0.0f, 1.0f, 0.0f));
    EXPECT_TRUE(FloatEqual(distance, 1.0f));
}

TEST(RayTest, Transform) {
    Ray ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    Transform transform(Vec3(1.0f, 1.0f, 1.0f));
    Ray transformedRay = ray.Transform(transform);
    EXPECT_TRUE(FloatEqual(transformedRay.mOrigin.x, 1.0f));
    EXPECT_TRUE(FloatEqual(transformedRay.mDirection.x, 1.0f));
}

TEST(RayTest, InverseTransform) {
    Ray ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    Transform transform(Vec3(1.0f, 1.0f, 1.0f));
    Ray transformedRay = ray.Transform(transform);
    Ray inverseTransformedRay = transformedRay.InverseTransform(transform);
    EXPECT_TRUE(FloatEqual(inverseTransformedRay.mOrigin.x, 0.0f));
    EXPECT_TRUE(FloatEqual(inverseTransformedRay.mDirection.x, 1.0f));
}

} // namespace SympConvTest
