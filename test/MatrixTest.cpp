#include <gtest/gtest.h>
#define _USE_MATH_DEFINES
#include <cmath>

// 定义M_PI的值，以确保在所有编译器环境中都能使用
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "SympConv/Matrix.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Mat2 = SympConv::Matrix2x2;
using Mat3 = SympConv::Matrix3x3;
using Mat4 = SympConv::Matrix4x4;
using Vec2 = SympConv::Vector2;
using Vec3 = SympConv::Vector3;
using Vec4 = SympConv::Vector4;

TEST(MatrixTest, Matrix2x2Constructors) {
    Mat2 mat2;
    Mat2 mat2_1(Real(1.0), Real(0.0), Real(0.0), Real(1.0));
    EXPECT_TRUE(SympConv::RealEqual(mat2_1.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat2_1.mRows[1].y, Real(1.0)));
}

TEST(MatrixTest, Matrix2x2Operators) {
    Mat2 mat2_1(Real(1.0), Real(0.0), Real(0.0), Real(1.0));
    Mat2 mat2_2(Real(2.0), Real(0.0), Real(0.0), Real(2.0));
    
    // 测试加法
    Mat2 mat2_add = mat2_1 + mat2_2;
    EXPECT_TRUE(SympConv::RealEqual(mat2_add.mRows[0].x, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat2_add.mRows[1].y, Real(3.0)));
    
    // 测试减法
    Mat2 mat2_sub = mat2_2 - mat2_1;
    EXPECT_TRUE(SympConv::RealEqual(mat2_sub.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat2_sub.mRows[1].y, Real(1.0)));
    
    // 测试乘法
    Mat2 mat2_mul = mat2_1 * mat2_2;
    EXPECT_TRUE(SympConv::RealEqual(mat2_mul.mRows[0].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat2_mul.mRows[1].y, Real(2.0)));
    
    // 测试矩阵与向量乘法
    Vec2 vec2(Real(1.0), Real(2.0));
    Vec2 vec2_result = mat2_2 * vec2;
    EXPECT_TRUE(SympConv::RealEqual(vec2_result.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec2_result.y, Real(4.0)));
    
    // 测试标量乘法
    Mat2 mat2_scalar = mat2_1 * Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat2_scalar.mRows[0].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat2_scalar.mRows[1].y, Real(2.0)));
    
    // 测试标量除法
    Mat2 mat2_div = mat2_2 / Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat2_div.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat2_div.mRows[1].y, Real(1.0)));
    
    // 测试复合赋值运算符
    Mat2 mat2_comp = mat2_1;
    mat2_comp += mat2_2;
    EXPECT_TRUE(SympConv::RealEqual(mat2_comp.mRows[0].x, Real(3.0)));
    
    mat2_comp -= mat2_1;
    EXPECT_TRUE(SympConv::RealEqual(mat2_comp.mRows[0].x, Real(2.0)));
    
    mat2_comp *= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat2_comp.mRows[0].x, Real(4.0)));
    
    mat2_comp /= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat2_comp.mRows[0].x, Real(2.0)));
}

TEST(MatrixTest, Matrix2x2Transpose) {
    Mat2 mat2(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Mat2 transposed = mat2.Transpose();
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].y, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].y, Real(4.0)));
}

TEST(MatrixTest, Matrix2x2Determinant) {
    // 测试单位矩阵的行列式
    Mat2 identity = Mat2::Identity();
    Real det_identity = identity.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det_identity, Real(1.0)));
    
    // 测试一个简单矩阵的行列式
    Mat2 mat2(Real(2.0), Real(0.0), Real(0.0), Real(3.0));
    Real det = mat2.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det, Real(6.0)));
    
    // 测试一个行列式为0的矩阵
    Mat2 singular(Real(1.0), Real(2.0), Real(2.0), Real(4.0));
    Real det_singular = singular.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det_singular, Real(0.0)));
}

TEST(MatrixTest, Matrix2x2Inverse) {
    // 测试单位矩阵的逆矩阵
    Mat2 identity = Mat2::Identity();
    Mat2 identity_inv = identity.Inverse();
    Mat2 identity_product = identity * identity_inv;
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[1].y, Real(1.0)));
    
    // 测试一个可逆矩阵的逆矩阵
    Mat2 mat2(Real(2.0), Real(0.0), Real(0.0), Real(3.0));
    Mat2 mat2_inv = mat2.Inverse();
    Mat2 mat2_product = mat2 * mat2_inv;
    EXPECT_TRUE(SympConv::RealEqual(mat2_product.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat2_product.mRows[1].y, Real(1.0)));
    
    // 测试不可逆矩阵的逆矩阵（应该返回零矩阵）
    Mat2 singular(Real(1.0), Real(2.0), Real(2.0), Real(4.0));
    Mat2 singular_inv = singular.Inverse();
    EXPECT_TRUE(SympConv::RealEqual(singular_inv.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(singular_inv.mRows[1].y, Real(0.0)));
}

TEST(MatrixTest, Matrix3x3Constructor) {
    Mat3 mat3(Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    EXPECT_TRUE(SympConv::RealEqual(mat3.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3.mRows[2].z, Real(1.0)));
}

TEST(MatrixTest, Matrix3x3VectorMultiplication) {
    Mat3 mat3(Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    Vec3 vec3(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_result = mat3 * vec3;
    EXPECT_TRUE(SympConv::RealEqual(vec3_result.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_result.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_result.z, Real(3.0)));
}

TEST(MatrixTest, Matrix3x3Operators) {
    Mat3 mat3_1(Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    Mat3 mat3_2(Real(2.0), Real(0.0), Real(0.0), Real(0.0), Real(2.0), Real(0.0), Real(0.0), Real(0.0), Real(2.0));
    
    // 测试加法
    Mat3 mat3_add = mat3_1 + mat3_2;
    EXPECT_TRUE(SympConv::RealEqual(mat3_add.mRows[0].x, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_add.mRows[1].y, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_add.mRows[2].z, Real(3.0)));
    
    // 测试减法
    Mat3 mat3_sub = mat3_2 - mat3_1;
    EXPECT_TRUE(SympConv::RealEqual(mat3_sub.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_sub.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_sub.mRows[2].z, Real(1.0)));
    
    // 测试乘法
    Mat3 mat3_mul = mat3_1 * mat3_2;
    EXPECT_TRUE(SympConv::RealEqual(mat3_mul.mRows[0].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_mul.mRows[1].y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_mul.mRows[2].z, Real(2.0)));
    
    // 测试标量乘法
    Mat3 mat3_scalar = mat3_1 * Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat3_scalar.mRows[0].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_scalar.mRows[1].y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_scalar.mRows[2].z, Real(2.0)));
    
    // 测试标量除法
    Mat3 mat3_div = mat3_2 / Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat3_div.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_div.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_div.mRows[2].z, Real(1.0)));
    
    // 测试复合赋值运算符
    Mat3 mat3_comp = mat3_1;
    mat3_comp += mat3_2;
    EXPECT_TRUE(SympConv::RealEqual(mat3_comp.mRows[0].x, Real(3.0)));
    
    mat3_comp -= mat3_1;
    EXPECT_TRUE(SympConv::RealEqual(mat3_comp.mRows[0].x, Real(2.0)));
    
    mat3_comp *= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat3_comp.mRows[0].x, Real(4.0)));
    
    mat3_comp /= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat3_comp.mRows[0].x, Real(2.0)));
}

TEST(MatrixTest, Matrix3x3Transpose) {
    Mat3 mat3(Real(1.0), Real(2.0), Real(3.0), Real(4.0), Real(5.0), Real(6.0), Real(7.0), Real(8.0), Real(9.0));
    Mat3 transposed = mat3.Transpose();
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].y, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].z, Real(7.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].y, Real(5.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].z, Real(8.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[2].x, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[2].y, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[2].z, Real(9.0)));
}

TEST(MatrixTest, Matrix3x3Determinant) {
    // 测试单位矩阵的行列式
    Mat3 identity = Mat3::Identity();
    Real det_identity = identity.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det_identity, Real(1.0)));
    
    // 测试一个简单矩阵的行列式
    Mat3 mat3(Real(2.0), Real(0.0), Real(0.0), Real(0.0), Real(3.0), Real(0.0), Real(0.0), Real(0.0), Real(4.0));
    Real det = mat3.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det, Real(24.0)));
}

TEST(MatrixTest, Matrix3x3Inverse) {
    // 测试单位矩阵的逆矩阵
    Mat3 identity = Mat3::Identity();
    Mat3 identity_inv = identity.Inverse();
    Mat3 identity_product = identity * identity_inv;
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[2].y, Real(0.0)));
    
    // 测试非单位矩阵的逆矩阵
    Mat3 mat3(Real(1.0), Real(2.0), Real(3.0),
              Real(0.0), Real(1.0), Real(4.0),
              Real(5.0), Real(6.0), Real(0.0));
    Mat3 mat3_inv = mat3.Inverse();
    Mat3 mat3_product = mat3 * mat3_inv;
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[0].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[0].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[1].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[1].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[2].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat3_product.mRows[2].y, Real(0.0)));
}

TEST(MatrixTest, Matrix4x4Constructor) {
    Mat4 mat4(Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    EXPECT_TRUE(SympConv::RealEqual(mat4.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4.mRows[3].w, Real(1.0)));
}

TEST(MatrixTest, Matrix4x4VectorMultiplication) {
    Mat4 mat4(Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    Vec4 vec4(Real(1.0), Real(2.0), Real(3.0), Real(1.0));
    Vec4 vec4_result = mat4 * vec4;
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.w, Real(1.0)));
}

TEST(MatrixTest, Matrix4x4Operators) {
    Mat4 mat4_1(Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    Mat4 mat4_2(Real(2.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(2.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(2.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(2.0));
    
    // 测试加法
    Mat4 mat4_add = mat4_1 + mat4_2;
    EXPECT_TRUE(SympConv::RealEqual(mat4_add.mRows[0].x, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_add.mRows[1].y, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_add.mRows[2].z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_add.mRows[3].w, Real(3.0)));
    
    // 测试减法
    Mat4 mat4_sub = mat4_2 - mat4_1;
    EXPECT_TRUE(SympConv::RealEqual(mat4_sub.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_sub.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_sub.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_sub.mRows[3].w, Real(1.0)));
    
    // 测试乘法
    Mat4 mat4_mul = mat4_1 * mat4_2;
    EXPECT_TRUE(SympConv::RealEqual(mat4_mul.mRows[0].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_mul.mRows[1].y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_mul.mRows[2].z, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_mul.mRows[3].w, Real(2.0)));
    
    // 测试标量乘法
    Mat4 mat4_scalar = mat4_1 * Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat4_scalar.mRows[0].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_scalar.mRows[1].y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_scalar.mRows[2].z, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_scalar.mRows[3].w, Real(2.0)));
    
    // 测试标量除法
    Mat4 mat4_div = mat4_2 / Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat4_div.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_div.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_div.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_div.mRows[3].w, Real(1.0)));
    
    // 测试复合赋值运算符
    Mat4 mat4_comp = mat4_1;
    mat4_comp += mat4_2;
    EXPECT_TRUE(SympConv::RealEqual(mat4_comp.mRows[0].x, Real(3.0)));
    
    mat4_comp -= mat4_1;
    EXPECT_TRUE(SympConv::RealEqual(mat4_comp.mRows[0].x, Real(2.0)));
    
    mat4_comp *= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat4_comp.mRows[0].x, Real(4.0)));
    
    mat4_comp /= Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat4_comp.mRows[0].x, Real(2.0)));
}

TEST(MatrixTest, Matrix4x4Transpose) {
    Mat4 mat4(Real(1.0), Real(2.0), Real(3.0), Real(4.0), Real(5.0), Real(6.0), Real(7.0), Real(8.0), Real(9.0), Real(10.0), Real(11.0), Real(12.0), Real(13.0), Real(14.0), Real(15.0), Real(16.0));
    Mat4 transposed = mat4.Transpose();
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].y, Real(5.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].z, Real(9.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[0].w, Real(13.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].y, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].z, Real(10.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[1].w, Real(14.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[2].x, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[2].y, Real(7.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[2].z, Real(11.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[2].w, Real(15.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[3].x, Real(4.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[3].y, Real(8.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[3].z, Real(12.0)));
    EXPECT_TRUE(SympConv::RealEqual(transposed.mRows[3].w, Real(16.0)));
}

TEST(MatrixTest, Matrix4x4SubscriptOperator) {
    Mat4 mat4(Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0), Real(0.0), Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    // 测试非常量下标运算符
    mat4[0].x = Real(2.0);
    EXPECT_TRUE(SympConv::RealEqual(mat4[0].x, Real(2.0)));
    
    // 测试常量下标运算符
    const Mat4 const_mat4 = mat4;
    EXPECT_TRUE(SympConv::RealEqual(const_mat4[0].x, Real(2.0)));
}

TEST(MatrixTest, IdentityMatrix) {
    Mat2 identity2 = Mat2::Identity();
    EXPECT_TRUE(SympConv::RealEqual(identity2.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity2.mRows[1].y, Real(1.0)));
    
    Mat3 identity3 = Mat3::Identity();
    EXPECT_TRUE(SympConv::RealEqual(identity3.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity3.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity3.mRows[2].z, Real(1.0)));
    
    Mat4 identity4 = Mat4::Identity();
    EXPECT_TRUE(SympConv::RealEqual(identity4.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity4.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity4.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity4.mRows[3].w, Real(1.0)));
}

TEST(MatrixTest, ZeroMatrix) {
    Mat2 zero2 = Mat2::Zero();
    EXPECT_TRUE(SympConv::RealEqual(zero2.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(zero2.mRows[1].y, Real(0.0)));
    
    Mat3 zero3 = Mat3::Zero();
    EXPECT_TRUE(SympConv::RealEqual(zero3.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(zero3.mRows[1].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(zero3.mRows[2].z, Real(0.0)));
    
    Mat4 zero4 = Mat4::Zero();
    EXPECT_TRUE(SympConv::RealEqual(zero4.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(zero4.mRows[1].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(zero4.mRows[2].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(zero4.mRows[3].w, Real(0.0)));
}

TEST(MatrixTest, Matrix4x4Determinant) {
    // 测试单位矩阵的行列式
    Mat4 identity = Mat4::Identity();
    Real det_identity = identity.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det_identity, Real(1.0)));
    
    // 测试一个简单矩阵的行列式
    // 矩阵: [1, 2, 3, 4]
    //       [5, 6, 7, 8]
    //       [9, 10, 11, 12]
    //       [13, 14, 15, 16]
    // 这个矩阵的行列式应该是0，因为行是线性相关的
    Mat4 mat4(Real(1.0), Real(2.0), Real(3.0), Real(4.0), 
              Real(5.0), Real(6.0), Real(7.0), Real(8.0), 
              Real(9.0), Real(10.0), Real(11.0), Real(12.0), 
              Real(13.0), Real(14.0), Real(15.0), Real(16.0));
    Real det = mat4.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det, Real(0.0)));
    
    // 测试一个可逆矩阵的行列式
    // 矩阵: [1, 0, 0, 0]
    //       [0, 2, 0, 0]
    //       [0, 0, 3, 0]
    //       [0, 0, 0, 4]
    // 这个矩阵的行列式应该是1*2*3*4=24
    Mat4 mat4_diag(Real(1.0), Real(0.0), Real(0.0), Real(0.0), 
                   Real(0.0), Real(2.0), Real(0.0), Real(0.0), 
                   Real(0.0), Real(0.0), Real(3.0), Real(0.0), 
                   Real(0.0), Real(0.0), Real(0.0), Real(4.0));
    Real det_diag = mat4_diag.Determinant();
    EXPECT_TRUE(SympConv::RealEqual(det_diag, Real(24.0)));
}

TEST(MatrixTest, Matrix4x4Inverse) {
    // 测试单位矩阵的逆矩阵
    Mat4 identity = Mat4::Identity();
    Mat4 identity_inv = identity.Inverse();
    Mat4 identity_product = identity * identity_inv;
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(identity_product.mRows[3].w, Real(1.0)));
    
    // 测试一个可逆矩阵的逆矩阵
    // 矩阵: [1, 0, 0, 0]
    //       [0, 2, 0, 0]
    //       [0, 0, 3, 0]
    //       [0, 0, 0, 4]
    // 逆矩阵应该是: [1, 0, 0, 0]
    //               [0, 0.5, 0, 0]
    //               [0, 0, 1/3, 0]
    //               [0, 0, 0, 0.25]
    Mat4 mat4_diag(Real(1.0), Real(0.0), Real(0.0), Real(0.0), 
                   Real(0.0), Real(2.0), Real(0.0), Real(0.0), 
                   Real(0.0), Real(0.0), Real(3.0), Real(0.0), 
                   Real(0.0), Real(0.0), Real(0.0), Real(4.0));
    Mat4 mat4_diag_inv = mat4_diag.Inverse();
    Mat4 mat4_diag_product = mat4_diag * mat4_diag_inv;
    EXPECT_TRUE(SympConv::RealEqual(mat4_diag_product.mRows[0].x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_diag_product.mRows[1].y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_diag_product.mRows[2].z, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_diag_product.mRows[3].w, Real(1.0)));
    
    // 测试不可逆矩阵的逆矩阵（应该返回零矩阵）
    // 矩阵: [1, 2, 3, 4]
    //       [5, 6, 7, 8]
    //       [9, 10, 11, 12]
    //       [13, 14, 15, 16]
    Mat4 mat4_singular(Real(1.0), Real(2.0), Real(3.0), Real(4.0), 
                       Real(5.0), Real(6.0), Real(7.0), Real(8.0), 
                       Real(9.0), Real(10.0), Real(11.0), Real(12.0), 
                       Real(13.0), Real(14.0), Real(15.0), Real(16.0));
    Mat4 mat4_singular_inv = mat4_singular.Inverse();
    // 检查是否返回零矩阵
    EXPECT_TRUE(SympConv::RealEqual(mat4_singular_inv.mRows[0].x, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_singular_inv.mRows[1].y, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_singular_inv.mRows[2].z, Real(0.0)));
    EXPECT_TRUE(SympConv::RealEqual(mat4_singular_inv.mRows[3].w, Real(0.0)));
}

TEST(MatrixTest, MatrixVectorMultiplication) {
    // 测试2x2矩阵与向量乘法
    Mat2 mat2_identity = Mat2::Identity();
    Vec2 vec2(Real(1.0), Real(2.0));
    Vec2 vec2_result = mat2_identity * vec2;
    EXPECT_TRUE(SympConv::RealEqual(vec2_result.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec2_result.y, Real(2.0)));
    
    // 测试2x2旋转45度矩阵与向量乘法
    Real cos45 = std::cos((Real)M_PI / Real(4.0));
    Real sin45 = std::sin((Real)M_PI / Real(4.0));
    Mat2 mat2_rotate45(cos45, -sin45, sin45, cos45);
    Vec2 vec2_rotated = mat2_rotate45 * vec2;
    Real expected_x = Real(1.0) * cos45 - Real(2.0) * sin45;
    Real expected_y = Real(1.0) * sin45 + Real(2.0) * cos45;
    EXPECT_TRUE(SympConv::RealEqual(vec2_rotated.x, expected_x));
    EXPECT_TRUE(SympConv::RealEqual(vec2_rotated.y, expected_y));
    
    // 测试2x2缩放矩阵与向量乘法
    Mat2 mat2_scale(Real(2.0), Real(0.0), Real(0.0), Real(3.0));
    Vec2 vec2_scaled = mat2_scale * vec2;
    EXPECT_TRUE(SympConv::RealEqual(vec2_scaled.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec2_scaled.y, Real(6.0)));
    
    // 测试3x3矩阵与向量乘法
    Mat3 mat3_identity = Mat3::Identity();
    Vec3 vec3(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_result = mat3_identity * vec3;
    EXPECT_TRUE(SympConv::RealEqual(vec3_result.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_result.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_result.z, Real(3.0)));
    
    // 测试3x3旋转90度矩阵与向量乘法
    Mat3 mat3_rotate90(Real(0.0), Real(-1.0), Real(0.0),
                      Real(1.0), Real(0.0), Real(0.0),
                      Real(0.0), Real(0.0), Real(1.0));
    Vec3 vec3_rotated = mat3_rotate90 * vec3;
    EXPECT_TRUE(SympConv::RealEqual(vec3_rotated.x, Real(-2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_rotated.y, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_rotated.z, Real(3.0)));
    
    // 测试3x3缩放矩阵与向量乘法
    Mat3 mat3_scale(Real(2.0), Real(0.0), Real(0.0),
                    Real(0.0), Real(3.0), Real(0.0),
                    Real(0.0), Real(0.0), Real(4.0));
    Vec3 vec3_scaled = mat3_scale * vec3;
    EXPECT_TRUE(SympConv::RealEqual(vec3_scaled.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_scaled.y, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec3_scaled.z, Real(12.0)));
    
    // 测试4x4矩阵与向量乘法
    Mat4 mat4_identity = Mat4::Identity();
    Vec4 vec4(Real(1.0), Real(2.0), Real(3.0), Real(1.0));
    Vec4 vec4_result = mat4_identity * vec4;
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.x, Real(1.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.y, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_result.w, Real(1.0)));
    
    // 测试4x4旋转180度矩阵与向量乘法
    Mat4 mat4_rotate180(Real(-1.0), Real(0.0), Real(0.0), Real(0.0),
                        Real(0.0), Real(-1.0), Real(0.0), Real(0.0),
                        Real(0.0), Real(0.0), Real(1.0), Real(0.0),
                        Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    Vec4 vec4_rotated = mat4_rotate180 * vec4;
    EXPECT_TRUE(SympConv::RealEqual(vec4_rotated.x, Real(-1.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_rotated.y, Real(-2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_rotated.z, Real(3.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_rotated.w, Real(1.0)));
    
    // 测试4x4缩放矩阵与向量乘法
    Mat4 mat4_scale(Real(2.0), Real(0.0), Real(0.0), Real(0.0),
                    Real(0.0), Real(3.0), Real(0.0), Real(0.0),
                    Real(0.0), Real(0.0), Real(4.0), Real(0.0),
                    Real(0.0), Real(0.0), Real(0.0), Real(1.0));
    Vec4 vec4_scaled = mat4_scale * vec4;
    EXPECT_TRUE(SympConv::RealEqual(vec4_scaled.x, Real(2.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_scaled.y, Real(6.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_scaled.z, Real(12.0)));
    EXPECT_TRUE(SympConv::RealEqual(vec4_scaled.w, Real(1.0)));
}

} // namespace SympConvTest
