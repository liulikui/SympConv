#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include "SympConv/Quaternion.h"
#include "SympConv/Vector.h"
#include "SympConv/Matrix.h"
#include "TestUtils.h"

namespace SympConvTest {

using Quaternion = SympConv::Quaternion;
using Vec3 = SympConv::Vector3;

TEST(QuaternionTest, DefaultConstructor) {
    Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(FloatEqual(q1.w, 1.0f));
    EXPECT_TRUE(FloatEqual(q1.x, 0.0f));
    EXPECT_TRUE(FloatEqual(q1.y, 0.0f));
    EXPECT_TRUE(FloatEqual(q1.z, 0.0f));
}

TEST(QuaternionTest, Identity) {
    Quaternion q2 = Quaternion::Identity();
    EXPECT_TRUE(FloatEqual(q2.w, 1.0f));
    EXPECT_TRUE(FloatEqual(q2.x, 0.0f));
    EXPECT_TRUE(FloatEqual(q2.y, 0.0f));
    EXPECT_TRUE(FloatEqual(q2.z, 0.0f));
}

TEST(QuaternionTest, VectorScalarConstructor) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quaternion q3(v, 4.0f);
    EXPECT_TRUE(FloatEqual(q3.x, 1.0f));
    EXPECT_TRUE(FloatEqual(q3.y, 2.0f));
    EXPECT_TRUE(FloatEqual(q3.z, 3.0f));
    EXPECT_TRUE(FloatEqual(q3.w, 4.0f));
}

TEST(QuaternionTest, SetMethod) {
    Quaternion q4;
    q4.Set(0.1f, 0.2f, 0.3f);
    EXPECT_TRUE(FloatEqual(q4.x, 0.1f));
    EXPECT_TRUE(FloatEqual(q4.y, 0.2f));
    EXPECT_TRUE(FloatEqual(q4.z, 0.3f));
}

TEST(QuaternionTest, Operators) {
    Quaternion q5(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q6(0.0f, 1.0f, 0.0f, 0.0f);
    
    // 测试加法
    Quaternion qAdd = q5 + q6;
    EXPECT_TRUE(FloatEqual(qAdd.x, 1.0f));
    EXPECT_TRUE(FloatEqual(qAdd.y, 1.0f));
    
    // 测试减法
    Quaternion qSub = q5 - q6;
    EXPECT_TRUE(FloatEqual(qSub.x, 1.0f));
    EXPECT_TRUE(FloatEqual(qSub.y, -1.0f));
    
    // 测试标量乘法
    Quaternion qScalarMul = q5 * 2.0f;
    EXPECT_TRUE(FloatEqual(qScalarMul.x, 2.0f));
    
    // 测试标量除法
    Quaternion qScalarDiv = q5 / 2.0f;
    EXPECT_TRUE(FloatEqual(qScalarDiv.x, 0.5f));
}

TEST(QuaternionTest, CompoundOperators) {
    Quaternion q5(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q6(0.0f, 1.0f, 0.0f, 0.0f);
    
    // 测试复合加法
    Quaternion q7 = q5;
    q7 += q6;
    EXPECT_TRUE(FloatEqual(q7.x, 1.0f));
    EXPECT_TRUE(FloatEqual(q7.y, 1.0f));
    
    // 测试复合减法
    Quaternion q8 = q5;
    q8 -= q6;
    EXPECT_TRUE(FloatEqual(q8.x, 1.0f));
    EXPECT_TRUE(FloatEqual(q8.y, -1.0f));
    
    // 测试复合乘法
    Quaternion q10 = q5;
    q10 *= 2.0f;
    EXPECT_TRUE(FloatEqual(q10.x, 2.0f));
    
    // 测试复合除法
    Quaternion q11 = q5;
    q11 /= 2.0f;
    EXPECT_TRUE(FloatEqual(q11.x, 0.5f));
}

TEST(QuaternionTest, DotProduct) {
    Quaternion q5(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q6(0.0f, 1.0f, 0.0f, 0.0f);
    
    float dot = q5.Dot(q6);
    EXPECT_TRUE(FloatEqual(dot, 0.0f));
    
    float dotProduct = Quaternion::DotProduct(q5, q6);
    EXPECT_TRUE(FloatEqual(dotProduct, 0.0f));
}

TEST(QuaternionTest, GetImaginary) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quaternion q3(v, 4.0f);
    Vec3 imaginary = q3.GetImaginary();
    EXPECT_TRUE(FloatEqual(imaginary.x, 1.0f));
    EXPECT_TRUE(FloatEqual(imaginary.y, 2.0f));
    EXPECT_TRUE(FloatEqual(imaginary.z, 3.0f));
}

TEST(QuaternionTest, Length) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quaternion q3(v, 4.0f);
    
    float lengthSquared = q3.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared, 1.0f + 4.0f + 9.0f + 16.0f));
    
    float length = q3.Length();
    EXPECT_TRUE(FloatEqual(length, std::sqrt(30.0f)));
}

TEST(QuaternionTest, Normalize) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quaternion q3(v, 4.0f);
    Quaternion normalized = q3.Normalize();
    EXPECT_TRUE(FloatEqual(normalized.Length(), 1.0f));
}

TEST(QuaternionTest, Conjugate) {
    Vec3 v(1.0f, 2.0f, 3.0f);
    Quaternion q3(v, 4.0f);
    Quaternion conjugate = q3.Conjugate();
    EXPECT_TRUE(FloatEqual(conjugate.x, -1.0f));
    EXPECT_TRUE(FloatEqual(conjugate.y, -2.0f));
    EXPECT_TRUE(FloatEqual(conjugate.z, -3.0f));
    EXPECT_TRUE(FloatEqual(conjugate.w, 4.0f));
}

TEST(QuaternionTest, Inverse) {
    Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion inverse = q.Inverse();
    EXPECT_TRUE(FloatEqual(inverse.x, -1.0f));
    EXPECT_TRUE(FloatEqual(inverse.y, 0.0f));
    EXPECT_TRUE(FloatEqual(inverse.z, 0.0f));
    EXPECT_TRUE(FloatEqual(inverse.w, 0.0f));
    
    // 测试单位四元数的逆
    Quaternion identity = Quaternion::Identity();
    Quaternion identityInverse = identity.Inverse();
    EXPECT_TRUE(FloatEqual(identityInverse.x, 0.0f));
    EXPECT_TRUE(FloatEqual(identityInverse.y, 0.0f));
    EXPECT_TRUE(FloatEqual(identityInverse.z, 0.0f));
    EXPECT_TRUE(FloatEqual(identityInverse.w, 1.0f));
}

TEST(QuaternionTest, Pow) {
    // 测试单位四元数的任意次幂
    Quaternion identity = Quaternion::Identity();
    Quaternion identityPow = identity.Pow(2.0f);
    EXPECT_TRUE(FloatEqual(identityPow.x, 0.0f));
    EXPECT_TRUE(FloatEqual(identityPow.y, 0.0f));
    EXPECT_TRUE(FloatEqual(identityPow.z, 0.0f));
    EXPECT_TRUE(FloatEqual(identityPow.w, 1.0f));
}

TEST(QuaternionTest, Slerp) {
    Quaternion q1 = Quaternion::Identity();
    Quaternion q2 = Quaternion::RotateX((float)M_PI / 2.0f);
    
    // 测试t=0
    Quaternion slerp0 = q1.Slerp(q2, 0.0f);
    EXPECT_TRUE(FloatEqual(slerp0.x, q1.x));
    EXPECT_TRUE(FloatEqual(slerp0.y, q1.y));
    EXPECT_TRUE(FloatEqual(slerp0.z, q1.z));
    EXPECT_TRUE(FloatEqual(slerp0.w, q1.w));
    
    // 测试t=1
    Quaternion slerp1 = q1.Slerp(q2, 1.0f);
    EXPECT_TRUE(FloatEqual(slerp1.x, q2.x));
    EXPECT_TRUE(FloatEqual(slerp1.y, q2.y));
    EXPECT_TRUE(FloatEqual(slerp1.z, q2.z));
    EXPECT_TRUE(FloatEqual(slerp1.w, q2.w));
}

TEST(QuaternionTest, RotateVector) {
    // 测试绕X轴旋转90度
    Quaternion q = Quaternion::RotateX((float)M_PI / 2.0f);
    Vec3 v(0.0f, 1.0f, 0.0f);
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, 1.0f));
    
    // 测试绕Y轴旋转90度
    q = Quaternion::RotateY((float)M_PI / 2.0f);
    v = Vec3(1.0f, 0.0f, 0.0f);
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, -1.0f));
    
    // 测试绕Z轴旋转90度
    q = Quaternion::RotateZ((float)M_PI / 2.0f);
    v = Vec3(1.0f, 0.0f, 0.0f);
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 1.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, 0.0f));
    
    // 测试绕X轴旋转45度
    q = Quaternion::RotateX((float)M_PI / 4.0f);
    v = Vec3(0.0f, 1.0f, 0.0f);
    rotated = q.RotateVector(v);
    float expected = 1.0f / std::sqrt(2.0f);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, expected));
    EXPECT_TRUE(FloatEqual(rotated.z, expected));
    
    // 测试绕Y轴旋转45度
    q = Quaternion::RotateY((float)M_PI / 4.0f);
    v = Vec3(1.0f, 0.0f, 0.0f);
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, expected));
    EXPECT_TRUE(FloatEqual(rotated.y, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, -expected));
    
    // 测试绕Z轴旋转45度
    q = Quaternion::RotateZ((float)M_PI / 4.0f);
    v = Vec3(1.0f, 0.0f, 0.0f);
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, expected));
    EXPECT_TRUE(FloatEqual(rotated.y, expected));
    EXPECT_TRUE(FloatEqual(rotated.z, 0.0f));
    
    // 测试绕任意轴旋转45度
    Vec3 axis(1.0f, 1.0f, 1.0f);
    axis = axis.Normalize();
    q = Quaternion::RotateAxis(axis, (float)M_PI / 4.0f);
    v = Vec3(1.0f, 0.0f, 0.0f);
    rotated = q.RotateVector(v);
    // 验证旋转后的向量长度保持不变
    EXPECT_TRUE(FloatEqual(rotated.Length(), 1.0f));
} 

TEST(QuaternionTest, ToMatrix3x3) {
    Quaternion q = Quaternion::Identity();
    auto mat = q.ToMatrix3x3();
    EXPECT_TRUE(FloatEqual(mat.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].y, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].z, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].x, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].z, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].x, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].y, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].z, 1.0f));
}

TEST(QuaternionTest, ToMatrix4x4) {
    Quaternion q = Quaternion::Identity();
    auto mat = q.ToMatrix4x4();
    EXPECT_TRUE(FloatEqual(mat.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].y, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].z, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].w, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].x, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].z, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].w, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].x, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].y, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].z, 1.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].w, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].x, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].y, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].z, 0.0f));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].w, 1.0f));
}

TEST(QuaternionTest, FromAxisAngle) {
    Vec3 axis(1.0f, 0.0f, 0.0f);
    float angle = (float)M_PI / 2.0f;
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);
    EXPECT_TRUE(FloatEqual(q.x, std::sin(angle / 2.0f)));
    EXPECT_TRUE(FloatEqual(q.y, 0.0f));
    EXPECT_TRUE(FloatEqual(q.z, 0.0f));
    EXPECT_TRUE(FloatEqual(q.w, std::cos(angle / 2.0f)));
}

TEST(QuaternionTest, ToAxisAngle) {
    Vec3 axis(1.0f, 0.0f, 0.0f);
    float angle = (float)M_PI / 2.0f;
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);
    
    Vec3 outAxis;
    float outAngle = q.ToAxisAngle(outAxis);
    EXPECT_TRUE(FloatEqual(outAngle, angle));
    EXPECT_TRUE(FloatEqual(outAxis.x, 1.0f));
    EXPECT_TRUE(FloatEqual(outAxis.y, 0.0f));
    EXPECT_TRUE(FloatEqual(outAxis.z, 0.0f));
}

TEST(QuaternionTest, FromEulerAngles) {
    float x = 0.0f;
    float y = (float)M_PI / 2.0f;
    float z = 0.0f;
    Quaternion q = Quaternion::FromEulerAngles(x, y, z);
    
    // 验证旋转效果
    Vec3 v(1.0f, 0.0f, 0.0f);
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, -1.0f));
}

TEST(QuaternionTest, ToEulerAngles) {
    // 测试小角度，避免万向锁问题
    float x = 0.1f;
    float y = 0.1f;
    float z = 0.1f;
    Quaternion q = Quaternion::FromEulerAngles(x, y, z);
    
    float outX, outY, outZ;
    q.ToEulerAngles(outX, outY, outZ);
    EXPECT_TRUE(FloatEqual(outX, x));
    EXPECT_TRUE(FloatEqual(outY, y));
    EXPECT_TRUE(FloatEqual(outZ, z));
    
    // 测试向量版本
    Vec3 euler = q.ToEulerAngles();
    EXPECT_TRUE(FloatEqual(euler.x, x));
    EXPECT_TRUE(FloatEqual(euler.y, y));
    EXPECT_TRUE(FloatEqual(euler.z, z));
    
    // 测试其他角度
    x = (float)M_PI / 4.0f;
    y = (float)M_PI / 4.0f;
    z = (float)M_PI / 4.0f;
    q = Quaternion::FromEulerAngles(x, y, z);
    q.ToEulerAngles(outX, outY, outZ);
    EXPECT_TRUE(FloatEqual(outX, x));
    EXPECT_TRUE(FloatEqual(outY, y));
    EXPECT_TRUE(FloatEqual(outZ, z));
}

TEST(QuaternionTest, RotateX) {
    Quaternion q = Quaternion::RotateX((float)M_PI / 2.0f);
    Vec3 v(0.0f, 1.0f, 0.0f);
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, 1.0f));
}

TEST(QuaternionTest, RotateY) {
    Quaternion q = Quaternion::RotateY((float)M_PI / 2.0f);
    Vec3 v(1.0f, 0.0f, 0.0f);
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, -1.0f));
}

TEST(QuaternionTest, RotateZ) {
    Quaternion q = Quaternion::RotateZ((float)M_PI / 2.0f);
    Vec3 v(1.0f, 0.0f, 0.0f);
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 1.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, 0.0f));
}

TEST(QuaternionTest, RotateAxis) {
    Vec3 axis(0.0f, 1.0f, 0.0f);
    float angle = (float)M_PI / 2.0f;
    Quaternion q = Quaternion::RotateAxis(axis, angle);
    Vec3 v(1.0f, 0.0f, 0.0f);
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.y, 0.0f));
    EXPECT_TRUE(FloatEqual(rotated.z, -1.0f));
}

TEST(QuaternionTest, Zero) {
    Quaternion q = Quaternion::Zero();
    EXPECT_TRUE(FloatEqual(q.x, 0.0f));
    EXPECT_TRUE(FloatEqual(q.y, 0.0f));
    EXPECT_TRUE(FloatEqual(q.z, 0.0f));
    EXPECT_TRUE(FloatEqual(q.w, 0.0f));
}

TEST(QuaternionTest, MinimizeAngle) {
    Quaternion q1 = Quaternion::Identity();
    Quaternion q2 = Quaternion::RotateX((float)M_PI);
    const Quaternion& result = q1.MinimizeAngle(q2);
    // 这里应该返回q2，因为它与q1的夹角最小
}

TEST(QuaternionTest, QuaternionMultiplication) {
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(0.0f, 1.0f, 0.0f, 0.0f);
    Quaternion product = q1 * q2;
    EXPECT_TRUE(FloatEqual(product.x, 0.0f));
    EXPECT_TRUE(FloatEqual(product.y, 0.0f));
    EXPECT_TRUE(FloatEqual(product.z, 1.0f));
    EXPECT_TRUE(FloatEqual(product.w, 0.0f));
}

TEST(QuaternionTest, ScalarMultiplication) {
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    float scalar = 2.0f;
    Quaternion product1 = q * scalar;
    Quaternion product2 = scalar * q;
    EXPECT_TRUE(FloatEqual(product1.x, 2.0f));
    EXPECT_TRUE(FloatEqual(product1.y, 4.0f));
    EXPECT_TRUE(FloatEqual(product1.z, 6.0f));
    EXPECT_TRUE(FloatEqual(product1.w, 8.0f));
    EXPECT_TRUE(FloatEqual(product2.x, 2.0f));
    EXPECT_TRUE(FloatEqual(product2.y, 4.0f));
    EXPECT_TRUE(FloatEqual(product2.z, 6.0f));
    EXPECT_TRUE(FloatEqual(product2.w, 8.0f));
}

} // namespace SympConvTest
