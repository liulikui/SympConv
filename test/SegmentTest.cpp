#include <gtest/gtest.h>
#include "SympConv/Segment.h"
#include "SympConv/Vector.h"
#include "SympConv/Transform.h"
#include "TestUtils.h"

namespace SympConvTest {

using Vec3 = SympConv::Vector3;
using Segment = SympConv::Segment;
using Transform = SympConv::Transform;

TEST(SegmentTest, DefaultConstructor) {
    Segment segment;
    EXPECT_TRUE(FloatEqual(segment.mStart.x, 0.0f));
    EXPECT_TRUE(FloatEqual(segment.mStart.y, 0.0f));
    EXPECT_TRUE(FloatEqual(segment.mStart.z, 0.0f));
    EXPECT_TRUE(FloatEqual(segment.mEnd.x, 0.0f));
    EXPECT_TRUE(FloatEqual(segment.mEnd.y, 0.0f));
    EXPECT_TRUE(FloatEqual(segment.mEnd.z, 0.0f));
}

TEST(SegmentTest, ConstructorWithPoints) {
    Vec3 start(1.0f, 2.0f, 3.0f);
    Vec3 end(4.0f, 5.0f, 6.0f);
    Segment segment(start, end);
    EXPECT_TRUE(FloatEqual(segment.mStart.x, 1.0f));
    EXPECT_TRUE(FloatEqual(segment.mStart.y, 2.0f));
    EXPECT_TRUE(FloatEqual(segment.mStart.z, 3.0f));
    EXPECT_TRUE(FloatEqual(segment.mEnd.x, 4.0f));
    EXPECT_TRUE(FloatEqual(segment.mEnd.y, 5.0f));
    EXPECT_TRUE(FloatEqual(segment.mEnd.z, 6.0f));
}

TEST(SegmentTest, GetPoint) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(1.0f, 2.0f, 3.0f);
    Segment segment(start, end);
    
    Vec3 point = segment.GetPoint(0.0f);
    EXPECT_TRUE(FloatEqual(point.x, 0.0f));
    EXPECT_TRUE(FloatEqual(point.y, 0.0f));
    EXPECT_TRUE(FloatEqual(point.z, 0.0f));
    
    point = segment.GetPoint(1.0f);
    EXPECT_TRUE(FloatEqual(point.x, 1.0f));
    EXPECT_TRUE(FloatEqual(point.y, 2.0f));
    EXPECT_TRUE(FloatEqual(point.z, 3.0f));
    
    point = segment.GetPoint(0.5f);
    EXPECT_TRUE(FloatEqual(point.x, 0.5f));
    EXPECT_TRUE(FloatEqual(point.y, 1.0f));
    EXPECT_TRUE(FloatEqual(point.z, 1.5f));
}

TEST(SegmentTest, Length) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(3.0f, 4.0f, 0.0f);
    Segment segment(start, end);
    EXPECT_TRUE(FloatEqual(segment.Length(), 5.0f));
}

TEST(SegmentTest, LengthSquared) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(3.0f, 4.0f, 0.0f);
    Segment segment(start, end);
    EXPECT_TRUE(FloatEqual(segment.LengthSquared(), 25.0f));
}

TEST(SegmentTest, GetCenter) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(2.0f, 2.0f, 2.0f);
    Segment segment(start, end);
    Vec3 center = segment.GetCenter();
    EXPECT_TRUE(FloatEqual(center.x, 1.0f));
    EXPECT_TRUE(FloatEqual(center.y, 1.0f));
    EXPECT_TRUE(FloatEqual(center.z, 1.0f));
}

TEST(SegmentTest, GetDirection) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(1.0f, 0.0f, 0.0f);
    Segment segment(start, end);
    Vec3 direction = segment.GetDirection();
    EXPECT_TRUE(FloatEqual(direction.x, 1.0f));
    EXPECT_TRUE(FloatEqual(direction.y, 0.0f));
    EXPECT_TRUE(FloatEqual(direction.z, 0.0f));
}

TEST(SegmentTest, DistanceTo) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(2.0f, 0.0f, 0.0f);
    Segment segment(start, end);
    
    Vec3 point1(1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point1), 0.0f));
    
    Vec3 point2(1.0f, 1.0f, 0.0f);
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point2), 1.0f));
    
    Vec3 point3(3.0f, 0.0f, 0.0f);
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point3), 1.0f));
    
    Vec3 point4(-1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(FloatEqual(segment.DistanceTo(point4), 1.0f));
}

TEST(SegmentTest, Transform) {
    Vec3 start(0.0f, 0.0f, 0.0f);
    Vec3 end(1.0f, 0.0f, 0.0f);
    Segment segment(start, end);
    
    // 创建一个简单的平移变换
    Transform transform;
    transform.mTranslation = Vec3(1.0f, 2.0f, 3.0f);
    
    Segment transformed = segment.Transform(transform);
    EXPECT_TRUE(FloatEqual(transformed.mStart.x, 1.0f));
    EXPECT_TRUE(FloatEqual(transformed.mStart.y, 2.0f));
    EXPECT_TRUE(FloatEqual(transformed.mStart.z, 3.0f));
    EXPECT_TRUE(FloatEqual(transformed.mEnd.x, 2.0f));
    EXPECT_TRUE(FloatEqual(transformed.mEnd.y, 2.0f));
    EXPECT_TRUE(FloatEqual(transformed.mEnd.z, 3.0f));
}

TEST(SegmentTest, InverseTransform) {
    Vec3 start(1.0f, 2.0f, 3.0f);
    Vec3 end(2.0f, 2.0f, 3.0f);
    Segment segment(start, end);
    
    // 创建一个简单的平移变换
    Transform transform;
    transform.mTranslation = Vec3(1.0f, 2.0f, 3.0f);
    
    Segment inverseTransformed = segment.InverseTransform(transform);
    EXPECT_TRUE(FloatEqual(inverseTransformed.mStart.x, 0.0f));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mStart.y, 0.0f));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mStart.z, 0.0f));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mEnd.x, 1.0f));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mEnd.y, 0.0f));
    EXPECT_TRUE(FloatEqual(inverseTransformed.mEnd.z, 0.0f));
}

} // namespace SympConvTest
