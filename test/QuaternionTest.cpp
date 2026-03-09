#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include "SympConv/Quaternion.h"
#include "SympConv/Vector.h"
#include "SympConv/Matrix.h"
#include "TestUtils.h"

namespace SympConvTest {

using Quat = SympConv::TQuaternion<float>;
using Vec3 = SympConv::TVector3<float>;

TEST(QuaternionTest, DefaultConstructor) {
    Quat q1(0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(FloatEqual(q1.w, 1.0f));
    EXPECT_TRUE(FloatEqual(q1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(q1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(q1.z, 0.0f));
}

TEST(QuaternionTest, Identity) {
    Quat q2 = Quat::Identity();
    EXPECT_TRUE(FloatEqual(q2.w, 1.0f));
    EXPECT_TRUE(FloatEqual(q2.x, 0.0f));
    EXPECT_TRUE(FloatEqual(q2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(q2.z, 0.0f));
}

TEST(QuaternionTest, VectorScalarConstructor) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quat q3(v, 4.0f);
    EXPECT_TRUE(FloatEqual(q3.x, 1.0f));
    EXPECT_TRUE(FloatEqual(q3.y, 2.0f));
    EXPECT_TRUE(FloatEqual(q3.z, 3.0f));
    EXPECT_TRUE(FloatEqual(q3.w, 4.0f));
}

TEST(QuaternionTest, SetMethod) {
    Quat q4;
    q4.Set(0.1f, 0.2f, 0.3f);
    EXPECT_TRUE(FloatEqual(q4.x, 0.1f));
    EXPECT_TRUE(FloatEqual(q4.y, 0.2f));
    EXPECT_TRUE(FloatEqual(q4.z, 0.3f));
}

TEST(QuaternionTest, Operators) {
    Quat q5(1.0f, 0.0f, 0.0f, 0.0f);
    Quat q6(0.0f, 1.0f, 0.0f, 0.0f);
    
    // 测试加法
    Quat qAdd = q5 + q6;
    EXPECT_TRUE(FloatEqual(qAdd.x, 1.0f));
    EXPECT_TRUE(FloatEqual(qAdd.y, 1.0f));
    
    // 测试减法
    Quat qSub = q5 - q6;
    EXPECT_TRUE(FloatEqual(qSub.x, 1.0f));
    EXPECT_TRUE(FloatEqual(qSub.y, -1.0f));
    
    // 测试标量乘法
    Quat qScalarMul = q5 * 2.0f;
    EXPECT_TRUE(FloatEqual(qScalarMul.x, 2.0f));
    
    // 测试标量除法
    Quat qScalarDiv = q5 / 2.0f;
    EXPECT_TRUE(FloatEqual(qScalarDiv.x, 0.5f));
}

TEST(QuaternionTest, CompoundOperators) {
    Quat q5(1.0f, 0.0f, 0.0f, 0.0f);
    Quat q6(0.0f, 1.0f, 0.0f, 0.0f);
    
    // 测试复合加法
    Quat q7 = q5;
    q7 += q6;
    EXPECT_TRUE(FloatEqual(q7.x, 1.0f));
    EXPECT_TRUE(FloatEqual(q7.y, 1.0f));
    
    // 测试复合减法
    Quat q8 = q5;
    q8 -= q6;
    EXPECT_TRUE(FloatEqual(q8.x, 1.0f));
    EXPECT_TRUE(FloatEqual(q8.y, -1.0f));
    
    // 测试复合乘法
    Quat q10 = q5;
    q10 *= 2.0f;
    EXPECT_TRUE(FloatEqual(q10.x, 2.0f));
    
    // 测试复合除法
    Quat q11 = q5;
    q11 /= 2.0f;
    EXPECT_TRUE(FloatEqual(q11.x, 0.5f));
}

TEST(QuaternionTest, DotProduct) {
    Quat q5(1.0f, 0.0f, 0.0f, 0.0f);
    Quat q6(0.0f, 1.0f, 0.0f, 0.0f);
    
    float dot = q5.Dot(q6);
    EXPECT_TRUE(FloatEqual(dot, 0.0f));
    
    float dotProduct = Quat::DotProduct(q5, q6);
    EXPECT_TRUE(FloatEqual(dotProduct, 0.0f));
}

TEST(QuaternionTest, GetImaginary) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quat q3(v, 4.0f);
    Vec3 imaginary = q3.GetImaginary();
    EXPECT_TRUE(FloatEqual(imaginary.x, 1.0f));
    EXPECT_TRUE(FloatEqual(imaginary.y, 2.0f));
    EXPECT_TRUE(FloatEqual(imaginary.z, 3.0f));
}

TEST(QuaternionTest, Length) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quat q3(v, 4.0f);
    
    float lengthSquared = q3.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared, 1.0f + 4.0f + 9.0f + 16.0f));
    
    float length = q3.Length();
    EXPECT_TRUE(FloatEqual(length, std::sqrt(30.0f)));
}

TEST(QuaternionTest, Normalize) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quat q3(v, 4.0f);
    Quat normalized = q3.Normalize();
    EXPECT_TRUE(FloatEqual(normalized.Length(), 1.0f));
}

TEST(QuaternionTest, Conjugate) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quat q3(v, 4.0f);
    Quat conjugate = q3.Conjugate();
    EXPECT_TRUE(FloatEqual(conjugate.x, -1.0f));
    EXPECT_TRUE(FloatEqual(conjugate.y, -2.0f));
    EXPECT_TRUE(FloatEqual(conjugate.z, -3.0f));
    EXPECT_TRUE(FloatEqual(conjugate.w, 4.0f));
}

} // namespace SympConvTest
