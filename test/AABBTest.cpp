#include <gtest/gtest.h>
#include "SympConv/AABB.h"
#include "SympConv/Vector.h"
#include "SympConv/Ray.h"
#include "SympConv/Transform.h"
#include "SympConv/CollisionDetection.h"
#include "SympConv/Quaternion.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using AABB = SympConv::AABB;
using Vec3 = SympConv::Vector3;
using Ray = SympConv::Ray;
using Transform = SympConv::Transform;

TEST(AABBTest, DefaultConstructor) {
    AABB emptyAABB;
    EXPECT_TRUE(emptyAABB.IsEmpty());
}

TEST(AABBTest, ParameterizedConstructor) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    EXPECT_TRUE(FloatEqual(aabb.mMin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, Real(1.0)));
}

TEST(AABBTest, PointConstructor) {
    AABB pointAABB(Vec3(Real(2.0), Real(3.0), Real(4.0)));
    EXPECT_TRUE(FloatEqual(pointAABB.mMin.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(pointAABB.mMax.x, Real(2.0)));
}

TEST(AABBTest, TriangleConstructor) {
    AABB triangleAABB(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(2.0), Real(1.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(triangleAABB.mMin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(triangleAABB.mMax.x, Real(2.0)));
}

TEST(AABBTest, Reset) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    aabb.Reset();
    EXPECT_TRUE(aabb.IsEmpty());
}

TEST(AABBTest, ExpandPoint) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    aabb.Expand(Vec3(Real(2.0), Real(2.0), Real(2.0)));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, Real(2.0)));
}

TEST(AABBTest, ExpandAABB) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB aabb2(Vec3(Real(0.5), Real(0.5), Real(0.5)), Vec3(Real(1.5), Real(1.5), Real(1.5)));
    aabb.Expand(aabb2);
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, Real(1.5)));
}

TEST(AABBTest, ExpandSphere) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    aabb.Expand(Vec3(Real(3.0), Real(3.0), Real(3.0)), Real(1.0));
    EXPECT_TRUE(FloatEqual(aabb.mMax.x, Real(4.0)));
}

TEST(AABBTest, GetCenter) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Vec3 center = aabb.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(center.y, Real(0.5)));
    EXPECT_TRUE(FloatEqual(center.z, Real(0.5)));
}

TEST(AABBTest, GetSize) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Vec3 size = aabb.GetSize();
    EXPECT_TRUE(FloatEqual(size.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(size.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(size.z, Real(1.0)));
}

TEST(AABBTest, GetRadius) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Real radius = aabb.GetRadius();
    EXPECT_TRUE(FloatEqual(radius, std::sqrt(Real(3.0)) / Real(2.0)));
}

TEST(AABBTest, GetSurfaceArea) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Real surfaceArea = aabb.GetSurfaceArea();
    EXPECT_TRUE(FloatEqual(surfaceArea, Real(6.0)));
}

TEST(AABBTest, GetVolume) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Real volume = aabb.GetVolume();
    EXPECT_TRUE(FloatEqual(volume, Real(1.0)));
}

TEST(AABBTest, ContainsPoint) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    EXPECT_TRUE(aabb.Contains(Vec3(Real(0.5), Real(0.5), Real(0.5))));
    EXPECT_FALSE(aabb.Contains(Vec3(Real(2.0), Real(2.0), Real(2.0))));
}

TEST(AABBTest, ContainsAABB) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB smallAABB(Vec3(Real(0.25), Real(0.25), Real(0.25)), Vec3(Real(0.75), Real(0.75), Real(0.75)));
    EXPECT_TRUE(aabb.Contains(smallAABB));
}

TEST(AABBTest, IntersectsAABB) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB aabb2(Vec3(Real(0.5), Real(0.5), Real(0.5)), Vec3(Real(1.5), Real(1.5), Real(1.5)));
    EXPECT_TRUE(aabb.Intersects(aabb2));
}

TEST(AABBTest, IntersectsRay) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Ray ray(Vec3(Real(-1.0), Real(0.5), Real(0.5)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray, aabb));
}

TEST(AABBTest, IntersectsRayWithParams) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Ray ray(Vec3(Real(-1.0), Real(0.5), Real(0.5)), Vec3(Real(1.0), Real(0.0), Real(0.0)));
    Real tMin, tMax;
    EXPECT_TRUE(SympConv::RayIntersectsAABB(ray, aabb, tMin, tMax));
    EXPECT_TRUE(FloatEqual(tMin, Real(1.0)));
    EXPECT_TRUE(FloatEqual(tMax, Real(2.0)));
}

TEST(AABBTest, Intersect) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB aabb2(Vec3(Real(0.5), Real(0.5), Real(0.5)), Vec3(Real(1.5), Real(1.5), Real(1.5)));
    AABB intersection = aabb.Intersect(aabb2);
    EXPECT_TRUE(FloatEqual(intersection.mMin.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(intersection.mMax.x, Real(1.0)));
}

TEST(AABBTest, Union) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB aabb2(Vec3(Real(0.5), Real(0.5), Real(0.5)), Vec3(Real(1.5), Real(1.5), Real(1.5)));
    AABB unionAABB = aabb.Union(aabb2);
    EXPECT_TRUE(FloatEqual(unionAABB.mMin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(unionAABB.mMax.x, Real(1.5)));
}

TEST(AABBTest, Transform) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Transform transform(Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB transformedAABB = aabb.Transform(transform);
    EXPECT_TRUE(FloatEqual(transformedAABB.mMin.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(transformedAABB.mMax.x, Real(2.0)));
}

TEST(AABBTest, DistanceTo) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Real distanceInside = aabb.DistanceTo(Vec3(Real(0.5), Real(0.5), Real(0.5)));
    EXPECT_TRUE(FloatEqual(distanceInside, Real(0.0)));
    Real distanceOutside = aabb.DistanceTo(Vec3(Real(2.0), Real(2.0), Real(2.0)));
    EXPECT_TRUE(FloatEqual(distanceOutside, std::sqrt(Real(3.0))));
}

TEST(AABBTest, Scale) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB scaledAABB = aabb.Scale(Real(2.0));
    EXPECT_TRUE(FloatEqual(scaledAABB.mMin.x, Real(-0.5)));
    EXPECT_TRUE(FloatEqual(scaledAABB.mMax.x, Real(1.5)));
}

TEST(AABBTest, Translate) {
    AABB aabb(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB translatedAABB = aabb.Translate(Vec3(Real(1.0), Real(2.0), Real(3.0)));
    EXPECT_TRUE(FloatEqual(translatedAABB.mMin.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(translatedAABB.mMax.x, Real(2.0)));
}

TEST(AABBTest, FromPoints) {
    Vec3 points[] = {Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(2.0), Real(3.0)), Vec3(Real(2.0), Real(1.0), Real(0.0))};
    AABB fromPointsAABB = AABB::FromPoints(points, 3);
    EXPECT_TRUE(FloatEqual(fromPointsAABB.mMin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(fromPointsAABB.mMax.x, Real(2.0)));
}

TEST(AABBTest, FromSphere) {
    AABB fromSphereAABB = AABB::FromSphere(Vec3(Real(1.0), Real(1.0), Real(1.0)), Real(1.0));
    EXPECT_TRUE(FloatEqual(fromSphereAABB.mMin.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(fromSphereAABB.mMax.x, Real(2.0)));
}

TEST(AABBTest, Union_ObliqueAABB) {
    // 测试斜向几何生成的AABB合并
    // 创建一个旋转后的AABB
    AABB originalAABB(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    Transform rotationTransform;
    rotationTransform.mOrientation = SympConv::Quaternion::FromEulerAngles(Real(0.0), Real(M_PI / 4.0), Real(0.0));
    AABB rotatedAABB = originalAABB.Transform(rotationTransform);
    
    // 创建另一个旋转后的AABB
    AABB originalAABB2(Vec3(Real(2.0), Real(2.0), Real(2.0)), Vec3(Real(3.0), Real(3.0), Real(3.0)));
    Transform rotationTransform2;
    rotationTransform2.mOrientation = SympConv::Quaternion::FromEulerAngles(Real(0.0), Real(M_PI / 3.0), Real(0.0));
    AABB rotatedAABB2 = originalAABB2.Transform(rotationTransform2);
    
    // 合并两个旋转后的AABB
    AABB unionAABB = rotatedAABB.Union(rotatedAABB2);
    
    // 验证合并结果
    EXPECT_TRUE(unionAABB.Contains(rotatedAABB));
    EXPECT_TRUE(unionAABB.Contains(rotatedAABB2));
}

TEST(AABBTest, EmptyAABB) {
    // 测试空AABB（min坐标 > max坐标）
    AABB emptyAABB(Vec3(Real(1.0), Real(1.0), Real(1.0)), Vec3(Real(0.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(emptyAABB.IsEmpty());
    
    // 测试空AABB与其他AABB的合并
    AABB normalAABB(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    AABB unionWithEmpty = emptyAABB.Union(normalAABB);
    EXPECT_TRUE(unionWithEmpty.Contains(normalAABB));
    
    // 测试空AABB的相交
    EXPECT_FALSE(emptyAABB.Intersects(normalAABB));
}

TEST(AABBTest, ZeroVolumeAABB) {
    // 测试零体积AABB（min=max，单点AABB）
    AABB zeroVolumeAABB(Vec3(Real(1.0), Real(1.0), Real(1.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    
    // 测试是否包含自身点
    EXPECT_TRUE(zeroVolumeAABB.Contains(Vec3(Real(1.0), Real(1.0), Real(1.0))));
    
    // 测试与其他AABB的相交
    AABB normalAABB(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(2.0), Real(2.0), Real(2.0)));
    EXPECT_TRUE(zeroVolumeAABB.Intersects(normalAABB));
    
    // 测试与点的距离
    EXPECT_TRUE(FloatEqual(zeroVolumeAABB.DistanceTo(Vec3(Real(1.0), Real(1.0), Real(1.0))), Real(0.0)));
}

TEST(AABBTest, Transform_RotationAndScaling) {
    // 测试旋转 + 非均匀缩放后的AABB计算
    AABB originalAABB(Vec3(Real(0.0), Real(0.0), Real(0.0)), Vec3(Real(1.0), Real(1.0), Real(1.0)));
    
    // 创建一个包含旋转和非均匀缩放的变换
    Transform transform;
    transform.mOrientation = SympConv::Quaternion::FromEulerAngles(Real(M_PI / 4.0), Real(M_PI / 4.0), Real(0.0));
    transform.mScale = Vec3(Real(2.0), Real(1.0), Real(0.5));
    
    AABB transformedAABB = originalAABB.Transform(transform);
    
    // 验证变换后的AABB是轴对齐的
    EXPECT_TRUE(transformedAABB.mMin.x <= transformedAABB.mMax.x);
    EXPECT_TRUE(transformedAABB.mMin.y <= transformedAABB.mMax.y);
    EXPECT_TRUE(transformedAABB.mMin.z <= transformedAABB.mMax.z);
    
    // 验证变换后的AABB包含原始AABB的所有顶点变换后的位置
    Vec3 vertices[] = {
        Vec3(Real(0.0), Real(0.0), Real(0.0)),
        Vec3(Real(1.0), Real(0.0), Real(0.0)),
        Vec3(Real(0.0), Real(1.0), Real(0.0)),
        Vec3(Real(1.0), Real(1.0), Real(0.0)),
        Vec3(Real(0.0), Real(0.0), Real(1.0)),
        Vec3(Real(1.0), Real(0.0), Real(1.0)),
        Vec3(Real(0.0), Real(1.0), Real(1.0)),
        Vec3(Real(1.0), Real(1.0), Real(1.0))
    };
    
    for (int i = 0; i < 8; i++) {
        Vec3 transformedVertex = transform.TransformPoint(vertices[i]);
        EXPECT_TRUE(transformedAABB.Contains(transformedVertex));
    }
}

} // namespace SympConvTest