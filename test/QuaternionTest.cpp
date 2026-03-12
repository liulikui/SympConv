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
    EXPECT_TRUE(SympConv::RealEqual(q1.w, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(q1.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q1.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q1.z, Real(0.0)));
}

TEST(QuaternionTest, Identity) {
    Quaternion q2 = Quaternion::Identity();
    EXPECT_TRUE(SympConv::RealEqual(q2.w, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(q2.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q2.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q2.z, Real(0.0)));
}

TEST(QuaternionTest, VectorScalarConstructor) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    EXPECT_TRUE(SympConv::RealEqual(q3.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(q3.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(q3.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(q3.w, Real(4.0)));
}

TEST(QuaternionTest, SetMethod) {
    Quaternion q4;
    q4.Set(Real(0.1), Real(0.2), Real(0.3));
    EXPECT_TRUE(SympConv::RealEqual(q4.x, Real(0.1)));
    EXPECT_TRUE(SympConv::RealEqual(q4.y, Real(0.2)));
    EXPECT_TRUE(SympConv::RealEqual(q4.z, Real(0.3)));
}

TEST(QuaternionTest, Operators) {
    Quaternion q5(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q6(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    
    // 测试加法
    Quaternion qAdd = q5 + q6;
    EXPECT_TRUE(SympConv::RealEqual(qAdd.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(qAdd.y, Real(1.0)));
    
    // 测试减法
    Quaternion qSub = q5 - q6;
    EXPECT_TRUE(SympConv::RealEqual(qSub.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(qSub.y, Real(-1.0)));
    
    // 测试标量乘法
    Quaternion qScalarMul = q5 * Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(qScalarMul.x, Real(2.0)));
    
    // 测试标量除法
    Quaternion qScalarDiv = q5 / Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(qScalarDiv.x, Real(0.5)));
}

TEST(QuaternionTest, CompoundOperators) {
    Quaternion q5(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q6(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    
    // 测试复合加法
    Quaternion q7 = q5;
    q7 += q6;
    EXPECT_TRUE(SympConv::RealEqual(q7.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(q7.y, Real(1.0)));
    
    // 测试复合减法
    Quaternion q8 = q5;
    q8 -= q6;
    EXPECT_TRUE(SympConv::RealEqual(q8.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(q8.y, Real(-1.0)));
    
    // 测试复合乘法
    Quaternion q10 = q5;
    q10 *= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(q10.x, Real(2.0)));
    
    // 测试复合除法
    Quaternion q11 = q5;
    q11 /= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(q11.x, Real(0.5)));
}

TEST(QuaternionTest, DotProduct) {
    Quaternion q5(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q6(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    
    Real dot = q5.Dot(q6);
    EXPECT_TRUE(SympConv::RealEqual(dot, Real(0.0)));
    
    Real dotProduct = Quaternion::DotProduct(q5, q6);
    EXPECT_TRUE(SympConv::RealEqual(dotProduct, Real(0.0)));
}

TEST(QuaternionTest, GetImaginary) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    Vec3 imaginary = q3.GetImaginary();
    EXPECT_TRUE(SympConv::RealEqual(imaginary.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(imaginary.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(imaginary.z, Real(3.0)));
}

TEST(QuaternionTest, Length) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    
    Real lengthSquared = q3.LengthSquared();
    EXPECT_TRUE(SympConv::RealEqual(lengthSquared, Real(1.0) + Real(4.0) + Real(9.0) + Real(16.0)));
    
    Real length = q3.Length();
    EXPECT_TRUE(SympConv::RealEqual(length, std::sqrt(Real(30.0))));
}

TEST(QuaternionTest, Normalize) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    Quaternion normalized = q3.Normalize();
    EXPECT_TRUE(SympConv::RealEqual(normalized.Length(), Real(1.0)));
}

TEST(QuaternionTest, Conjugate) {
    Vec3 v(Real(1.0), Real(2.0), Real(3.0));
    Quaternion q3(v, Real(4.0));
    Quaternion conjugate = q3.Conjugate();
    EXPECT_TRUE(SympConv::RealEqual(conjugate.x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(conjugate.y, Real(-2.0)));
    EXPECT_TRUE(SympConv::RealEqual(conjugate.z, Real(-3.0)));
    EXPECT_TRUE(SympConv::RealEqual(conjugate.w, Real(4.0)));
}

TEST(QuaternionTest, Inverse) {
    Quaternion q(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion inverse = q.Inverse();
    EXPECT_TRUE(SympConv::RealEqual(inverse.x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(inverse.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(inverse.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(inverse.w, Real(0.0)));
    
    // 测试单位四元数的逆
    Quaternion identity = Quaternion::Identity();
    Quaternion identityInverse = identity.Inverse();
    EXPECT_TRUE(SympConv::RealEqual(identityInverse.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identityInverse.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identityInverse.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identityInverse.w, Real(1.0)));
}

TEST(QuaternionTest, Pow) {
    // 测试单位四元数的任意次幂
    Quaternion identity = Quaternion::Identity();
    Quaternion identityPow = identity.Pow(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(identityPow.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identityPow.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identityPow.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identityPow.w, Real(1.0)));
}

TEST(QuaternionTest, Slerp) {
    Quaternion q1 = Quaternion::Identity();
    Quaternion q2 = Quaternion::RotateX((Real)M_PI / Real(2.0));
    
    // 测试t=0
    Quaternion slerp0 = q1.Slerp(q2, Real(0.0));
    EXPECT_TRUE(SympConv::RealEqual(slerp0.x, q1.x));
    EXPECT_TRUE(SympConv::RealEqual(slerp0.y, q1.y));
    EXPECT_TRUE(SympConv::RealEqual(slerp0.z, q1.z));
    EXPECT_TRUE(SympConv::RealEqual(slerp0.w, q1.w));
    EXPECT_TRUE(SympConv::RealEqual(slerp0.Length(), Real(1.0))); // 验证归一化
    
    // 测试t=1
    Quaternion slerp1 = q1.Slerp(q2, Real(1.0));
    EXPECT_TRUE(SympConv::RealEqual(slerp1.x, q2.x));
    EXPECT_TRUE(SympConv::RealEqual(slerp1.y, q2.y));
    EXPECT_TRUE(SympConv::RealEqual(slerp1.z, q2.z));
    EXPECT_TRUE(SympConv::RealEqual(slerp1.w, q2.w));
    EXPECT_TRUE(SympConv::RealEqual(slerp1.Length(), Real(1.0))); // 验证归一化
    
    // 测试t=0.5
    Quaternion slerp05 = q1.Slerp(q2, Real(0.5));
    EXPECT_TRUE(SympConv::RealEqual(slerp05.Length(), Real(1.0))); // 验证归一化
    // 验证插值结果是否在两个四元数之间
    Real dot0 = q1.Dot(slerp05);
    Real dot1 = q2.Dot(slerp05);
    EXPECT_TRUE(dot0 > Real(0.0));
    EXPECT_TRUE(dot1 > Real(0.0));
}

TEST(QuaternionTest, RotateVector) {
    // 测试绕X轴旋转90度
    Quaternion q = Quaternion::RotateX((Real)M_PI / Real(2.0));
    Vec3 v(Real(0.0), Real(1.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(1.0)));
    
    // 测试绕Y轴旋转90度
    q = Quaternion::RotateY((Real)M_PI / Real(2.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-1.0)));
    
    // 测试绕Z轴旋转90度
    q = Quaternion::RotateZ((Real)M_PI / Real(2.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕X轴旋转45度
    q = Quaternion::RotateX((Real)M_PI / Real(4.0));
    v = Vec3(Real(0.0), Real(1.0), Real(0.0));
    rotated = q.RotateVector(v);
    Real expected = Real(1.0) / std::sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, expected));
    
    // 测试绕Y轴旋转45度
    q = Quaternion::RotateY((Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-expected)));
    
    // 测试绕Z轴旋转45度
    q = Quaternion::RotateZ((Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕任意轴旋转45度
    Vec3 axis(Real(1.0), Real(1.0), Real(1.0));
    axis = axis.Normalize();
    q = Quaternion::RotateAxis(axis, (Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    // 验证旋转后的向量长度保持不变
    EXPECT_TRUE(SympConv::RealEqual(rotated.Length(), Real(1.0)));
} 

TEST(QuaternionTest, ToMatrix3x3) {
    // 测试Identity四元数
    Quaternion q = Quaternion::Identity();
    auto mat = q.ToMatrix3x3();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(1.0)));
    
    // 测试绕X轴旋转90度的四元数
    q = Quaternion::RotateX((Real)M_PI / Real(2.0));
    mat = q.ToMatrix3x3();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(0.0)));
    
    // 测试绕Y轴旋转90度的四元数
    q = Quaternion::RotateY((Real)M_PI / Real(2.0));
    mat = q.ToMatrix3x3();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(0.0)));
    
    // 测试绕Z轴旋转90度的四元数
    q = Quaternion::RotateZ((Real)M_PI / Real(2.0));
    mat = q.ToMatrix3x3();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(1.0)));
}

TEST(QuaternionTest, ToMatrix4x4) {
    // 测试Identity四元数
    Quaternion q = Quaternion::Identity();
    auto mat = q.ToMatrix4x4();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].w, Real(1.0)));
    
    // 测试绕X轴旋转90度的四元数
    q = Quaternion::RotateX((Real)M_PI / Real(2.0));
    mat = q.ToMatrix4x4();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].w, Real(1.0)));
    
    // 测试绕Y轴旋转90度的四元数
    q = Quaternion::RotateY((Real)M_PI / Real(2.0));
    mat = q.ToMatrix4x4();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].w, Real(1.0)));
    
    // 测试绕Z轴旋转90度的四元数
    q = Quaternion::RotateZ((Real)M_PI / Real(2.0));
    mat = q.ToMatrix4x4();
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].w, Real(1.0)));
    
    // 测试绕Y轴旋转45度的四元数（非90度测试用例）
    q = Quaternion::RotateY((Real)M_PI / Real(4.0));
    mat = q.ToMatrix4x4();
    Real cos45 = std::cos((Real)M_PI / Real(4.0));
    Real sin45 = std::sin((Real)M_PI / Real(4.0));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].x, cos45));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].z, -sin45));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[0].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[1].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].x, sin45));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].z, cos45));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[2].w, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat.mRows[3].w, Real(1.0)));
}

TEST(QuaternionTest, FromAxisAngle) {
    // 测试绕X轴旋转90度
    Vec3 axis(Real(1.0), Real(0.0), Real(0.0));
    Real angle = (Real)M_PI / Real(2.0);
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);
    EXPECT_TRUE(SympConv::RealEqual(q.x, std::sin(angle / Real(2.0))));
    EXPECT_TRUE(SympConv::RealEqual(q.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.w, std::cos(angle / Real(2.0))));
    
    // 测试绕X轴旋转45度
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::FromAxisAngle(axis, angle);
    EXPECT_TRUE(SympConv::RealEqual(q.x, std::sin(angle / Real(2.0))));
    EXPECT_TRUE(SympConv::RealEqual(q.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.w, std::cos(angle / Real(2.0))));
    
    // 测试绕X轴旋转180度
    angle = (Real)M_PI;
    q = Quaternion::FromAxisAngle(axis, angle);
    EXPECT_TRUE(SympConv::RealEqual(q.x, std::sin(angle / Real(2.0))));
    EXPECT_TRUE(SympConv::RealEqual(q.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.w, std::cos(angle / Real(2.0))));
    
    // 测试绕Y轴旋转45度
    axis = Vec3(Real(0.0), Real(1.0), Real(0.0));
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::FromAxisAngle(axis, angle);
    EXPECT_TRUE(SympConv::RealEqual(q.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.y, std::sin(angle / Real(2.0))));
    EXPECT_TRUE(SympConv::RealEqual(q.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.w, std::cos(angle / Real(2.0))));
    
    // 测试绕Z轴旋转45度
    axis = Vec3(Real(0.0), Real(0.0), Real(1.0));
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::FromAxisAngle(axis, angle);
    EXPECT_TRUE(SympConv::RealEqual(q.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.z, std::sin(angle / Real(2.0))));
    EXPECT_TRUE(SympConv::RealEqual(q.w, std::cos(angle / Real(2.0))));
}

TEST(QuaternionTest, ToAxisAngle) {
    // 测试绕X轴旋转90度
    Vec3 axis(Real(1.0), Real(0.0), Real(0.0));
    Real angle = (Real)M_PI / Real(2.0);
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);
    
    Vec3 outAxis;
    Real outAngle = q.ToAxisAngle(outAxis);
    EXPECT_TRUE(SympConv::RealEqual(outAngle, angle));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.z, Real(0.0)));
    
    // 测试绕X轴旋转45度
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::FromAxisAngle(axis, angle);
    outAngle = q.ToAxisAngle(outAxis);
    EXPECT_TRUE(SympConv::RealEqual(outAngle, angle));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.z, Real(0.0)));
    
    // 测试绕X轴旋转180度
    angle = (Real)M_PI;
    q = Quaternion::FromAxisAngle(axis, angle);
    outAngle = q.ToAxisAngle(outAxis);
    EXPECT_TRUE(SympConv::RealEqual(outAngle, angle));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.z, Real(0.0)));
    
    // 测试绕Y轴旋转45度
    axis = Vec3(Real(0.0), Real(1.0), Real(0.0));
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::FromAxisAngle(axis, angle);
    outAngle = q.ToAxisAngle(outAxis);
    EXPECT_TRUE(SympConv::RealEqual(outAngle, angle));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.z, Real(0.0)));
    
    // 测试绕Z轴旋转45度
    axis = Vec3(Real(0.0), Real(0.0), Real(1.0));
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::FromAxisAngle(axis, angle);
    outAngle = q.ToAxisAngle(outAxis);
    EXPECT_TRUE(SympConv::RealEqual(outAngle, angle));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(outAxis.z, Real(1.0)));
}

TEST(QuaternionTest, FromEulerAngles) {
    // 测试绕Y轴旋转90度
    Real x = Real(0.0);
    Real y = (Real)M_PI / Real(2.0);
    Real z = Real(0.0);
    Quaternion q = Quaternion::FromEulerAngles(x, y, z);
    
    // 验证旋转效果
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-1.0)));
    
    // 测试绕X轴旋转45度
    x = (Real)M_PI / Real(4.0);
    y = Real(0.0);
    z = Real(0.0);
    q = Quaternion::FromEulerAngles(x, y, z);
    v = Vec3(Real(0.0), Real(1.0), Real(0.0));
    rotated = q.RotateVector(v);
    Real expected = Real(1.0) / std::sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, expected));
    
    // 测试绕Y轴旋转45度
    x = Real(0.0);
    y = (Real)M_PI / Real(4.0);
    z = Real(0.0);
    q = Quaternion::FromEulerAngles(x, y, z);
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-expected)));
    
    // 测试绕Z轴旋转45度
    x = Real(0.0);
    y = Real(0.0);
    z = (Real)M_PI / Real(4.0);
    q = Quaternion::FromEulerAngles(x, y, z);
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试复合旋转（X、Y、Z各45度）
    x = (Real)M_PI / Real(4.0);
    y = (Real)M_PI / Real(4.0);
    z = (Real)M_PI / Real(4.0);
    q = Quaternion::FromEulerAngles(x, y, z);
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    // 验证旋转后的向量长度保持不变
    EXPECT_TRUE(SympConv::RealEqual(rotated.Length(), Real(1.0)));
}

TEST(QuaternionTest, ToEulerAngles) {
    // 测试小角度，避免万向锁问题
    Real x = Real(0.1);
    Real y = Real(0.1);
    Real z = Real(0.1);
    Quaternion q = Quaternion::FromEulerAngles(x, y, z);
    
    Real outX, outY, outZ;
    q.ToEulerAngles(outX, outY, outZ);
    EXPECT_TRUE(SympConv::RealEqual(outX, x));
    EXPECT_TRUE(SympConv::RealEqual(outY, y));
    EXPECT_TRUE(SympConv::RealEqual(outZ, z));
    
    // 测试向量版本
    Vec3 euler = q.ToEulerAngles();
    EXPECT_TRUE(SympConv::RealEqual(euler.x, x));
    EXPECT_TRUE(SympConv::RealEqual(euler.y, y));
    EXPECT_TRUE(SympConv::RealEqual(euler.z, z));
    
    // 测试其他角度
    x = (Real)M_PI / Real(4.0);
    y = (Real)M_PI / Real(4.0);
    z = (Real)M_PI / Real(4.0);
    q = Quaternion::FromEulerAngles(x, y, z);
    q.ToEulerAngles(outX, outY, outZ);
    EXPECT_TRUE(SympConv::RealEqual(outX, x));
    EXPECT_TRUE(SympConv::RealEqual(outY, y));
    EXPECT_TRUE(SympConv::RealEqual(outZ, z));
}

TEST(QuaternionTest, RotateX) {
    // 测试绕X轴旋转90度
    Quaternion q = Quaternion::RotateX((Real)M_PI / Real(2.0));
    Vec3 v(Real(0.0), Real(1.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(1.0)));
    
    // 测试绕X轴旋转45度
    q = Quaternion::RotateX((Real)M_PI / Real(4.0));
    v = Vec3(Real(0.0), Real(1.0), Real(0.0));
    rotated = q.RotateVector(v);
    Real expected = Real(1.0) / std::sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, expected));
    
    // 测试绕X轴旋转180度
    q = Quaternion::RotateX((Real)M_PI);
    v = Vec3(Real(0.0), Real(1.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕X轴旋转30度
    q = Quaternion::RotateX((Real)M_PI / Real(6.0));
    v = Vec3(Real(0.0), Real(1.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, std::cos((Real)M_PI / Real(6.0))));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, std::sin((Real)M_PI / Real(6.0))));
}

TEST(QuaternionTest, RotateY) {
    // 测试绕Y轴旋转90度
    Quaternion q = Quaternion::RotateY((Real)M_PI / Real(2.0));
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-1.0)));
    
    // 测试绕Y轴旋转45度
    q = Quaternion::RotateY((Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    Real expected = Real(1.0) / std::sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-expected)));
    
    // 测试绕Y轴旋转180度
    q = Quaternion::RotateY((Real)M_PI);
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕Y轴旋转30度
    q = Quaternion::RotateY((Real)M_PI / Real(6.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, std::cos((Real)M_PI / Real(6.0))));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-std::sin((Real)M_PI / Real(6.0)))));
}

TEST(QuaternionTest, RotateZ) {
    // 测试绕Z轴旋转90度
    Quaternion q = Quaternion::RotateZ((Real)M_PI / Real(2.0));
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕Z轴旋转45度
    q = Quaternion::RotateZ((Real)M_PI / Real(4.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    Real expected = Real(1.0) / std::sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕Z轴旋转180度
    q = Quaternion::RotateZ((Real)M_PI);
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕Z轴旋转30度
    q = Quaternion::RotateZ((Real)M_PI / Real(6.0));
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, std::cos((Real)M_PI / Real(6.0))));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, std::sin((Real)M_PI / Real(6.0))));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
}

TEST(QuaternionTest, RotateAxis) {
    // 测试绕Y轴旋转90度
    Vec3 axis(Real(0.0), Real(1.0), Real(0.0));
    Real angle = (Real)M_PI / Real(2.0);
    Quaternion q = Quaternion::RotateAxis(axis, angle);
    Vec3 v(Real(1.0), Real(0.0), Real(0.0));
    Vec3 rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(-1.0)));
    
    // 测试绕X轴旋转45度
    axis = Vec3(Real(1.0), Real(0.0), Real(0.0));
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::RotateAxis(axis, angle);
    v = Vec3(Real(0.0), Real(1.0), Real(0.0));
    rotated = q.RotateVector(v);
    Real expected = Real(1.0) / std::sqrt(Real(2.0));
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, expected));
    
    // 测试绕Z轴旋转45度
    axis = Vec3(Real(0.0), Real(0.0), Real(1.0));
    angle = (Real)M_PI / Real(4.0);
    q = Quaternion::RotateAxis(axis, angle);
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    EXPECT_TRUE(SympConv::RealEqual(rotated.x, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.y, expected));
    EXPECT_TRUE(SympConv::RealEqual(rotated.z, Real(0.0)));
    
    // 测试绕任意轴旋转30度
    axis = Vec3(Real(1.0), Real(1.0), Real(1.0));
    axis = axis.Normalize();
    angle = (Real)M_PI / Real(6.0);
    q = Quaternion::RotateAxis(axis, angle);
    v = Vec3(Real(1.0), Real(0.0), Real(0.0));
    rotated = q.RotateVector(v);
    // 验证旋转后的向量长度保持不变
    EXPECT_TRUE(SympConv::RealEqual(rotated.Length(), Real(1.0)));
}

TEST(QuaternionTest, Zero) {
    Quaternion q = Quaternion::Zero();
    EXPECT_TRUE(SympConv::RealEqual(q.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(q.w, Real(0.0)));
}

TEST(QuaternionTest, MinimizeAngle) {
    Quaternion q1 = Quaternion::Identity();
    
    // 测试1：q2与q1的夹角较小
    Quaternion q2 = Quaternion::RotateX((Real)M_PI / Real(4.0)); // 45度旋转
    Quaternion result1 = q1.MinimizeAngle(q2);
    // 验证返回的是q2
    EXPECT_TRUE(SympConv::RealEqual(result1.x, q2.x));
    EXPECT_TRUE(SympConv::RealEqual(result1.y, q2.y));
    EXPECT_TRUE(SympConv::RealEqual(result1.z, q2.z));
    EXPECT_TRUE(SympConv::RealEqual(result1.w, q2.w));
    
    // 测试2：q2与q1的夹角较大，应该返回-q2
    Quaternion q3 = Quaternion::RotateX((Real)M_PI * Real(5.0) / Real(4.0)); // 225度旋转
    Quaternion result2 = q1.MinimizeAngle(q3);
    // 验证返回的是-q3
    Quaternion expected = -q3;
    EXPECT_TRUE(SympConv::RealEqual(result2.x, expected.x));
    EXPECT_TRUE(SympConv::RealEqual(result2.y, expected.y));
    EXPECT_TRUE(SympConv::RealEqual(result2.z, expected.z));
    EXPECT_TRUE(SympConv::RealEqual(result2.w, expected.w));
    
    // 测试3：q2是q1的相反数
    Quaternion q4 = -q1;
    Quaternion result3 = q1.MinimizeAngle(q4);
    // 验证返回的是q4（或q1，因为它们的夹角相同）
    EXPECT_TRUE(SympConv::RealEqual(result3.Length(), Real(1.0)));
}

TEST(QuaternionTest, QuaternionMultiplication) {
    // 测试1：基本的四元数乘法
    Quaternion q1(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
    Quaternion q2(Real(0.0), Real(1.0), Real(0.0), Real(0.0));
    Quaternion product = q1 * q2;
    EXPECT_TRUE(SympConv::RealEqual(product.x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(product.y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(product.z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(product.w, Real(0.0)));
    
    // 测试2：非90度旋转的四元数乘法
    // 绕X轴旋转45度的四元数
    Quaternion qX45 = Quaternion::RotateX((Real)M_PI / Real(4.0));
    // 绕Y轴旋转45度的四元数
    Quaternion qY45 = Quaternion::RotateY((Real)M_PI / Real(4.0));
    // 乘法结果应该是先绕X轴旋转45度，再绕Y轴旋转45度
    Quaternion product2 = qY45 * qX45;
    EXPECT_TRUE(SympConv::RealEqual(product2.Length(), Real(1.0))); // 验证结果是单位四元数
    
    // 测试3：验证四元数乘法的结合律
    Quaternion qZ45 = Quaternion::RotateZ((Real)M_PI / Real(4.0));
    Quaternion product3a = (qX45 * qY45) * qZ45;
    Quaternion product3b = qX45 * (qY45 * qZ45);
    EXPECT_TRUE(SympConv::RealEqual(product3a.x, product3b.x));
    EXPECT_TRUE(SympConv::RealEqual(product3a.y, product3b.y));
    EXPECT_TRUE(SympConv::RealEqual(product3a.z, product3b.z));
    EXPECT_TRUE(SympConv::RealEqual(product3a.w, product3b.w));
    
    // 测试4：与单位四元数相乘
    Quaternion identity = Quaternion::Identity();
    Quaternion product4a = identity * qX45;
    Quaternion product4b = qX45 * identity;
    EXPECT_TRUE(SympConv::RealEqual(product4a.x, qX45.x));
    EXPECT_TRUE(SympConv::RealEqual(product4a.y, qX45.y));
    EXPECT_TRUE(SympConv::RealEqual(product4a.z, qX45.z));
    EXPECT_TRUE(SympConv::RealEqual(product4a.w, qX45.w));
    EXPECT_TRUE(SympConv::RealEqual(product4b.x, qX45.x));
    EXPECT_TRUE(SympConv::RealEqual(product4b.y, qX45.y));
    EXPECT_TRUE(SympConv::RealEqual(product4b.z, qX45.z));
    EXPECT_TRUE(SympConv::RealEqual(product4b.w, qX45.w));
}

TEST(QuaternionTest, ScalarMultiplication) {
    Quaternion q(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Real scalar = Real(2.0);
    Quaternion product1 = q * scalar;
    Quaternion product2 = scalar * q;
    EXPECT_TRUE(SympConv::RealEqual(product1.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(product1.y, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(product1.z, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(product1.w, Real(8.0)));
    EXPECT_TRUE(SympConv::RealEqual(product2.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(product2.y, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(product2.z, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(product2.w, Real(8.0)));
}

} // namespace SympConvTest
