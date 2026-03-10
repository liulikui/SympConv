#include <gtest/gtest.h>
#include "SympConv/Vector.h"
#include "TestUtils.h"

namespace SympConvTest {

using Vec2 = SympConv::Vector2;
using Vec3 = SympConv::Vector3;
using Vec4 = SympConv::Vector4;

TEST(VectorTest, Vector2Constructor) {
    Vec2 vec2;
    Vec2 vec2_1(1.0f, 2.0f);
    EXPECT_TRUE(FloatEqual(vec2_1.x, 1.0f));
    EXPECT_TRUE(FloatEqual(vec2_1.y, 2.0f));
}

TEST(VectorTest, Vector2Operators) {
    Vec2 vec2_1(1.0f, 2.0f);
    Vec2 vec2_2(3.0f, 4.0f);
    
    // 测试加法
    Vec2 vec2_add = vec2_1 + vec2_2;
    EXPECT_TRUE(FloatEqual(vec2_add.x, 4.0f));
    EXPECT_TRUE(FloatEqual(vec2_add.y, 6.0f));
    
    // 测试减法
    Vec2 vec2_sub = vec2_2 - vec2_1;
    EXPECT_TRUE(FloatEqual(vec2_sub.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec2_sub.y, 2.0f));
    
    // 测试标量乘法
    Vec2 vec2_scalar = vec2_1 * 2.0f;
    EXPECT_TRUE(FloatEqual(vec2_scalar.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec2_scalar.y, 4.0f));
    
    // 测试标量除法
    Vec2 vec2_div = vec2_1 / 2.0f;
    EXPECT_TRUE(FloatEqual(vec2_div.x, 0.5f));
    EXPECT_TRUE(FloatEqual(vec2_div.y, 1.0f));
}

TEST(VectorTest, Vector2CompoundOperators) {
    Vec2 vec2_1(1.0f, 2.0f);
    Vec2 vec2_2(3.0f, 4.0f);
    
    // 测试复合加法
    Vec2 vec2_3 = vec2_1;
    vec2_3 += vec2_2;
    EXPECT_TRUE(FloatEqual(vec2_3.x, 4.0f));
    EXPECT_TRUE(FloatEqual(vec2_3.y, 6.0f));
    
    // 测试复合减法
    Vec2 vec2_4 = vec2_2;
    vec2_4 -= vec2_1;
    EXPECT_TRUE(FloatEqual(vec2_4.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec2_4.y, 2.0f));
    
    // 测试复合乘法
    Vec2 vec2_5 = vec2_1;
    vec2_5 *= 2.0f;
    EXPECT_TRUE(FloatEqual(vec2_5.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec2_5.y, 4.0f));
    
    // 测试复合除法
    Vec2 vec2_6 = vec2_1;
    vec2_6 /= 2.0f;
    EXPECT_TRUE(FloatEqual(vec2_6.x, 0.5f));
    EXPECT_TRUE(FloatEqual(vec2_6.y, 1.0f));
}

TEST(VectorTest, Vector2UnaryMinus) {
    Vec2 vec2_1(1.0f, 2.0f);
    Vec2 vec2_neg = -vec2_1;
    EXPECT_TRUE(FloatEqual(vec2_neg.x, -1.0f));
    EXPECT_TRUE(FloatEqual(vec2_neg.y, -2.0f));
}

TEST(VectorTest, Vector2DotProduct) {
    Vec2 vec2_1(1.0f, 2.0f);
    Vec2 vec2_2(3.0f, 4.0f);
    
    float dot = vec2_1.Dot(vec2_2);
    EXPECT_TRUE(FloatEqual(dot, 1.0f * 3.0f + 2.0f * 4.0f));
    
    float dotProduct = Vec2::DotProduct(vec2_1, vec2_2);
    EXPECT_TRUE(FloatEqual(dotProduct, 1.0f * 3.0f + 2.0f * 4.0f));
}

TEST(VectorTest, Vector2Length) {
    Vec2 vec2_1(1.0f, 2.0f);
    
    float lengthSquared = vec2_1.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared, 1.0f * 1.0f + 2.0f * 2.0f));
    
    float length = vec2_1.Length();
    EXPECT_TRUE(FloatEqual(length, std::sqrt(5.0f)));
}

TEST(VectorTest, Vector2Normalize) {
    Vec2 vec2_1(1.0f, 2.0f);
    Vec2 normalized = vec2_1.Normalize();
    EXPECT_TRUE(FloatEqual(normalized.Length(), 1.0f));
}

TEST(VectorTest, Vector2Lerp) {
    Vec2 vec2_1(1.0f, 2.0f);
    Vec2 vec2_2(3.0f, 4.0f);
    Vec2 lerp = Vec2::Lerp(vec2_1, vec2_2, 0.5f);
    EXPECT_TRUE(FloatEqual(lerp.x, 2.0f));
    EXPECT_TRUE(FloatEqual(lerp.y, 3.0f));
}

TEST(VectorTest, Vector3Constructor) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    EXPECT_TRUE(FloatEqual(vec3_1.x, 1.0f));
    EXPECT_TRUE(FloatEqual(vec3_1.y, 2.0f));
    EXPECT_TRUE(FloatEqual(vec3_1.z, 3.0f));
}

TEST(VectorTest, Vector3Operators) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    Vec3 vec3_2(4.0f, 5.0f, 6.0f);
    
    // 测试加法
    Vec3 vec3_add = vec3_1 + vec3_2;
    EXPECT_TRUE(FloatEqual(vec3_add.x, 5.0f));
    EXPECT_TRUE(FloatEqual(vec3_add.y, 7.0f));
    EXPECT_TRUE(FloatEqual(vec3_add.z, 9.0f));
    
    // 测试减法
    Vec3 vec3_sub = vec3_2 - vec3_1;
    EXPECT_TRUE(FloatEqual(vec3_sub.x, 3.0f));
    EXPECT_TRUE(FloatEqual(vec3_sub.y, 3.0f));
    EXPECT_TRUE(FloatEqual(vec3_sub.z, 3.0f));
    
    // 测试标量乘法
    Vec3 vec3_scalar = vec3_1 * 2.0f;
    EXPECT_TRUE(FloatEqual(vec3_scalar.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec3_scalar.y, 4.0f));
    EXPECT_TRUE(FloatEqual(vec3_scalar.z, 6.0f));
    
    // 测试分量乘法
    Vec3 vec3_component = vec3_1 * vec3_2;
    EXPECT_TRUE(FloatEqual(vec3_component.x, 4.0f));
    EXPECT_TRUE(FloatEqual(vec3_component.y, 10.0f));
    EXPECT_TRUE(FloatEqual(vec3_component.z, 18.0f));
    
    // 测试标量除法
    Vec3 vec3_div = vec3_1 / 2.0f;
    EXPECT_TRUE(FloatEqual(vec3_div.x, 0.5f));
    EXPECT_TRUE(FloatEqual(vec3_div.y, 1.0f));
    EXPECT_TRUE(FloatEqual(vec3_div.z, 1.5f));
}

TEST(VectorTest, Vector3CompoundOperators) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    Vec3 vec3_2(4.0f, 5.0f, 6.0f);
    
    // 测试复合加法
    Vec3 vec3_3 = vec3_1;
    vec3_3 += vec3_2;
    EXPECT_TRUE(FloatEqual(vec3_3.x, 5.0f));
    EXPECT_TRUE(FloatEqual(vec3_3.y, 7.0f));
    EXPECT_TRUE(FloatEqual(vec3_3.z, 9.0f));
    
    // 测试复合减法
    Vec3 vec3_4 = vec3_2;
    vec3_4 -= vec3_1;
    EXPECT_TRUE(FloatEqual(vec3_4.x, 3.0f));
    EXPECT_TRUE(FloatEqual(vec3_4.y, 3.0f));
    EXPECT_TRUE(FloatEqual(vec3_4.z, 3.0f));
    
    // 测试复合乘法
    Vec3 vec3_5 = vec3_1;
    vec3_5 *= 2.0f;
    EXPECT_TRUE(FloatEqual(vec3_5.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec3_5.y, 4.0f));
    EXPECT_TRUE(FloatEqual(vec3_5.z, 6.0f));
    
    // 测试复合除法
    Vec3 vec3_6 = vec3_1;
    vec3_6 /= 2.0f;
    EXPECT_TRUE(FloatEqual(vec3_6.x, 0.5f));
    EXPECT_TRUE(FloatEqual(vec3_6.y, 1.0f));
    EXPECT_TRUE(FloatEqual(vec3_6.z, 1.5f));
}

TEST(VectorTest, Vector3UnaryMinus) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    Vec3 vec3_neg = -vec3_1;
    EXPECT_TRUE(FloatEqual(vec3_neg.x, -1.0f));
    EXPECT_TRUE(FloatEqual(vec3_neg.y, -2.0f));
    EXPECT_TRUE(FloatEqual(vec3_neg.z, -3.0f));
}

TEST(VectorTest, Vector3DotProduct) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    Vec3 vec3_2(4.0f, 5.0f, 6.0f);
    
    float dot3 = vec3_1.Dot(vec3_2);
    EXPECT_TRUE(FloatEqual(dot3, 1.0f * 4.0f + 2.0f * 5.0f + 3.0f * 6.0f));
    
    float dotProduct3 = Vec3::DotProduct(vec3_1, vec3_2);
    EXPECT_TRUE(FloatEqual(dotProduct3, 1.0f * 4.0f + 2.0f * 5.0f + 3.0f * 6.0f));
}

TEST(VectorTest, Vector3CrossProduct) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    Vec3 vec3_2(4.0f, 5.0f, 6.0f);
    
    Vec3 cross = vec3_1.Cross(vec3_2);
    EXPECT_TRUE(FloatEqual(cross.x, 2.0f * 6.0f - 3.0f * 5.0f));
    EXPECT_TRUE(FloatEqual(cross.y, 3.0f * 4.0f - 1.0f * 6.0f));
    EXPECT_TRUE(FloatEqual(cross.z, 1.0f * 5.0f - 2.0f * 4.0f));
    
    Vec3 crossProduct = Vec3::CrossProduct(vec3_1, vec3_2);
    EXPECT_TRUE(FloatEqual(crossProduct.x, 2.0f * 6.0f - 3.0f * 5.0f));
    EXPECT_TRUE(FloatEqual(crossProduct.y, 3.0f * 4.0f - 1.0f * 6.0f));
    EXPECT_TRUE(FloatEqual(crossProduct.z, 1.0f * 5.0f - 2.0f * 4.0f));
}

TEST(VectorTest, Vector3Length) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    
    float lengthSquared3 = vec3_1.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared3, 1.0f * 1.0f + 2.0f * 2.0f + 3.0f * 3.0f));
    
    float length3 = vec3_1.Length();
    EXPECT_TRUE(FloatEqual(length3, std::sqrt(14.0f)));
}

TEST(VectorTest, Vector3Normalize) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    Vec3 normalized3 = vec3_1.Normalize();
    EXPECT_TRUE(FloatEqual(normalized3.Length(), 1.0f));
}

TEST(VectorTest, Vector3Lerp) {
    Vec3 vec3_1(1.0f, 2.0f, 3.0f);
    Vec3 vec3_2(4.0f, 5.0f, 6.0f);
    Vec3 lerp3 = Vec3::Lerp(vec3_1, vec3_2, 0.5f);
    EXPECT_TRUE(FloatEqual(lerp3.x, 2.5f));
    EXPECT_TRUE(FloatEqual(lerp3.y, 3.5f));
    EXPECT_TRUE(FloatEqual(lerp3.z, 4.5f));
}

TEST(VectorTest, Vector4Constructor) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_TRUE(FloatEqual(vec4_1.x, 1.0f));
    EXPECT_TRUE(FloatEqual(vec4_1.y, 2.0f));
    EXPECT_TRUE(FloatEqual(vec4_1.z, 3.0f));
    EXPECT_TRUE(FloatEqual(vec4_1.w, 4.0f));
}

TEST(VectorTest, Vector4Operators) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 vec4_2(5.0f, 6.0f, 7.0f, 8.0f);
    
    // 测试加法
    Vec4 vec4_add = vec4_1 + vec4_2;
    EXPECT_TRUE(FloatEqual(vec4_add.x, 6.0f));
    EXPECT_TRUE(FloatEqual(vec4_add.y, 8.0f));
    EXPECT_TRUE(FloatEqual(vec4_add.z, 10.0f));
    EXPECT_TRUE(FloatEqual(vec4_add.w, 12.0f));
    
    // 测试减法
    Vec4 vec4_sub = vec4_2 - vec4_1;
    EXPECT_TRUE(FloatEqual(vec4_sub.x, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_sub.y, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_sub.z, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_sub.w, 4.0f));
    
    // 测试标量乘法
    Vec4 vec4_scalar = vec4_1 * 2.0f;
    EXPECT_TRUE(FloatEqual(vec4_scalar.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec4_scalar.y, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_scalar.z, 6.0f));
    EXPECT_TRUE(FloatEqual(vec4_scalar.w, 8.0f));
    
    // 测试标量除法
    Vec4 vec4_div = vec4_1 / 2.0f;
    EXPECT_TRUE(FloatEqual(vec4_div.x, 0.5f));
    EXPECT_TRUE(FloatEqual(vec4_div.y, 1.0f));
    EXPECT_TRUE(FloatEqual(vec4_div.z, 1.5f));
    EXPECT_TRUE(FloatEqual(vec4_div.w, 2.0f));
}

TEST(VectorTest, Vector4CompoundOperators) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 vec4_2(5.0f, 6.0f, 7.0f, 8.0f);
    
    // 测试复合加法
    Vec4 vec4_3 = vec4_1;
    vec4_3 += vec4_2;
    EXPECT_TRUE(FloatEqual(vec4_3.x, 6.0f));
    EXPECT_TRUE(FloatEqual(vec4_3.y, 8.0f));
    EXPECT_TRUE(FloatEqual(vec4_3.z, 10.0f));
    EXPECT_TRUE(FloatEqual(vec4_3.w, 12.0f));
    
    // 测试复合减法
    Vec4 vec4_4 = vec4_2;
    vec4_4 -= vec4_1;
    EXPECT_TRUE(FloatEqual(vec4_4.x, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_4.y, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_4.z, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_4.w, 4.0f));
    
    // 测试复合乘法
    Vec4 vec4_5 = vec4_1;
    vec4_5 *= 2.0f;
    EXPECT_TRUE(FloatEqual(vec4_5.x, 2.0f));
    EXPECT_TRUE(FloatEqual(vec4_5.y, 4.0f));
    EXPECT_TRUE(FloatEqual(vec4_5.z, 6.0f));
    EXPECT_TRUE(FloatEqual(vec4_5.w, 8.0f));
    
    // 测试复合除法
    Vec4 vec4_6 = vec4_1;
    vec4_6 /= 2.0f;
    EXPECT_TRUE(FloatEqual(vec4_6.x, 0.5f));
    EXPECT_TRUE(FloatEqual(vec4_6.y, 1.0f));
    EXPECT_TRUE(FloatEqual(vec4_6.z, 1.5f));
    EXPECT_TRUE(FloatEqual(vec4_6.w, 2.0f));
}

TEST(VectorTest, Vector4UnaryMinus) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 vec4_neg = -vec4_1;
    EXPECT_TRUE(FloatEqual(vec4_neg.x, -1.0f));
    EXPECT_TRUE(FloatEqual(vec4_neg.y, -2.0f));
    EXPECT_TRUE(FloatEqual(vec4_neg.z, -3.0f));
    EXPECT_TRUE(FloatEqual(vec4_neg.w, -4.0f));
}

TEST(VectorTest, Vector4DotProduct) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 vec4_2(5.0f, 6.0f, 7.0f, 8.0f);
    
    float dot = vec4_1.Dot(vec4_2);
    EXPECT_TRUE(FloatEqual(dot, 1.0f * 5.0f + 2.0f * 6.0f + 3.0f * 7.0f + 4.0f * 8.0f));
    
    float dotProduct = Vec4::DotProduct(vec4_1, vec4_2);
    EXPECT_TRUE(FloatEqual(dotProduct, 1.0f * 5.0f + 2.0f * 6.0f + 3.0f * 7.0f + 4.0f * 8.0f));
}

TEST(VectorTest, Vector4Length) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    
    float lengthSquared = vec4_1.LengthSquared();
    EXPECT_TRUE(FloatEqual(lengthSquared, 1.0f * 1.0f + 2.0f * 2.0f + 3.0f * 3.0f + 4.0f * 4.0f));
    
    float length = vec4_1.Length();
    EXPECT_TRUE(FloatEqual(length, std::sqrt(30.0f)));
}

TEST(VectorTest, Vector4Normalize) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 normalized = vec4_1.Normalize();
    EXPECT_TRUE(FloatEqual(normalized.Length(), 1.0f));
}

TEST(VectorTest, Vector4Lerp) {
    Vec4 vec4_1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 vec4_2(5.0f, 6.0f, 7.0f, 8.0f);
    Vec4 lerp = Vec4::Lerp(vec4_1, vec4_2, 0.5f);
    EXPECT_TRUE(FloatEqual(lerp.x, 3.0f));
    EXPECT_TRUE(FloatEqual(lerp.y, 4.0f));
    EXPECT_TRUE(FloatEqual(lerp.z, 5.0f));
    EXPECT_TRUE(FloatEqual(lerp.w, 6.0f));
}

} // namespace SympConvTest
