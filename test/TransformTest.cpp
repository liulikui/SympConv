#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include "SympConv/Transform.h"
#include "SympConv/Vector.h"
#include "SympConv/Quaternion.h"
#include "SympConv/Matrix.h"
#include "TestUtils.h"

namespace SympConvTest {

using Transform = SympConv::Transform;
using Vec3 = SympConv::Vector3;
using Quaternion = SympConv::Quaternion;
using Mat4 = SympConv::Matrix4x4;

TEST(TransformTest, DefaultConstructor) {
    Transform transform1;
    EXPECT_TRUE(FloatEqual(transform1.mTranslation.x, 0.0f));
    EXPECT_TRUE(FloatEqual(transform1.mScale.x, 1.0f));
}

TEST(TransformTest, ParameterizedConstructor) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Vec3 scale(2.0f, 2.0f, 2.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    EXPECT_TRUE(FloatEqual(transform2.mTranslation.x, 1.0f));
    EXPECT_TRUE(FloatEqual(transform2.mScale.x, 2.0f));
}

TEST(TransformTest, TranslationRotationConstructor) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform3(translation, rotation);
    EXPECT_TRUE(FloatEqual(transform3.mTranslation.x, 1.0f));
    EXPECT_TRUE(FloatEqual(transform3.mScale.x, 1.0f));
}

TEST(TransformTest, TranslationConstructor) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Transform transform4(translation);
    EXPECT_TRUE(FloatEqual(transform4.mTranslation.x, 1.0f));
    EXPECT_TRUE(FloatEqual(transform4.mScale.x, 1.0f));
}

TEST(TransformTest, QuaternionConstructor) {
    Quaternion rotation = Quaternion::Identity();
    Transform transform5(rotation);
    EXPECT_TRUE(FloatEqual(transform5.mTranslation.x, 0.0f));
    EXPECT_TRUE(FloatEqual(transform5.mScale.x, 1.0f));
}

TEST(TransformTest, TransformPoint) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Vec3 scale(2.0f, 2.0f, 2.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 point(1.0f, 0.0f, 0.0f);
    Vec3 transformed = transform2.TransformPoint(point);
    EXPECT_TRUE(FloatEqual(transformed.x, 3.0f));
    EXPECT_TRUE(FloatEqual(transformed.y, 2.0f));
    EXPECT_TRUE(FloatEqual(transformed.z, 3.0f));
}

TEST(TransformTest, TransformVector) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Vec3 scale(2.0f, 2.0f, 2.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 vector(1.0f, 0.0f, 0.0f);
    Vec3 transformedVector = transform2.TransformVector(vector);
    EXPECT_TRUE(FloatEqual(transformedVector.x, 2.0f));
    EXPECT_TRUE(FloatEqual(transformedVector.y, 0.0f));
    EXPECT_TRUE(FloatEqual(transformedVector.z, 0.0f));
}

TEST(TransformTest, TransformDirection) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Vec3 scale(2.0f, 2.0f, 2.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 direction(1.0f, 0.0f, 0.0f);
    Vec3 transformedDir = transform2.TransformDirection(direction);
    EXPECT_TRUE(FloatEqual(transformedDir.x, 2.0f));
}

TEST(TransformTest, InverseTransformPoint) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Vec3 scale(2.0f, 2.0f, 2.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 point(1.0f, 0.0f, 0.0f);
    Vec3 transformed = transform2.TransformPoint(point);
    
    Transform inverse = transform2.Inverse();
    Vec3 originalPoint = inverse.TransformPoint(transformed);
    EXPECT_TRUE(FloatEqual(originalPoint.x, 1.0f));
    EXPECT_TRUE(FloatEqual(originalPoint.y, 0.0f));
    EXPECT_TRUE(FloatEqual(originalPoint.z, 0.0f));
}

TEST(TransformTest, InverseTransformVector) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Vec3 scale(2.0f, 2.0f, 2.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 vector(1.0f, 0.0f, 0.0f);
    Vec3 transformedVector = transform2.TransformVector(vector);
    
    Transform inverse = transform2.Inverse();
    Vec3 originalVector = inverse.TransformVector(transformedVector);
    EXPECT_TRUE(FloatEqual(originalVector.x, 1.0f));
    EXPECT_TRUE(FloatEqual(originalVector.y, 0.0f));
    EXPECT_TRUE(FloatEqual(originalVector.z, 0.0f));
}

TEST(TransformTest, InverseTransformDirection) {
    Vec3 translation(1.0f, 2.0f, 3.0f);
    Vec3 scale(2.0f, 2.0f, 2.0f);
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 direction(1.0f, 0.0f, 0.0f);
    Vec3 transformedDir = transform2.TransformDirection(direction);
    
    Transform inverse = transform2.Inverse();
    Vec3 originalDir = inverse.TransformDirection(transformedDir);
    EXPECT_TRUE(FloatEqual(originalDir.x, 1.0f));
    EXPECT_TRUE(FloatEqual(originalDir.y, 0.0f));
    EXPECT_TRUE(FloatEqual(originalDir.z, 0.0f));
}

TEST(TransformTest, StaticMethods) {
    Transform identity = Transform::Identity();
    EXPECT_TRUE(FloatEqual(identity.mTranslation.x, 0.0f));
    EXPECT_TRUE(FloatEqual(identity.mScale.x, 1.0f));
    
    Transform trans = Transform::Translation(Vec3(1.0f, 0.0f, 0.0f));
    EXPECT_TRUE(FloatEqual(trans.mTranslation.x, 1.0f));
    
    Quaternion rotation = Quaternion::Identity();
    Transform rot = Transform::Rotation(rotation);
    EXPECT_TRUE(FloatEqual(rot.mOrientation.w, rotation.w));
    
    Transform sc = Transform::Scaling(Vec3(2.0f, 2.0f, 2.0f));
    EXPECT_TRUE(FloatEqual(sc.mScale.x, 2.0f));
}

} // namespace SympConvTest
