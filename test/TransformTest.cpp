#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include "SympConv/Transform.h"
#include "SympConv/Vector.h"
#include "SympConv/Quaternion.h"
#include "SympConv/Matrix.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Transform = SympConv::Transform;
using Vec3 = SympConv::Vector3;
using Quaternion = SympConv::Quaternion;
using Mat4 = SympConv::Matrix4x4;

TEST(TransformTest, DefaultConstructor) {
    Transform transform1;
    EXPECT_TRUE(FloatEqual(transform1.mTranslation.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(transform1.mScale.x, Real(1.0)));
}

TEST(TransformTest, ParameterizedConstructor) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Vec3 scale(Real(2.0), Real(2.0), Real(2.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    EXPECT_TRUE(FloatEqual(transform2.mTranslation.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(transform2.mScale.x, Real(2.0)));
}

TEST(TransformTest, TranslationRotationConstructor) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform3(translation, rotation);
    EXPECT_TRUE(FloatEqual(transform3.mTranslation.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(transform3.mScale.x, Real(1.0)));
}

TEST(TransformTest, TranslationConstructor) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Transform transform4(translation);
    EXPECT_TRUE(FloatEqual(transform4.mTranslation.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(transform4.mScale.x, Real(1.0)));
}

TEST(TransformTest, QuaternionConstructor) {
    Quaternion rotation = Quaternion::Identity();
    Transform transform5(rotation);
    EXPECT_TRUE(FloatEqual(transform5.mTranslation.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(transform5.mScale.x, Real(1.0)));
}

TEST(TransformTest, TransformPoint) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Vec3 scale(Real(2.0), Real(2.0), Real(2.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 point(Real(1.0), Real(0.0), Real(0.0));
    Vec3 transformed = transform2.TransformPoint(point);
    EXPECT_TRUE(FloatEqual(transformed.x, Real(3.0)));
    EXPECT_TRUE(FloatEqual(transformed.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(transformed.z, Real(3.0)));
}

TEST(TransformTest, TransformVector) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Vec3 scale(Real(2.0), Real(2.0), Real(2.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 vector(Real(1.0), Real(0.0), Real(0.0));
    Vec3 transformedVector = transform2.TransformVector(vector);
    EXPECT_TRUE(FloatEqual(transformedVector.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(transformedVector.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(transformedVector.z, Real(0.0)));
}

TEST(TransformTest, TransformDirection) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Vec3 scale(Real(2.0), Real(2.0), Real(2.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 direction(Real(1.0), Real(0.0), Real(0.0));
    Vec3 transformedDir = transform2.TransformDirection(direction);
    EXPECT_TRUE(FloatEqual(transformedDir.x, Real(2.0)));
}

TEST(TransformTest, InverseTransformPoint) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Vec3 scale(Real(2.0), Real(2.0), Real(2.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 point(Real(1.0), Real(0.0), Real(0.0));
    Vec3 transformed = transform2.TransformPoint(point);
    
    Transform inverse = transform2.Inverse();
    Vec3 originalPoint = inverse.TransformPoint(transformed);
    EXPECT_TRUE(FloatEqual(originalPoint.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(originalPoint.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(originalPoint.z, Real(0.0)));
}

TEST(TransformTest, InverseTransformVector) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Vec3 scale(Real(2.0), Real(2.0), Real(2.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 vector(Real(1.0), Real(0.0), Real(0.0));
    Vec3 transformedVector = transform2.TransformVector(vector);
    
    Transform inverse = transform2.Inverse();
    Vec3 originalVector = inverse.TransformVector(transformedVector);
    EXPECT_TRUE(FloatEqual(originalVector.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(originalVector.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(originalVector.z, Real(0.0)));
}

TEST(TransformTest, InverseTransformDirection) {
    Vec3 translation(Real(1.0), Real(2.0), Real(3.0));
    Vec3 scale(Real(2.0), Real(2.0), Real(2.0));
    Quaternion rotation = Quaternion::Identity();
    Transform transform2(translation, scale, rotation);
    
    Vec3 direction(Real(1.0), Real(0.0), Real(0.0));
    Vec3 transformedDir = transform2.TransformDirection(direction);
    
    Transform inverse = transform2.Inverse();
    Vec3 originalDir = inverse.TransformDirection(transformedDir);
    EXPECT_TRUE(FloatEqual(originalDir.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(originalDir.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(originalDir.z, Real(0.0)));
}

TEST(TransformTest, StaticMethods) {
    Transform identity = Transform::Identity();
    EXPECT_TRUE(FloatEqual(identity.mTranslation.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(identity.mScale.x, Real(1.0)));
    
    Transform trans = Transform::Translation(Vec3(Real(1.0), Real(0.0), Real(0.0)));
    EXPECT_TRUE(FloatEqual(trans.mTranslation.x, Real(1.0)));
    
    Quaternion rotation = Quaternion::Identity();
    Transform rot = Transform::Rotation(rotation);
    EXPECT_TRUE(FloatEqual(rot.mOrientation.w, rotation.w));
    
    Transform sc = Transform::Scaling(Vec3(Real(2.0), Real(2.0), Real(2.0)));
    EXPECT_TRUE(FloatEqual(sc.mScale.x, Real(2.0)));
}

} // namespace SympConvTest
