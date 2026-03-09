#include <gtest/gtest.h>
#include "SympConv/Matrix.h"
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Mat2 = SympConv::TMatrix2x2<float>;
using Mat3 = SympConv::TMatrix3x3<float>;
using Mat4 = SympConv::TMatrix4x4<float>;
using Vec2 = SympConv::TVector2<float>;
using Vec3 = SympConv::TVector3<float>;
using Vec4 = SympConv::TVector4<float>;

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

} // namespace SympConvTest
