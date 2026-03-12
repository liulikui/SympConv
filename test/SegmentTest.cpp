#include <gtest/gtest.h>
#include "SympConv/Segment.h"
#include "SympConv/Vector.h"
#include "SympConv/Transform.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Vec3 = SympConv::Vector3;
using Segment = SympConv::Segment;
using Transform = SympConv::Transform;

TEST(SegmentTest, DefaultConstructor) {
    Segment segment;
    EXPECT_TRUE(FloatEqual(segment.mStart.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(segment.mStart.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(segment.mStart.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(segment.mEnd.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(segment.mEnd.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(segment.mEnd.z, Real(0.0)));
}

TEST(SegmentTest, ConstructorWithPoints) {
    Vec3 start(Real(1.0), Real(2.0), Real(3.0));
    Vec3 end(Real(4.0), Real(5.0), Real(6.0));
    Segment segment(start, end);
    EXPECT_TRUE(FloatEqual(segment.mStart.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(segment.mStart.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(segment.mStart.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(segment.mEnd.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(segment.mEnd.y, Real(5.0)));
    EXPECT_TRUE(FloatEqual(segment.mEnd.z, Real(6.0)));
}

TEST(SegmentTest, GetPoint) {
    Vec3 start(Real(0.0), Real(0.0), Real(0.0));
    Vec3 end(Real(1.0), Real(2.0), Real(3.0));
    Segment segment(start, end);
    
    Vec3 point = segment.GetPoint(Real(0.0));
    EXPECT_TRUE(FloatEqual(point.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(point.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(point.z, Real(0.0)));
    
    point = segment.GetPoint(Real(1.0));
    EXPECT_TRUE(FloatEqual(point.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(point.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(point.z, Real(3.0)));
    
    point = segment.GetPoint(Real(0.5));
    EXPECT_TRUE(FloatEqual(point.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(point.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(point.z, Real(1.5)));
}

TEST(SegmentTest, Length) {
    Vec3 start(Real(0.0), Real(0.0), Real(0.0));
    Vec3 end(Real(3.0), Real(4.0), Real(0.0));
    Segment segment(start, end);
    EXPECT_TRUE(FloatEqual(segment.Length(), Real(5.0)));
}

TEST(SegmentTest, LengthSquared) {
    Vec3 start(Real(0.0), Real(0.0), Real(0.0));
    Vec3 end(Real(3.0), Real(4.0), Real(0.0));
    Segment segment(start, end);
    EXPECT_TRUE(FloatEqual(segment.LengthSquared(), Real(25.0)));
}

TEST(SegmentTest, GetCenter) {
    Vec3 start(Real(0.0), Real(0.0), Real(0.0));
    Vec3 end(Real(2.0), Real(2.0), Real(2.0));
    Segment segment(start, end);
    Vec3 center = segment.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(center.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(center.z, Real(1.0)));
}

TEST(SegmentTest, GetDirection) {
    Vec3 start(Real(0.0), Real(0.0), Real(0.0));
    Vec3 end(Real(1.0), Real(0.0), Real(0.0));
    Segment segment(start, end);
    Vec3 direction = segment.GetDirection();
    EXPECT_TRUE(FloatEqual(direction.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(direction.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(direction.z, Real(0.0)));
}

TEST(SegmentTest, DistanceTo) {
    Vec3 start(Real(0.0), Real(0.0), Real(0.0));
    Vec3 end(Real(2.0), Real(0.0), Real(0.0));
    Segment segment(start, end);
    
    Vec3 point1(Real(1.0), Real(0.0), Real(0.0));
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point1), Real(0.0)));
    
    Vec3 point2(Real(1.0), Real(1.0), Real(0.0));
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point2), Real(1.0)));
    
    Vec3 point3(Real(3.0), Real(0.0), Real(0.0));
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point3), Real(1.0)));
    
    Vec3 point4(Real(-1.0), Real(0.0), Real(0.0));
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point4), Real(1.0)));
}

TEST(SegmentTest, Transform) {
    Vec3 start(Real(0.0), Real(0.0), Real(0.0));
    Vec3 end(Real(1.0), Real(0.0), Real(0.0));
    Segment segment(start, end);
    
    // 创建一个简单的平移变换
    Transform transform;
    transform.mTranslation = Vec3(Real(1.0), Real(2.0), Real(3.0));
    
    Segment transformed = segment.Transform(transform);
    EXPECT_TRUE(FloatEqual(transformed.mStart.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(transformed.mStart.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(transformed.mStart.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(transformed.mEnd.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(transformed.mEnd.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(transformed.mEnd.z, Real(3.0)));
}

TEST(SegmentTest, InverseTransform) {
    Vec3 start(Real(1.0), Real(2.0), Real(3.0));
    Vec3 end(Real(2.0), Real(2.0), Real(3.0));
    Segment segment(start, end);
    
    // 创建一个简单的平移变换
    Transform transform;
    transform.mTranslation = Vec3(Real(1.0), Real(2.0), Real(3.0));
    
    Segment inverseTransformed = segment.InverseTransform(transform);
    EXPECT_TRUE(FloatEqual(inverseTransformed.mStart.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mStart.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mStart.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mEnd.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mEnd.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mEnd.z, Real(0.0)));
}

} // namespace SympConvTest
