#include <gtest/gtest.h>
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Real = SympConv::Real;
using Vec2 = SympConv::Vector2;
using Vec3 = SympConv::Vector3;
using Vec4 = SympConv::Vector4;

TEST(VectorTest, Vector2Constructor) {
    Vec2 vec2;
    Vec2 vec2_1(Real(1.0), Real(2.0));
    EXPECT_TRUE(FloatEqual(vec2_1.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec2_1.y, Real(2.0)));
}

TEST(VectorTest, Vector2Operators) {
    Vec2 vec2_1(Real(1.0), Real(2.0));
    Vec2 vec2_2(Real(3.0), Real(4.0));
    
    // 测试加法
    Vec2 vec2_add = vec2_1 + vec2_2;
    EXPECT_TRUE(FloatEqual(vec2_add.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec2_add.y, Real(6.0)));
    
    // 测试减法
    Vec2 vec2_sub = vec2_2 - vec2_1;
    EXPECT_TRUE(FloatEqual(vec2_sub.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec2_sub.y, Real(2.0)));
    
    // 测试标量乘法
    Vec2 vec2_scalar = vec2_1 * Real(2.0);
    EXPECT_TRUE(FloatEqual(vec2_scalar.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec2_scalar.y, Real(4.0)));
    
    // 测试标量除法
    Vec2 vec2_div = vec2_1 / Real(2.0);
    EXPECT_TRUE(FloatEqual(vec2_div.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(vec2_div.y, Real(1.0)));
}

TEST(VectorTest, Vector2CompoundOperators) {
    Vec2 vec2_1(Real(1.0), Real(2.0));
    Vec2 vec2_2(Real(3.0), Real(4.0));
    
    // 测试复合加法
    Vec2 vec2_3 = vec2_1;
    vec2_3 += vec2_2;
    EXPECT_TRUE(FloatEqual(vec2_3.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec2_3.y, Real(6.0)));
    
    // 测试复合减法
    Vec2 vec2_4 = vec2_2;
    vec2_4 -= vec2_1;
    EXPECT_TRUE(FloatEqual(vec2_4.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec2_4.y, Real(2.0)));
    
    // 测试复合乘法
    Vec2 vec2_5 = vec2_1;
    vec2_5 *= Real(2.0);
    EXPECT_TRUE(FloatEqual(vec2_5.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec2_5.y, Real(4.0)));
    
    // 测试复合除法
    Vec2 vec2_6 = vec2_1;
    vec2_6 /= Real(2.0);
    EXPECT_TRUE(FloatEqual(vec2_6.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(vec2_6.y, Real(1.0)));
}

TEST(VectorTest, Vector2UnaryMinus) {
    Vec2 vec2_1(Real(1.0), Real(2.0));
    Vec2 vec2_neg = -vec2_1;
    EXPECT_TRUE(FloatEqual(vec2_neg.x, -Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec2_neg.y, -Real(2.0)));
}

TEST(VectorTest, Vector2DotProduct) {
    Vec2 vec2_1(Real(1.0), Real(2.0));
    Vec2 vec2_2(Real(3.0), Real(4.0));
    
    Real dot = vec2_1.Dot(vec2_2);
    EXPECT_TRUE(FloatEqual(dot, Real(1.0) * Real(3.0) + Real(2.0) * Real(4.0)));
    
    Real dotProduct = Vec2::DotProduct(vec2_1, vec2_2);
    EXPECT_TRUE(FloatEqual(dotProduct, Real(1.0) * Real(3.0) + Real(2.0) * Real(4.0)));
}

TEST(VectorTest, Vector2Length) {
    Vec2 vec2_1(Real(1.0), Real(2.0));
    
    Real lengthSquared = vec2_1.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared, Real(1.0) * Real(1.0) + Real(2.0) * Real(2.0)));
    
    Real length = vec2_1.Length();
    EXPECT_TRUE(FloatEqual(length, std::sqrt(Real(5.0))));
}

TEST(VectorTest, Vector2Normalize) {
    Vec2 vec2_1(Real(1.0), Real(2.0));
    Vec2 normalized = vec2_1.Normalize();
    EXPECT_TRUE(FloatEqual(normalized.Length(), Real(1.0)));
}

TEST(VectorTest, Vector2Lerp) {
    Vec2 vec2_1(Real(1.0), Real(2.0));
    Vec2 vec2_2(Real(3.0), Real(4.0));
    Vec2 lerp = Vec2::Lerp(vec2_1, vec2_2, Real(0.5));
    EXPECT_TRUE(FloatEqual(lerp.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(lerp.y, Real(3.0)));
}

TEST(VectorTest, Vector3Constructor) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    EXPECT_TRUE(FloatEqual(vec3_1.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec3_1.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec3_1.z, Real(3.0)));
}

TEST(VectorTest, Vector3Operators) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_2(Real(4.0), Real(5.0), Real(6.0));
    
    // 测试加法
    Vec3 vec3_add = vec3_1 + vec3_2;
    EXPECT_TRUE(FloatEqual(vec3_add.x, Real(5.0)));
    EXPECT_TRUE(FloatEqual(vec3_add.y, Real(7.0)));
    EXPECT_TRUE(FloatEqual(vec3_add.z, Real(9.0)));
    
    // 测试减法
    Vec3 vec3_sub = vec3_2 - vec3_1;
    EXPECT_TRUE(FloatEqual(vec3_sub.x, Real(3.0)));
    EXPECT_TRUE(FloatEqual(vec3_sub.y, Real(3.0)));
    EXPECT_TRUE(FloatEqual(vec3_sub.z, Real(3.0)));
    
    // 测试标量乘法
    Vec3 vec3_scalar = vec3_1 * Real(2.0);
    EXPECT_TRUE(FloatEqual(vec3_scalar.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec3_scalar.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec3_scalar.z, Real(6.0)));
    
    // 测试分量乘法
    Vec3 vec3_component = vec3_1 * vec3_2;
    EXPECT_TRUE(FloatEqual(vec3_component.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec3_component.y, Real(10.0)));
    EXPECT_TRUE(FloatEqual(vec3_component.z, Real(18.0)));
    
    // 测试标量除法
    Vec3 vec3_div = vec3_1 / Real(2.0);
    EXPECT_TRUE(FloatEqual(vec3_div.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(vec3_div.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec3_div.z, Real(1.5)));
}

TEST(VectorTest, Vector3CompoundOperators) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_2(Real(4.0), Real(5.0), Real(6.0));
    
    // 测试复合加法
    Vec3 vec3_3 = vec3_1;
    vec3_3 += vec3_2;
    EXPECT_TRUE(FloatEqual(vec3_3.x, Real(5.0)));
    EXPECT_TRUE(FloatEqual(vec3_3.y, Real(7.0)));
    EXPECT_TRUE(FloatEqual(vec3_3.z, Real(9.0)));
    
    // 测试复合减法
    Vec3 vec3_4 = vec3_2;
    vec3_4 -= vec3_1;
    EXPECT_TRUE(FloatEqual(vec3_4.x, Real(3.0)));
    EXPECT_TRUE(FloatEqual(vec3_4.y, Real(3.0)));
    EXPECT_TRUE(FloatEqual(vec3_4.z, Real(3.0)));
    
    // 测试复合乘法
    Vec3 vec3_5 = vec3_1;
    vec3_5 *= Real(2.0);
    EXPECT_TRUE(FloatEqual(vec3_5.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec3_5.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec3_5.z, Real(6.0)));
    
    // 测试复合除法
    Vec3 vec3_6 = vec3_1;
    vec3_6 /= Real(2.0);
    EXPECT_TRUE(FloatEqual(vec3_6.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(vec3_6.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec3_6.z, Real(1.5)));
}

TEST(VectorTest, Vector3UnaryMinus) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_neg = -vec3_1;
    EXPECT_TRUE(FloatEqual(vec3_neg.x, -Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec3_neg.y, -Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec3_neg.z, -Real(3.0)));
}

TEST(VectorTest, Vector3DotProduct) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_2(Real(4.0), Real(5.0), Real(6.0));
    
    Real dot3 = vec3_1.Dot(vec3_2);
    EXPECT_TRUE(FloatEqual(dot3, Real(1.0) * Real(4.0) + Real(2.0) * Real(5.0) + Real(3.0) * Real(6.0)));
    
    Real dotProduct3 = Vec3::DotProduct(vec3_1, vec3_2);
    EXPECT_TRUE(FloatEqual(dotProduct3, Real(1.0) * Real(4.0) + Real(2.0) * Real(5.0) + Real(3.0) * Real(6.0)));
}

TEST(VectorTest, Vector3CrossProduct) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_2(Real(4.0), Real(5.0), Real(6.0));
    
    Vec3 cross = vec3_1.Cross(vec3_2);
    EXPECT_TRUE(FloatEqual(cross.x, Real(2.0) * Real(6.0) - Real(3.0) * Real(5.0)));
    EXPECT_TRUE(FloatEqual(cross.y, Real(3.0) * Real(4.0) - Real(1.0) * Real(6.0)));
    EXPECT_TRUE(FloatEqual(cross.z, Real(1.0) * Real(5.0) - Real(2.0) * Real(4.0)));
    
    Vec3 crossProduct = Vec3::CrossProduct(vec3_1, vec3_2);
    EXPECT_TRUE(FloatEqual(crossProduct.x, Real(2.0) * Real(6.0) - Real(3.0) * Real(5.0)));
    EXPECT_TRUE(FloatEqual(crossProduct.y, Real(3.0) * Real(4.0) - Real(1.0) * Real(6.0)));
    EXPECT_TRUE(FloatEqual(crossProduct.z, Real(1.0) * Real(5.0) - Real(2.0) * Real(4.0)));
}

TEST(VectorTest, Vector3Length) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    
    Real lengthSquared3 = vec3_1.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared3, Real(1.0) * Real(1.0) + Real(2.0) * Real(2.0) + Real(3.0) * Real(3.0)));
    
    Real length3 = vec3_1.Length();
    EXPECT_TRUE(FloatEqual(length3, std::sqrt(Real(14.0))));
}

TEST(VectorTest, Vector3Normalize) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    Vec3 normalized3 = vec3_1.Normalize();
    EXPECT_TRUE(FloatEqual(normalized3.Length(), Real(1.0)));
}

TEST(VectorTest, Vector3Lerp) {
    Vec3 vec3_1(Real(1.0), Real(2.0), Real(3.0));
    Vec3 vec3_2(Real(4.0), Real(5.0), Real(6.0));
    Vec3 lerp3 = Vec3::Lerp(vec3_1, vec3_2, Real(0.5));
    EXPECT_TRUE(FloatEqual(lerp3.x, Real(2.5)));
    EXPECT_TRUE(FloatEqual(lerp3.y, Real(3.5)));
    EXPECT_TRUE(FloatEqual(lerp3.z, Real(4.5)));
}

TEST(VectorTest, Vector4Constructor) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    EXPECT_TRUE(FloatEqual(vec4_1.x, Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec4_1.y, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec4_1.z, Real(3.0)));
    EXPECT_TRUE(FloatEqual(vec4_1.w, Real(4.0)));
}

TEST(VectorTest, Vector4Operators) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Vec4 vec4_2(Real(5.0), Real(6.0), Real(7.0), Real(8.0));
    
    // 测试加法
    Vec4 vec4_add = vec4_1 + vec4_2;
    EXPECT_TRUE(FloatEqual(vec4_add.x, Real(6.0)));
    EXPECT_TRUE(FloatEqual(vec4_add.y, Real(8.0)));
    EXPECT_TRUE(FloatEqual(vec4_add.z, Real(10.0)));
    EXPECT_TRUE(FloatEqual(vec4_add.w, Real(12.0)));
    
    // 测试减法
    Vec4 vec4_sub = vec4_2 - vec4_1;
    EXPECT_TRUE(FloatEqual(vec4_sub.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_sub.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_sub.z, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_sub.w, Real(4.0)));
    
    // 测试标量乘法
    Vec4 vec4_scalar = vec4_1 * Real(2.0);
    EXPECT_TRUE(FloatEqual(vec4_scalar.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec4_scalar.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_scalar.z, Real(6.0)));
    EXPECT_TRUE(FloatEqual(vec4_scalar.w, Real(8.0)));
    
    // 测试标量除法
    Vec4 vec4_div = vec4_1 / Real(2.0);
    EXPECT_TRUE(FloatEqual(vec4_div.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(vec4_div.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec4_div.z, Real(1.5)));
    EXPECT_TRUE(FloatEqual(vec4_div.w, Real(2.0)));
}

TEST(VectorTest, Vector4CompoundOperators) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Vec4 vec4_2(Real(5.0), Real(6.0), Real(7.0), Real(8.0));
    
    // 测试复合加法
    Vec4 vec4_3 = vec4_1;
    vec4_3 += vec4_2;
    EXPECT_TRUE(FloatEqual(vec4_3.x, Real(6.0)));
    EXPECT_TRUE(FloatEqual(vec4_3.y, Real(8.0)));
    EXPECT_TRUE(FloatEqual(vec4_3.z, Real(10.0)));
    EXPECT_TRUE(FloatEqual(vec4_3.w, Real(12.0)));
    
    // 测试复合减法
    Vec4 vec4_4 = vec4_2;
    vec4_4 -= vec4_1;
    EXPECT_TRUE(FloatEqual(vec4_4.x, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_4.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_4.z, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_4.w, Real(4.0)));
    
    // 测试复合乘法
    Vec4 vec4_5 = vec4_1;
    vec4_5 *= Real(2.0);
    EXPECT_TRUE(FloatEqual(vec4_5.x, Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec4_5.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(vec4_5.z, Real(6.0)));
    EXPECT_TRUE(FloatEqual(vec4_5.w, Real(8.0)));
    
    // 测试复合除法
    Vec4 vec4_6 = vec4_1;
    vec4_6 /= Real(2.0);
    EXPECT_TRUE(FloatEqual(vec4_6.x, Real(0.5)));
    EXPECT_TRUE(FloatEqual(vec4_6.y, Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec4_6.z, Real(1.5)));
    EXPECT_TRUE(FloatEqual(vec4_6.w, Real(2.0)));
}

TEST(VectorTest, Vector4UnaryMinus) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Vec4 vec4_neg = -vec4_1;
    EXPECT_TRUE(FloatEqual(vec4_neg.x, -Real(1.0)));
    EXPECT_TRUE(FloatEqual(vec4_neg.y, -Real(2.0)));
    EXPECT_TRUE(FloatEqual(vec4_neg.z, -Real(3.0)));
    EXPECT_TRUE(FloatEqual(vec4_neg.w, -Real(4.0)));
}

TEST(VectorTest, Vector4DotProduct) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Vec4 vec4_2(Real(5.0), Real(6.0), Real(7.0), Real(8.0));
    
    Real dot = vec4_1.Dot(vec4_2);
    EXPECT_TRUE(FloatEqual(dot, Real(1.0) * Real(5.0) + Real(2.0) * Real(6.0) + Real(3.0) * Real(7.0) + Real(4.0) * Real(8.0)));
    
    Real dotProduct = Vec4::DotProduct(vec4_1, vec4_2);
    EXPECT_TRUE(FloatEqual(dotProduct, Real(1.0) * Real(5.0) + Real(2.0) * Real(6.0) + Real(3.0) * Real(7.0) + Real(4.0) * Real(8.0)));
}

TEST(VectorTest, Vector4Length) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    
    Real lengthSquared = vec4_1.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared, Real(1.0) * Real(1.0) + Real(2.0) * Real(2.0) + Real(3.0) * Real(3.0) + Real(4.0) * Real(4.0)));
    
    Real length = vec4_1.Length();
    EXPECT_TRUE(FloatEqual(length, std::sqrt(Real(30.0))));
}

TEST(VectorTest, Vector4Normalize) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Vec4 normalized = vec4_1.Normalize();
    EXPECT_TRUE(FloatEqual(normalized.Length(), Real(1.0)));
}

TEST(VectorTest, Vector4Lerp) {
    Vec4 vec4_1(Real(1.0), Real(2.0), Real(3.0), Real(4.0));
    Vec4 vec4_2(Real(5.0), Real(6.0), Real(7.0), Real(8.0));
    Vec4 lerp = Vec4::Lerp(vec4_1, vec4_2, Real(0.5));
    EXPECT_TRUE(FloatEqual(lerp.x, Real(3.0)));
    EXPECT_TRUE(FloatEqual(lerp.y, Real(4.0)));
    EXPECT_TRUE(FloatEqual(lerp.z, Real(5.0)));
    EXPECT_TRUE(FloatEqual(lerp.w, Real(6.0)));
}

} // namespace SympConvTest
