#ifndef SYMPCONV_TEST_H
#define SYMPCONV_TEST_H

#include <iostream>
#include <cassert>
#include <cmath>
#include "SympConv/Vector.h"
#include "SympConv/Matrix.h"
#include "SympConv/AABB.h"
#include "SympConv/Ray.h"

namespace SympConvTest {

inline void PrintTestHeader(const char* testName) {
    std::cout << "========================================" << std::endl;
    std::cout << "Running: " << testName << std::endl;
    std::cout << "========================================" << std::endl;
}

inline void PrintTestResult(bool passed) {
    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl;
    std::cout << std::endl;
}

inline bool FloatEqual(float a, float b, float epsilon = 1e-4f) {
    return std::abs(a - b) < epsilon;
}

void TestTVector2() {
    PrintTestHeader("TVector2");

    using Vec2 = SympConv::TVector2<float>;
    bool passed = true;

    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);

    Vec2 add = v1 + v2;
    passed &= FloatEqual(add.x, 4.0f) && FloatEqual(add.y, 6.0f);

    Vec2 sub = v2 - v1;
    passed &= FloatEqual(sub.x, 2.0f) && FloatEqual(sub.y, 2.0f);

    Vec2 mul = v1 * 2.0f;
    passed &= FloatEqual(mul.x, 2.0f) && FloatEqual(mul.y, 4.0f);

    float dot = v1.Dot(v2);
    passed &= FloatEqual(dot, 11.0f);

    float len = v1.Length();
    passed &= FloatEqual(len, std::sqrt(5.0f));

    Vec2 norm = v1.Normalize();
    passed &= FloatEqual(norm.Length(), 1.0f);

    PrintTestResult(passed);
}

void TestTVector3() {
    PrintTestHeader("TVector3");

    using Vec3 = SympConv::TVector3<float>;
    bool passed = true;

    Vec3 v1(1.0f, 2.0f, 3.0f);
    Vec3 v2(4.0f, 5.0f, 6.0f);

    Vec3 add = v1 + v2;
    passed &= FloatEqual(add.x, 5.0f) && FloatEqual(add.y, 7.0f) && FloatEqual(add.z, 9.0f);

    Vec3 sub = v2 - v1;
    passed &= FloatEqual(sub.x, 3.0f) && FloatEqual(sub.y, 3.0f) && FloatEqual(sub.z, 3.0f);

    Vec3 mul = v1 * 2.0f;
    passed &= FloatEqual(mul.x, 2.0f) && FloatEqual(mul.y, 4.0f) && FloatEqual(mul.z, 6.0f);

    float dot = v1.Dot(v2);
    passed &= FloatEqual(dot, 32.0f);

    Vec3 cross = v1.Cross(v2);
    passed &= FloatEqual(cross.x, -3.0f) && FloatEqual(cross.y, 6.0f) && FloatEqual(cross.z, -3.0f);

    Vec3 crossProduct = Vec3::CrossProduct(v1, v2);
    passed &= FloatEqual(crossProduct.x, -3.0f) && FloatEqual(crossProduct.y, 6.0f) && FloatEqual(crossProduct.z, -3.0f);

    float len = v1.Length();
    passed &= FloatEqual(len, std::sqrt(14.0f));

    Vec3 norm = v1.Normalize();
    passed &= FloatEqual(norm.Length(), 1.0f);

    Vec3 lerp = Vec3::Lerp(v1, v2, 0.5f);
    passed &= FloatEqual(lerp.x, 2.5f) && FloatEqual(lerp.y, 3.5f) && FloatEqual(lerp.z, 4.5f);

    PrintTestResult(passed);
}

void TestTVector4() {
    PrintTestHeader("TVector4");

    using Vec4 = SympConv::TVector4<float>;
    bool passed = true;

    Vec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 v2(5.0f, 6.0f, 7.0f, 8.0f);

    Vec4 add = v1 + v2;
    passed &= FloatEqual(add.x, 6.0f) && FloatEqual(add.y, 8.0f) && FloatEqual(add.z, 10.0f) && FloatEqual(add.w, 12.0f);

    Vec4 sub = v2 - v1;
    passed &= FloatEqual(sub.x, 4.0f) && FloatEqual(sub.y, 4.0f) && FloatEqual(sub.z, 4.0f) && FloatEqual(sub.w, 4.0f);

    Vec4 mul = v1 * 2.0f;
    passed &= FloatEqual(mul.x, 2.0f) && FloatEqual(mul.y, 4.0f) && FloatEqual(mul.z, 6.0f) && FloatEqual(mul.w, 8.0f);

    float dot = v1.Dot(v2);
    passed &= FloatEqual(dot, 70.0f);

    float len = v1.Length();
    passed &= FloatEqual(len, std::sqrt(30.0f));

    Vec4 norm = v1.Normalize();
    passed &= FloatEqual(norm.Length(), 1.0f);

    PrintTestResult(passed);
}

void TestTMatrix2x2() {
    PrintTestHeader("TMatrix2x2");

    using Mat2 = SympConv::TMatrix2x2<float>;
    using Vec2 = SympConv::TVector2<float>;
    bool passed = true;

    Mat2 m1(1.0f, 2.0f, 3.0f, 4.0f);
    Mat2 m2(5.0f, 6.0f, 7.0f, 8.0f);

    Mat2 add = m1 + m2;
    passed &= FloatEqual(add.mRows[0].x, 6.0f) && FloatEqual(add.mRows[0].y, 8.0f);
    passed &= FloatEqual(add.mRows[1].x, 10.0f) && FloatEqual(add.mRows[1].y, 12.0f);

    Mat2 sub = m2 - m1;
    passed &= FloatEqual(sub.mRows[0].x, 4.0f) && FloatEqual(sub.mRows[0].y, 4.0f);
    passed &= FloatEqual(sub.mRows[1].x, 4.0f) && FloatEqual(sub.mRows[1].y, 4.0f);

    Mat2 mul = m1 * m2;
    passed &= FloatEqual(mul.mRows[0].x, 19.0f) && FloatEqual(mul.mRows[0].y, 22.0f);
    passed &= FloatEqual(mul.mRows[1].x, 43.0f) && FloatEqual(mul.mRows[1].y, 50.0f);

    Vec2 v(1.0f, 2.0f);
    Vec2 mv = m1 * v;
    passed &= FloatEqual(mv.x, 5.0f) && FloatEqual(mv.y, 11.0f);

    Mat2 trans = m1.Transpose();
    passed &= FloatEqual(trans.mRows[0].x, 1.0f) && FloatEqual(trans.mRows[0].y, 3.0f);
    passed &= FloatEqual(trans.mRows[1].x, 2.0f) && FloatEqual(trans.mRows[1].y, 4.0f);

    float det = m1.Determinant();
    passed &= FloatEqual(det, -2.0f);

    Mat2 identity = Mat2::Identity();
    passed &= FloatEqual(identity.mRows[0].x, 1.0f) && FloatEqual(identity.mRows[0].y, 0.0f);
    passed &= FloatEqual(identity.mRows[1].x, 0.0f) && FloatEqual(identity.mRows[1].y, 1.0f);

    Mat2 zero = Mat2::Zero();
    passed &= FloatEqual(zero.mRows[0].x, 0.0f) && FloatEqual(zero.mRows[0].y, 0.0f);
    passed &= FloatEqual(zero.mRows[1].x, 0.0f) && FloatEqual(zero.mRows[1].y, 0.0f);

    PrintTestResult(passed);
}

void TestTMatrix3x3() {
    PrintTestHeader("TMatrix3x3");

    using Mat3 = SympConv::TMatrix3x3<float>;
    using Vec3 = SympConv::TVector3<float>;
    bool passed = true;

    Mat3 m1(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
    Mat3 m2(9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);

    Mat3 add = m1 + m2;
    passed &= FloatEqual(add.mRows[0].x, 10.0f) && FloatEqual(add.mRows[0].y, 10.0f) && FloatEqual(add.mRows[0].z, 10.0f);

    Mat3 sub = m2 - m1;
    passed &= FloatEqual(sub.mRows[0].x, 8.0f) && FloatEqual(sub.mRows[0].y, 6.0f) && FloatEqual(sub.mRows[0].z, 4.0f);

    Vec3 v(1.0f, 2.0f, 3.0f);
    Vec3 mv = m1 * v;
    passed &= FloatEqual(mv.x, 14.0f) && FloatEqual(mv.y, 32.0f) && FloatEqual(mv.z, 50.0f);

    Mat3 trans = m1.Transpose();
    passed &= FloatEqual(trans.mRows[0].x, 1.0f) && FloatEqual(trans.mRows[0].y, 4.0f) && FloatEqual(trans.mRows[0].z, 7.0f);

    float det = m1.Determinant();
    passed &= FloatEqual(det, 0.0f);

    Mat3 identity = Mat3::Identity();
    passed &= FloatEqual(identity.mRows[0].x, 1.0f) && FloatEqual(identity.mRows[1].y, 1.0f) && FloatEqual(identity.mRows[2].z, 1.0f);

    Mat3 zero = Mat3::Zero();
    passed &= FloatEqual(zero.mRows[0].x, 0.0f) && FloatEqual(zero.mRows[1].y, 0.0f) && FloatEqual(zero.mRows[2].z, 0.0f);

    PrintTestResult(passed);
}

void TestTMatrix4x4() {
    PrintTestHeader("TMatrix4x4");

    using Mat4 = SympConv::TMatrix4x4<float>;
    using Vec4 = SympConv::TVector4<float>;
    bool passed = true;

    Mat4 m1 = Mat4::Identity();
    passed &= FloatEqual(m1.mRows[0].x, 1.0f) && FloatEqual(m1.mRows[1].y, 1.0f);
    passed &= FloatEqual(m1.mRows[2].z, 1.0f) && FloatEqual(m1.mRows[3].w, 1.0f);

    Mat4 m2(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);

    Vec4 v(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 mv = m2 * v;
    passed &= FloatEqual(mv.x, 30.0f) && FloatEqual(mv.y, 70.0f);
    passed &= FloatEqual(mv.z, 110.0f) && FloatEqual(mv.w, 150.0f);

    Mat4 trans = m2.Transpose();
    passed &= FloatEqual(trans.mRows[0].y, 5.0f);

    Mat4 zero = Mat4::Zero();
    passed &= FloatEqual(zero.mRows[0].x, 0.0f) && FloatEqual(zero.mRows[3].w, 0.0f);

    PrintTestResult(passed);
}

void TestTAABB() {
    PrintTestHeader("TAABB");

    using AABB = SympConv::TAABB<float>;
    using Vec3 = SympConv::TVector3<float>;
    bool passed = true;

    AABB aabb(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));

    Vec3 center = aabb.GetCenter();
    passed &= FloatEqual(center.x, 0.5f) && FloatEqual(center.y, 0.5f) && FloatEqual(center.z, 0.5f);

    Vec3 size = aabb.GetSize();
    passed &= FloatEqual(size.x, 1.0f) && FloatEqual(size.y, 1.0f) && FloatEqual(size.z, 1.0f);

    bool contains = aabb.Contains(Vec3(0.5f, 0.5f, 0.5f));
    passed &= contains;

    bool notContains = aabb.Contains(Vec3(2.0f, 2.0f, 2.0f));
    passed &= !notContains;

    AABB aabb2(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.5f, 1.5f, 1.5f));
    bool intersects = aabb.Intersects(aabb2);
    passed &= intersects;

    AABB emptyAABB;
    bool isEmpty = emptyAABB.IsEmpty();
    passed &= isEmpty;

    aabb.Expand(Vec3(2.0f, 2.0f, 2.0f));
    passed &= FloatEqual(aabb.mMax.x, 2.0f);

    PrintTestResult(passed);
}

void TestTRay() {
    PrintTestHeader("TRay");

    using Ray = SympConv::TRay<float>;
    using Vec3 = SympConv::TVector3<float>;
    bool passed = true;

    Ray ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));

    Vec3 point = ray.GetPoint(5.0f);
    passed &= FloatEqual(point.x, 5.0f) && FloatEqual(point.y, 0.0f) && FloatEqual(point.z, 0.0f);

    Ray normalizedRay = ray.Normalized();
    passed &= FloatEqual(normalizedRay.mDirection.Length(), 1.0f);

    PrintTestResult(passed);
}

void RunAllTests() {
    std::cout << "========================================" << std::endl;
    std::cout << "SympConv Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    TestTVector2();
    TestTVector3();
    TestTVector4();
    TestTMatrix2x2();
    TestTMatrix3x3();
    TestTMatrix4x4();
    TestTAABB();
    TestTRay();

    std::cout << "========================================" << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << "========================================" << std::endl;
}

}

#endif // SYMPCONV_TEST_H
