#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include "SympConv/Quaternion.h"
#include "SympConv/Vector.h"
#include "SympConv/Matrix.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Quaternion = SympConv::Quaternion;
using Vec3 = SympConv::Vector3;

TEST(QuaternionTest, DefaultConstructor) {
    Quaternion q1(Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    EXPECT_TRUE(FloatEqual(q1.w, Real(1.0)));
    EXPECT_TRUE(FloatEqual(q1.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q1.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q1.z, Real(0.0)));
}

TEST(QuaternionTest, Identity) {
    Quaternion q2 = Quaternion::Identity();
    EXPECT_TRUE(FloatEqual(q2.w, Real(1.0)));
    EXPECT_TRUE(FloatEqual(q2.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q2.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q2.z, Real(0.0)));
}

TEST(QuaternionTest, VectorScalarConstructor) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    EXPECT_TRUE(FloatEqual(q3.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(q3.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(q3.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(q3.w, Real(4.0)));
}

TEST(QuaternionTest, SetMethod) {
    Quaternion q4;
    q4.Set(Real(0.1), Real(0.2), Real(0.3));
    EXPECT_TRUE(FloatEqual(q4.x, Real(0.1)));
    EXPECT_TRUE(FloatEqual(q4.y, Real(0.2)));
    EXPECT_TRUE(FloatEqual(q4.z, Real(0.3)));
}

TEST(QuaternionTest, Operators) {
    Quaternion q5(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q6(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    
    // 测试加法
    Quaternion qAdd = q5 + q6;
    EXPECT_TRUE(FloatEqual(qAdd.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(qAdd.y, Real(1.0)));
    
    // 测试减法
    Quaternion qSub = q5 - q6;
    EXPECT_TRUE(FloatEqual(qSub.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(qSub.y, Real(-1.0)));
    
    // 测试标量乘法
    Quaternion qScalarMul = q5 * Real(2.0);
    EXPECT_TRUE(FloatEqual(qScalarMul.x, Real(2.0)));
    
    // 测试标量除法
    Quaternion qScalarDiv = q5 / Real(2.0);
    EXPECT_TRUE(FloatEqual(qScalarDiv.x, Real(0.5)));
}

TEST(QuaternionTest, CompoundOperators) {
    Quaternion q5(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q6(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    
    // 测试复合加法
    Quaternion q7 = q5;
    q7 += q6;
    EXPECT_TRUE(FloatEqual(q7.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(q7.y, Real(1.0)));
    
    // 测试复合减法
    Quaternion q8 = q5;
    q8 -= q6;
    EXPECT_TRUE(FloatEqual(q8.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(q8.y, Real(-1.0)));
    
    // 测试复合乘法
    Quaternion q10 = q5;
    q10 *= Real(2.0);
    EXPECT_TRUE(FloatEqual(q10.x, Real(2.0)));
    
    // 测试复合除法
    Quaternion q11 = q5;
    q11 /= Real(2.0);
    EXPECT_TRUE(FloatEqual(q11.x, Real(0.5)));
}

TEST(QuaternionTest, DotProduct) {
    Quaternion q5(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q6(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    
    Real dot = q5.Dot(q6);
    EXPECT_TRUE(FloatEqual(dot, Real(0.0)));
    
    Real dotProduct = Quaternion::DotProduct(q5, q6);
    EXPECT_TRUE(FloatEqual(dotProduct, Real(0.0)));
}

TEST(QuaternionTest, GetImaginary) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    Vec3 imaginary = q3.GetImaginary();
    EXPECT_TRUE(FloatEqual(imaginary.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(imaginary.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(imaginary.z, Real(3.0)));
}

TEST(QuaternionTest, Length) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    
    Real lengthSquared = q3.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared, Real(1.0) + Real(4.0) + Real(9.0) + Real(16.0)));
    
    Real length = q3.Length();
    EXPECT_TRUE(FloatEqual(length, std::sqrt(Real(30.0))));
}

TEST(QuaternionTest, Normalize) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    Quaternion normalized = q3.Normalize();
    EXPECT_TRUE(FloatEqual(normalized.Length(), Real(1.0)));
}

TEST(QuaternionTest, Conjugate) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    Quaternion conjugate = q3.Conjugate();
    EXPECT_TRUE(FloatEqual(conjugate.x, Real(-1.0)));
    EXPECT_TRUE(FloatEqual(conjugate.y, Real(-2.0)));
    EXPECT_TRUE(FloatEqual(conjugate.z, Real(-3.0)));
    EXPECT_TRUE(FloatEqual(conjugate.w, Real(4.0)));
}

TEST(QuaternionTest, Inverse) {
    Quaternion q(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion inverse = q.Inverse();
    EXPECT_TRUE(FloatEqual(inverse.x, Real(-1.0)));
    EXPECT_TRUE(FloatEqual(inverse.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(inverse.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(inverse.w, Real(0.0)));
    
    // 测试单位四元数的逆
    Quaternion identity = Quaternion::Identity();
    Quaternion identityInverse = identity.Inverse();
    EXPECT_TRUE(FloatEqual(identityInverse.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(identityInverse.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(identityInverse.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(identityInverse.w, Real(1.0)));
}

TEST(QuaternionTest, Pow) {
    // 测试单位四元数的任意次幂
    Quaternion identity = Quaternion::Identity();
    Quaternion identityPow = identity.Pow(Real(2.0));
    EXPECT_TRUE(FloatEqual(identityPow.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(identityPow.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(identityPow.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(identityPow.w, Real(1.0)));
}

TEST(QuaternionTest, Slerp) {
    Quaternion q1 = Quaternion::Identity();
    Quaternion q2 = Quaternion::RotateX((Real)M_PI / Real(2.0));
    
    // 测试t=0
    Quaternion slerp0 = q1.Slerp(q2, Real(0.0));
    EXPECT_TRUE(FloatEqual(slerp0.x, q1.x));
    EXPECT_TRUE(FloatEqual(slerp0.y, q1.y));
    EXPECT_TRUE(FloatEqual(slerp0.z, q1.z));
    EXPECT_TRUE(FloatEqual(slerp0.w, q1.w));
    
    // 测试t=1
    Quaternion slerp1 = q1.Slerp(q2, Real(1.0));
    EXPECT_TRUE(FloatEqual(slerp1.x, q2.x));
    EXPECT_TRUE(FloatEqual(slerp1.y, q2.y));
    EXPECT_TRUE(FloatEqual(slerp1.z, q2.z));
    EXPECT_TRUE(FloatEqual(slerp1.w, q2.w));
}

TEST(QuaternionTest, RotateVector) {
    // 测试绕X轴旋转90度
    Quaternion q = Quaternion::RotateX((Real)M_PI / Real(2.0));
    Vec3 v(Real(0.0), Real(1.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(1.0)));
    
    // 测试绕Y轴旋转90度
    q = Quaternion::RotateY((Real)M_PI / Real(2.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(-1.0)));
    
    // 测试绕Z轴旋转90度
    q = Quaternion::RotateZ((Real)M_PI / Real(2.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(0.0)));
    
    // 测试绕X轴旋转45度
    q = Quaternion::RotateX((Real)M_PI / Real(4.0));
    v = Vec3(Real(0.0), Real(1.0), Real(0.0));
    rotated = q.RotateVector(v);
    Real expected = Real(1.0) / std::sqrt(Real(2.0));
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, expected));
    EXPECT_TRUE(FloatEqual(rotated.z, expected));
    
    // 测试绕Y轴旋转45度
    q = Quaternion::RotateY((Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, expected));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(-expected)));
    
    // 测试绕Z轴旋转45度
    q = Quaternion::RotateZ((Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, expected));
    EXPECT_TRUE(FloatEqual(rotated.y, expected));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(0.0)));
    
    // 测试绕任意轴旋转45度
    Vec3 axis(Real(1.0), Real(1.0), Real(1.0));
    axis = axis.Normalize();
    q = Quaternion::RotateAxis(axis, (Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    // 验证旋转后的向量长度保持不变
    EXPECT_TRUE(FloatEqual(rotated.Length(), Real(1.0)));
} 

TEST(QuaternionTest, ToMatrix3x3) {
    Quaternion q = Quaternion::Identity();
    auto mat = q.ToMatrix3x3();
    EXPECT_TRUE(FloatEqual(mat.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].z, Real(1.0)));
}

TEST(QuaternionTest, ToMatrix4x4) {
    Quaternion q = Quaternion::Identity();
    auto mat = q.ToMatrix4x4();
    EXPECT_TRUE(FloatEqual(mat.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[0].w, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[1].w, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[2].w, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(mat.mRows[3].w, Real(1.0)));
}

TEST(QuaternionTest, FromAxisAngle) {
    Vec3 axis(Real(1.0), Real(0.0), Real(0.0));
    Real angle = (Real)M_PI / Real(2.0);
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);
    EXPECT_TRUE(FloatEqual(q.x, std::sin(angle / Real(2.0))));
    EXPECT_TRUE(FloatEqual(q.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q.w, std::cos(angle / Real(2.0))));
}

TEST(QuaternionTest, ToAxisAngle) {
    Vec3 axis(Real(1.0), Real(0.0), Real(0.0));
    Real angle = (Real)M_PI / Real(2.0);
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);
    
    Vec3 outAxis;
    Real outAngle = q.ToAxisAngle(outAxis);
    EXPECT_TRUE(FloatEqual(outAngle, angle));
    EXPECT_TRUE(FloatEqual(outAxis.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(outAxis.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(outAxis.z, Real(0.0)));
}

TEST(QuaternionTest, FromEulerAngles) {
    Real x = Real(0.0);
    Real y = (Real)M_PI / Real(2.0);
    Real z = Real(0.0);
    Quaternion q = Quaternion::FromEulerAngles(x, y, z);
    
    // 验证旋转效果
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(-1.0)));
}

TEST(QuaternionTest, ToEulerAngles) {
    // 测试小角度，避免万向锁问题
    Real x = Real(0.1);
    Real y = Real(0.1);
    Real z = Real(0.1);
    Quaternion q = Quaternion::FromEulerAngles(x, y, z);
    
    Real outX, outY, outZ;
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
    x = (Real)M_PI / Real(4.0);
    y = (Real)M_PI / Real(4.0);
    z = (Real)M_PI / Real(4.0);
    q = Quaternion::FromEulerAngles(x, y, z);
    q.ToEulerAngles(outX, outY, outZ);
    EXPECT_TRUE(FloatEqual(outX, x));
    EXPECT_TRUE(FloatEqual(outY, y));
    EXPECT_TRUE(FloatEqual(outZ, z));
}

TEST(QuaternionTest, RotateX) {
    Quaternion q = Quaternion::RotateX((Real)M_PI / Real(2.0));
    Vec3 v(Real(0.0), Real(1.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(1.0)));
}

TEST(QuaternionTest, RotateY) {
    Quaternion q = Quaternion::RotateY((Real)M_PI / Real(2.0));
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(-1.0)));
}

TEST(QuaternionTest, RotateZ) {
    Quaternion q = Quaternion::RotateZ((Real)M_PI / Real(2.0));
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(0.0)));
}

TEST(QuaternionTest, RotateAxis) {
    Vec3 axis(Real(0.0), Real(1.0), Real(0.0));
    Real angle = (Real)M_PI / Real(2.0);
    Quaternion q = Quaternion::RotateAxis(axis, angle);
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(FloatEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(rotated.z, Real(-1.0)));
}

TEST(QuaternionTest, Zero) {
    Quaternion q = Quaternion::Zero();
    EXPECT_TRUE(FloatEqual(q.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q.z, Real(0.0)));
    EXPECT_TRUE(FloatEqual(q.w, Real(0.0)));
}

TEST(QuaternionTest, MinimizeAngle) {
    Quaternion q1 = Quaternion::Identity();
    Quaternion q2 = Quaternion::RotateX((Real)M_PI);
    const Quaternion& result = q1.MinimizeAngle(q2);
    // 这里应该返回q2，因为它与q1的夹角最小
}

TEST(QuaternionTest, QuaternionMultiplication) {
    Quaternion q1(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q2(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    Quaternion product = q1 * q2;
    EXPECT_TRUE(FloatEqual(product.x, Real(0.0)));
    EXPECT_TRUE(FloatEqual(product.y, Real(0.0)));
    EXPECT_TRUE(FloatEqual(product.z, Real(1.0)));
    EXPECT_TRUE(FloatEqual(product.w, Real(0.0)));
}

TEST(QuaternionTest, ScalarMultiplication) {
    Quaternion q(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Real scalar = Real(2.0);
    Quaternion product1 = q * scalar;
    Quaternion product2 = scalar * q;
    EXPECT_TRUE(FloatEqual(product1.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(product1.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(product1.z, Real(6.0)));
    EXPECT_TRUE(FloatEqual(product1.w, Real(8.0)));
    EXPECT_TRUE(FloatEqual(product2.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(product2.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(product2.z, Real(6.0)));
    EXPECT_TRUE(FloatEqual(product2.w, Real(8.0)));
}

} // namespace SympConvTest
