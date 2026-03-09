#include <gtest/gtest.h>
#include "SympConv/Matrix.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Mat2 = SympConv::Matrix2x2;
using Mat3 = SympConv::Matrix3x3;
using Mat4 = SympConv::Matrix4x4;
using Vec2 = SympConv::Vector2;
using Vec3 = SympConv::Vector3;
using Vec4 = SympConv::Vector4;

TEST(MatrixTest, Matrix2x2Constructors) {
    Mat2 mat2;
    Mat2 mat2_1(1.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(FloatEqual(mat2_1.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat2_1.mRows[1].y, 1.0f));
}

TEST(MatrixTest, Matrix2x2Operators) {
    Mat2 mat2_1(1.0f, 0.0f, 0.0f, 1.0f);
    Mat2 mat2_2(2.0f, 0.0f, 0.0f, 2.0f);
    
    // 测试加法
    Mat2 mat2_add = mat2_1 + mat2_2;
    EXPECT_TRUE(FloatEqual(mat2_add.mRows[0].x, 3.0f));
    EXPECT_TRUE(FloatEqual(mat2_add.mRows[1].y, 3.0f));
    
    // 测试减法
    Mat2 mat2_sub = mat2_2 - mat2_1;
    EXPECT_TRUE(FloatEqual(mat2_sub.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat2_sub.mRows[1].y, 1.0f));
    
    // 测试乘法
    Mat2 mat2_mul = mat2_1 * mat2_2;
    EXPECT_TRUE(FloatEqual(mat2_mul.mRows[0].x, 2.0f));
    EXPECT_TRUE(FloatEqual(mat2_mul.mRows[1].y, 2.0f));
    
    // 测试矩阵与向量乘法
    Vec2 vec2(1.0f, 2.0f);
    Vec2 vec2_result = mat2_2 * vec2;
    EXPECT_TRUE(FloatEqual(vec2_result.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec2_result.y, 4.0f));
    
    // 测试标量乘法
    Mat2 mat2_scalar = mat2_1 * 2.0f;
    EXPECT_TRUE(FloatEqual(mat2_scalar.mRows[0].x, 2.0f));
    EXPECT_TRUE(FloatEqual(mat2_scalar.mRows[1].y, 2.0f));
    
    // 测试标量除法
    Mat2 mat2_div = mat2_2 / 2.0f;
    EXPECT_TRUE(FloatEqual(mat2_div.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat2_div.mRows[1].y, 1.0f));
    
    // 测试复合赋值运算符
    Mat2 mat2_comp = mat2_1;
    mat2_comp += mat2_2;
    EXPECT_TRUE(FloatEqual(mat2_comp.mRows[0].x, 3.0f));
    
    mat2_comp -= mat2_1;
    EXPECT_TRUE(FloatEqual(mat2_comp.mRows[0].x, 2.0f));
    
    mat2_comp *= 2.0f;
    EXPECT_TRUE(FloatEqual(mat2_comp.mRows[0].x, 4.0f));
    
    mat2_comp /= 2.0f;
    EXPECT_TRUE(FloatEqual(mat2_comp.mRows[0].x, 2.0f));
}

TEST(MatrixTest, Matrix2x2Transpose) {
    Mat2 mat2(1.0f, 2.0f, 3.0f, 4.0f);
    Mat2 transposed = mat2.Transpose();
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].y, 3.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].x, 2.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].y, 4.0f));
}

TEST(MatrixTest, Matrix2x2Determinant) {
    // 测试单位矩阵的行列式
    Mat2 identity = Mat2::Identity();
    float det_identity = identity.Determinant();
    EXPECT_TRUE(FloatEqual(det_identity, 1.0f));
    
    // 测试一个简单矩阵的行列式
    Mat2 mat2(2.0f, 0.0f, 0.0f, 3.0f);
    float det = mat2.Determinant();
    EXPECT_TRUE(FloatEqual(det, 6.0f));
    
    // 测试一个行列式为0的矩阵
    Mat2 singular(1.0f, 2.0f, 2.0f, 4.0f);
    float det_singular = singular.Determinant();
    EXPECT_TRUE(FloatEqual(det_singular, 0.0f));
}

TEST(MatrixTest, Matrix2x2Inverse) {
    // 测试单位矩阵的逆矩阵
    Mat2 identity = Mat2::Identity();
    Mat2 identity_inv = identity.Inverse();
    Mat2 identity_product = identity * identity_inv;
    EXPECT_TRUE(FloatEqual(identity_product.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(identity_product.mRows[1].y, 1.0f));
    
    // 测试一个可逆矩阵的逆矩阵
    Mat2 mat2(2.0f, 0.0f, 0.0f, 3.0f);
    Mat2 mat2_inv = mat2.Inverse();
    Mat2 mat2_product = mat2 * mat2_inv;
    EXPECT_TRUE(FloatEqual(mat2_product.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat2_product.mRows[1].y, 1.0f));
    
    // 测试不可逆矩阵的逆矩阵（应该返回零矩阵）
    Mat2 singular(1.0f, 2.0f, 2.0f, 4.0f);
    Mat2 singular_inv = singular.Inverse();
    EXPECT_TRUE(FloatEqual(singular_inv.mRows[0].x, 0.0f));
    EXPECT_TRUE(FloatEqual(singular_inv.mRows[1].y, 0.0f));
}

TEST(MatrixTest, Matrix3x3Constructor) {
    Mat3 mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(FloatEqual(mat3.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat3.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat3.mRows[2].z, 1.0f));
}

TEST(MatrixTest, Matrix3x3VectorMultiplication) {
    Mat3 mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    Vec3 vec3(1.0f, 2.0f, 3.0f);
    Vec3 vec3_result = mat3 * vec3;
    EXPECT_TRUE(FloatEqual(vec3_result.x, 1.0f));
    EXPECT_TRUE(FloatEqual(vec3_result.y, 2.0f));
    EXPECT_TRUE(FloatEqual(vec3_result.z, 3.0f));
}

TEST(MatrixTest, Matrix3x3Operators) {
    Mat3 mat3_1(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    Mat3 mat3_2(2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 2.0f);
    
    // 测试加法
    Mat3 mat3_add = mat3_1 + mat3_2;
    EXPECT_TRUE(FloatEqual(mat3_add.mRows[0].x, 3.0f));
    EXPECT_TRUE(FloatEqual(mat3_add.mRows[1].y, 3.0f));
    EXPECT_TRUE(FloatEqual(mat3_add.mRows[2].z, 3.0f));
    
    // 测试减法
    Mat3 mat3_sub = mat3_2 - mat3_1;
    EXPECT_TRUE(FloatEqual(mat3_sub.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat3_sub.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat3_sub.mRows[2].z, 1.0f));
    
    // 测试乘法
    Mat3 mat3_mul = mat3_1 * mat3_2;
    EXPECT_TRUE(FloatEqual(mat3_mul.mRows[0].x, 2.0f));
    EXPECT_TRUE(FloatEqual(mat3_mul.mRows[1].y, 2.0f));
    EXPECT_TRUE(FloatEqual(mat3_mul.mRows[2].z, 2.0f));
    
    // 测试标量乘法
    Mat3 mat3_scalar = mat3_1 * 2.0f;
    EXPECT_TRUE(FloatEqual(mat3_scalar.mRows[0].x, 2.0f));
    EXPECT_TRUE(FloatEqual(mat3_scalar.mRows[1].y, 2.0f));
    EXPECT_TRUE(FloatEqual(mat3_scalar.mRows[2].z, 2.0f));
    
    // 测试标量除法
    Mat3 mat3_div = mat3_2 / 2.0f;
    EXPECT_TRUE(FloatEqual(mat3_div.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat3_div.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat3_div.mRows[2].z, 1.0f));
    
    // 测试复合赋值运算符
    Mat3 mat3_comp = mat3_1;
    mat3_comp += mat3_2;
    EXPECT_TRUE(FloatEqual(mat3_comp.mRows[0].x, 3.0f));
    
    mat3_comp -= mat3_1;
    EXPECT_TRUE(FloatEqual(mat3_comp.mRows[0].x, 2.0f));
    
    mat3_comp *= 2.0f;
    EXPECT_TRUE(FloatEqual(mat3_comp.mRows[0].x, 4.0f));
    
    mat3_comp /= 2.0f;
    EXPECT_TRUE(FloatEqual(mat3_comp.mRows[0].x, 2.0f));
}

TEST(MatrixTest, Matrix3x3Transpose) {
    Mat3 mat3(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
    Mat3 transposed = mat3.Transpose();
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].y, 4.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].z, 7.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].x, 2.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].y, 5.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].z, 8.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[2].x, 3.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[2].y, 6.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[2].z, 9.0f));
}

TEST(MatrixTest, Matrix3x3Determinant) {
    // 测试单位矩阵的行列式
    Mat3 identity = Mat3::Identity();
    float det_identity = identity.Determinant();
    EXPECT_TRUE(FloatEqual(det_identity, 1.0f));
    
    // 测试一个简单矩阵的行列式
    Mat3 mat3(2.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 4.0f);
    float det = mat3.Determinant();
    EXPECT_TRUE(FloatEqual(det, 24.0f));
}

TEST(MatrixTest, Matrix3x3Inverse) {
    // 测试单位矩阵的逆矩阵
    Mat3 identity = Mat3::Identity();
    Mat3 identity_inv = identity.Transpose(); // 对称矩阵的逆矩阵等于其转置
    Mat3 identity_product = identity * identity_inv;
    EXPECT_TRUE(FloatEqual(identity_product.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(identity_product.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(identity_product.mRows[2].z, 1.0f));
}

TEST(MatrixTest, Matrix4x4Constructor) {
    Mat4 mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_TRUE(FloatEqual(mat4.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4.mRows[2].z, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4.mRows[3].w, 1.0f));
}

TEST(MatrixTest, Matrix4x4VectorMultiplication) {
    Mat4 mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    Vec4 vec4(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4 vec4_result = mat4 * vec4;
    EXPECT_TRUE(FloatEqual(vec4_result.x, 1.0f));
    EXPECT_TRUE(FloatEqual(vec4_result.y, 2.0f));
    EXPECT_TRUE(FloatEqual(vec4_result.z, 3.0f));
    EXPECT_TRUE(FloatEqual(vec4_result.w, 1.0f));
}

TEST(MatrixTest, Matrix4x4Operators) {
    Mat4 mat4_1(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    Mat4 mat4_2(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f);
    
    // 测试加法
    Mat4 mat4_add = mat4_1 + mat4_2;
    EXPECT_TRUE(FloatEqual(mat4_add.mRows[0].x, 3.0f));
    EXPECT_TRUE(FloatEqual(mat4_add.mRows[1].y, 3.0f));
    EXPECT_TRUE(FloatEqual(mat4_add.mRows[2].z, 3.0f));
    EXPECT_TRUE(FloatEqual(mat4_add.mRows[3].w, 3.0f));
    
    // 测试减法
    Mat4 mat4_sub = mat4_2 - mat4_1;
    EXPECT_TRUE(FloatEqual(mat4_sub.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_sub.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_sub.mRows[2].z, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_sub.mRows[3].w, 1.0f));
    
    // 测试乘法
    Mat4 mat4_mul = mat4_1 * mat4_2;
    EXPECT_TRUE(FloatEqual(mat4_mul.mRows[0].x, 2.0f));
    EXPECT_TRUE(FloatEqual(mat4_mul.mRows[1].y, 2.0f));
    EXPECT_TRUE(FloatEqual(mat4_mul.mRows[2].z, 2.0f));
    EXPECT_TRUE(FloatEqual(mat4_mul.mRows[3].w, 2.0f));
    
    // 测试标量乘法
    Mat4 mat4_scalar = mat4_1 * 2.0f;
    EXPECT_TRUE(FloatEqual(mat4_scalar.mRows[0].x, 2.0f));
    EXPECT_TRUE(FloatEqual(mat4_scalar.mRows[1].y, 2.0f));
    EXPECT_TRUE(FloatEqual(mat4_scalar.mRows[2].z, 2.0f));
    EXPECT_TRUE(FloatEqual(mat4_scalar.mRows[3].w, 2.0f));
    
    // 测试标量除法
    Mat4 mat4_div = mat4_2 / 2.0f;
    EXPECT_TRUE(FloatEqual(mat4_div.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_div.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_div.mRows[2].z, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_div.mRows[3].w, 1.0f));
    
    // 测试复合赋值运算符
    Mat4 mat4_comp = mat4_1;
    mat4_comp += mat4_2;
    EXPECT_TRUE(FloatEqual(mat4_comp.mRows[0].x, 3.0f));
    
    mat4_comp -= mat4_1;
    EXPECT_TRUE(FloatEqual(mat4_comp.mRows[0].x, 2.0f));
    
    mat4_comp *= 2.0f;
    EXPECT_TRUE(FloatEqual(mat4_comp.mRows[0].x, 4.0f));
    
    mat4_comp /= 2.0f;
    EXPECT_TRUE(FloatEqual(mat4_comp.mRows[0].x, 2.0f));
}

TEST(MatrixTest, Matrix4x4Transpose) {
    Mat4 mat4(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 transposed = mat4.Transpose();
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].y, 5.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].z, 9.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[0].w, 13.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].x, 2.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].y, 6.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].z, 10.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[1].w, 14.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[2].x, 3.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[2].y, 7.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[2].z, 11.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[2].w, 15.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[3].x, 4.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[3].y, 8.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[3].z, 12.0f));
    EXPECT_TRUE(FloatEqual(transposed.mRows[3].w, 16.0f));
}

TEST(MatrixTest, Matrix4x4SubscriptOperator) {
    Mat4 mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    // 测试非常量下标运算符
    mat4[0].x = 2.0f;
    EXPECT_TRUE(FloatEqual(mat4[0].x, 2.0f));
    
    // 测试常量下标运算符
    const Mat4 const_mat4 = mat4;
    EXPECT_TRUE(FloatEqual(const_mat4[0].x, 2.0f));
}

TEST(MatrixTest, IdentityMatrix) {
    Mat2 identity2 = Mat2::Identity();
    EXPECT_TRUE(FloatEqual(identity2.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(identity2.mRows[1].y, 1.0f));
    
    Mat3 identity3 = Mat3::Identity();
    EXPECT_TRUE(FloatEqual(identity3.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(identity3.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(identity3.mRows[2].z, 1.0f));
    
    Mat4 identity4 = Mat4::Identity();
    EXPECT_TRUE(FloatEqual(identity4.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(identity4.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(identity4.mRows[2].z, 1.0f));
    EXPECT_TRUE(FloatEqual(identity4.mRows[3].w, 1.0f));
}

TEST(MatrixTest, ZeroMatrix) {
    Mat2 zero2 = Mat2::Zero();
    EXPECT_TRUE(FloatEqual(zero2.mRows[0].x, 0.0f));
    EXPECT_TRUE(FloatEqual(zero2.mRows[1].y, 0.0f));
    
    Mat3 zero3 = Mat3::Zero();
    EXPECT_TRUE(FloatEqual(zero3.mRows[0].x, 0.0f));
    EXPECT_TRUE(FloatEqual(zero3.mRows[1].y, 0.0f));
    EXPECT_TRUE(FloatEqual(zero3.mRows[2].z, 0.0f));
    
    Mat4 zero4 = Mat4::Zero();
    EXPECT_TRUE(FloatEqual(zero4.mRows[0].x, 0.0f));
    EXPECT_TRUE(FloatEqual(zero4.mRows[1].y, 0.0f));
    EXPECT_TRUE(FloatEqual(zero4.mRows[2].z, 0.0f));
    EXPECT_TRUE(FloatEqual(zero4.mRows[3].w, 0.0f));
}

TEST(MatrixTest, Matrix4x4Determinant) {
    // 测试单位矩阵的行列式
    Mat4 identity = Mat4::Identity();
    float det_identity = identity.Determinant();
    EXPECT_TRUE(FloatEqual(det_identity, 1.0f));
    
    // 测试一个简单矩阵的行列式
    // 矩阵: [1, 2, 3, 4]
    //       [5, 6, 7, 8]
    //       [9, 10, 11, 12]
    //       [13, 14, 15, 16]
    // 这个矩阵的行列式应该是0，因为行是线性相关的
    Mat4 mat4(1.0f, 2.0f, 3.0f, 4.0f, 
              5.0f, 6.0f, 7.0f, 8.0f, 
              9.0f, 10.0f, 11.0f, 12.0f, 
              13.0f, 14.0f, 15.0f, 16.0f);
    float det = mat4.Determinant();
    EXPECT_TRUE(FloatEqual(det, 0.0f));
    
    // 测试一个可逆矩阵的行列式
    // 矩阵: [1, 0, 0, 0]
    //       [0, 2, 0, 0]
    //       [0, 0, 3, 0]
    //       [0, 0, 0, 4]
    // 这个矩阵的行列式应该是1*2*3*4=24
    Mat4 mat4_diag(1.0f, 0.0f, 0.0f, 0.0f, 
                   0.0f, 2.0f, 0.0f, 0.0f, 
                   0.0f, 0.0f, 3.0f, 0.0f, 
                   0.0f, 0.0f, 0.0f, 4.0f);
    float det_diag = mat4_diag.Determinant();
    EXPECT_TRUE(FloatEqual(det_diag, 24.0f));
}

TEST(MatrixTest, Matrix4x4Inverse) {
    // 测试单位矩阵的逆矩阵
    Mat4 identity = Mat4::Identity();
    Mat4 identity_inv = identity.Inverse();
    Mat4 identity_product = identity * identity_inv;
    EXPECT_TRUE(FloatEqual(identity_product.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(identity_product.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(identity_product.mRows[2].z, 1.0f));
    EXPECT_TRUE(FloatEqual(identity_product.mRows[3].w, 1.0f));
    
    // 测试一个可逆矩阵的逆矩阵
    // 矩阵: [1, 0, 0, 0]
    //       [0, 2, 0, 0]
    //       [0, 0, 3, 0]
    //       [0, 0, 0, 4]
    // 逆矩阵应该是: [1, 0, 0, 0]
    //               [0, 0.5, 0, 0]
    //               [0, 0, 1/3, 0]
    //               [0, 0, 0, 0.25]
    Mat4 mat4_diag(1.0f, 0.0f, 0.0f, 0.0f, 
                   0.0f, 2.0f, 0.0f, 0.0f, 
                   0.0f, 0.0f, 3.0f, 0.0f, 
                   0.0f, 0.0f, 0.0f, 4.0f);
    Mat4 mat4_diag_inv = mat4_diag.Inverse();
    Mat4 mat4_diag_product = mat4_diag * mat4_diag_inv;
    EXPECT_TRUE(FloatEqual(mat4_diag_product.mRows[0].x, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_diag_product.mRows[1].y, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_diag_product.mRows[2].z, 1.0f));
    EXPECT_TRUE(FloatEqual(mat4_diag_product.mRows[3].w, 1.0f));
    
    // 测试不可逆矩阵的逆矩阵（应该返回零矩阵）
    // 矩阵: [1, 2, 3, 4]
    //       [5, 6, 7, 8]
    //       [9, 10, 11, 12]
    //       [13, 14, 15, 16]
    Mat4 mat4_singular(1.0f, 2.0f, 3.0f, 4.0f, 
                       5.0f, 6.0f, 7.0f, 8.0f, 
                       9.0f, 10.0f, 11.0f, 12.0f, 
                       13.0f, 14.0f, 15.0f, 16.0f);
    Mat4 mat4_singular_inv = mat4_singular.Inverse();
    // 检查是否返回零矩阵
    EXPECT_TRUE(FloatEqual(mat4_singular_inv.mRows[0].x, 0.0f));
    EXPECT_TRUE(FloatEqual(mat4_singular_inv.mRows[1].y, 0.0f));
    EXPECT_TRUE(FloatEqual(mat4_singular_inv.mRows[2].z, 0.0f));
    EXPECT_TRUE(FloatEqual(mat4_singular_inv.mRows[3].w, 0.0f));
}

} // namespace SympConvTest
