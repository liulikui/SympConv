#ifndef SYMPCONV_VECTOR_H
#define SYMPCONV_VECTOR_H

#include "Config.h"
#include <type_traits>
#include <cmath>

namespace SympConv {

/**
 * @brief 二维向量模板类
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TVector2
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T x; ///< x分量
    T y; ///< y分量

public:
    /**
     * @brief 默认构造函数
     */
    TVector2() = default;

    /**
     * @brief 带参数的构造函数
     * @param InX x分量值
     * @param InY y分量值
     */
    TVector2(T InX, T InY)
        : x(InX)
        , y(InY)
    {

    }

    /**
     * @brief 向量加法运算符
     * @param V 另一个向量
     * @return 两个向量的和
     */
    TVector2<T> operator+(const TVector2<T>& V) const
    {
        return TVector2<T>(x + V.x, y + V.y);
    }

    /**
     * @brief 向量减法运算符
     * @param V 另一个向量
     * @return 两个向量的差
     */
    TVector2<T> operator-(const TVector2<T>& V) const
    {
        return TVector2<T>(x - V.x, y - V.y);
    }

    /**
     * @brief 一元负号运算符
     * @return 取反后的向量
     */
    TVector2<T> operator-() const
    {
        return TVector2<T>(-x, -y);
    }

    /**
     * @brief 向量点积
     * @param V 另一个向量
     * @return 点积结果
     */
    T Dot(const TVector2<T>& V) const
    {
        return x * V.x + y * V.y;
    }

    /**
     * @brief 静态点积计算
     * @param A 第一个向量
     * @param B 第二个向量
     * @return 点积结果
     */
    static T DotProduct(const TVector2<T>& A, const TVector2<T>& B)
    {
        return A.x * B.x + A.y * B.y;
    }

    /**
     * @brief 标量乘法运算符
     * @param scalar 标量值
     * @return 向量与标量的乘积
     */
    TVector2<T> operator*(T scalar) const
    {
        return TVector2<T>(x * scalar, y * scalar);
    }

    /**
     * @brief 标量除法运算符
     * @param scalar 标量值
     * @return 向量与标量的商
     */
    TVector2<T> operator/(T scalar) const
    {
        return TVector2<T>(x / scalar, y / scalar);
    }

    /**
     * @brief 向量加法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector2<T>& operator+=(const TVector2<T>& V)
    {
        x += V.x;
        y += V.y;
        return *this;
    }

    /**
     * @brief 向量减法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector2<T>& operator-=(const TVector2<T>& V)
    {
        x -= V.x;
        y -= V.y;
        return *this;
    }

    /**
     * @brief 标量乘法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前向量
     */
    TVector2<T>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    /**
     * @brief 标量除法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前向量
     */
    TVector2<T>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    /**
     * @brief 计算向量长度
     * @return 向量的长度
     */
    T Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    /**
     * @brief 计算向量长度的平方
     * @return 向量长度的平方
     */
    T LengthSquared() const
    {
        return x * x + y * y;
    }

    /**
     * @brief 归一化向量
     * @return 归一化后的单位向量
     */
    TVector2<T> Normalize() const
    {
        T len = Length();
        if (len > T(0))
        {
            return *this / len;
        }
        return TVector2<T>(T(0), T(0));
    }

    /**
     * @brief 线性插值
     * @param A 起始向量
     * @param B 结束向量
     * @param t 插值参数，范围[0,1]
     * @return 插值结果
     */
    static TVector2<T> Lerp(const TVector2<T>& A, const TVector2<T>& B, T t)
    {
        return TVector2<T>(
            A.x + (B.x - A.x) * t,
            A.y + (B.y - A.y) * t
        );
    }
};

/**
 * @brief 三维向量模板类
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TVector3
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T x; ///< x分量
    T y; ///< y分量
    T z; ///< z分量

public:
    /**
     * @brief 默认构造函数
     */
    TVector3() = default;

    /**
     * @brief 带参数的构造函数
     * @param InX x分量值
     * @param InY y分量值
     * @param InZ z分量值
     */
    TVector3(T InX, T InY, T InZ)
        :x(InX)
        ,y(InY)
        ,z(InZ)
    {

    }

    /**
     * @brief 向量加法运算符
     * @param V 另一个向量
     * @return 两个向量的和
     */
    TVector3<T> operator+(const TVector3<T>& V) const
    {
        return TVector3<T>(x + V.x, y + V.y, z + V.z);
    }

    /**
     * @brief 向量减法运算符
     * @param V 另一个向量
     * @return 两个向量的差
     */
    TVector3<T> operator-(const TVector3<T>& V) const
    {
        return TVector3<T>(x - V.x, y - V.y, z - V.z);
    }

    /**
     * @brief 一元负号运算符
     * @return 取反后的向量
     */
    TVector3<T> operator-() const
    {
        return TVector3<T>(-x, -y, -z);
    }

    /**
     * @brief 向量点积
     * @param V 另一个向量
     * @return 点积结果
     */
    T Dot(const TVector3<T>& V) const
    {
        return x * V.x + y * V.y + z * V.z;
    }

    /**
     * @brief 静态点积计算
     * @param A 第一个向量
     * @param B 第二个向量
     * @return 点积结果
     */
    static T DotProduct(const TVector3<T>& A, const TVector3<T>& B)
    {
        return A.x * B.x + A.y * B.y + A.z * B.z;
    }

    /**
     * @brief 向量叉积
     * @param V 另一个向量
     * @return 叉积结果
     */
    TVector3<T> Cross(const TVector3<T>& V) const
    {
        return TVector3<T>(
            y * V.z - z * V.y,
            z * V.x - x * V.z,
            x * V.y - y * V.x
        );
    }

    /**
     * @brief 静态叉积计算
     * @param A 第一个向量
     * @param B 第二个向量
     * @return 叉积结果
     */
    static TVector3<T> CrossProduct(const TVector3<T>& A, const TVector3<T>& B)
    {
        return TVector3<T>(
            A.y * B.z - A.z * B.y,
            A.z * B.x - A.x * B.z,
            A.x * B.y - A.y * B.x
        );
    }

    /**
     * @brief 标量乘法运算符
     * @param scalar 标量值
     * @return 向量与标量的乘积
     */
    TVector3<T> operator*(T scalar) const
    {
        return TVector3<T>(x * scalar, y * scalar, z * scalar);
    }

    /**
     * @brief 向量分量乘法运算符
     * @param V 另一个向量
     * @return 向量的分量乘积
     */
    TVector3<T> operator*(const TVector3<T>& V) const
    {
        return TVector3<T>(x * V.x, y * V.y, z * V.z);
    }

    /**
     * @brief 标量除法运算符
     * @param scalar 标量值
     * @return 向量与标量的商
     */
    TVector3<T> operator/(T scalar) const
    {
        return TVector3<T>(x / scalar, y / scalar, z / scalar);
    }

    /**
     * @brief 向量分量除法运算符
     * @param V 另一个向量
     * @return 向量的分量除法结果
     */
    TVector3<T> operator/(const TVector3<T>& V) const
    {
        return TVector3<T>(x / V.x, y / V.y, z / V.z);
    }

    /**
     * @brief 向量加法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector3<T>& operator+=(const TVector3<T>& V)
    {
        x += V.x;
        y += V.y;
        z += V.z;
        return *this;
    }

    /**
     * @brief 向量减法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector3<T>& operator-=(const TVector3<T>& V)
    {
        x -= V.x;
        y -= V.y;
        z -= V.z;
        return *this;
    }

    /**
     * @brief 向量分量乘法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector3<T>& operator*=(const TVector3<T>& V)
    {
        x *= V.x;
        y *= V.y;
        z *= V.z;
        return *this;
    }

    /**
     * @brief 标量乘法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前向量
     */
    TVector3<T>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    /**
     * @brief 标量除法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前向量
     */
    TVector3<T>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    /**
     * @brief 向量分量除法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector3<T>& operator/=(const TVector3<T>& V)
    {
        x /= V.x;
        y /= V.y;
        z /= V.z;
        return *this;
    }

    /**
     * @brief 计算向量长度
     * @return 向量的长度
     */
    T Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    /**
     * @brief 计算向量长度的平方
     * @return 向量长度的平方
     */
    T LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    /**
     * @brief 归一化向量
     * @return 归一化后的单位向量
     */
    TVector3<T> Normalize() const
    {
        T len = Length();
        if (len > T(0))
        {
            return *this / len;
        }
        return TVector3<T>(T(0), T(0), T(0));
    }

    /**
     * @brief 线性插值
     * @param A 起始向量
     * @param B 结束向量
     * @param t 插值参数，范围[0,1]
     * @return 插值结果
     */
    static TVector3<T> Lerp(const TVector3<T>& A, const TVector3<T>& B, T t)
    {
        return TVector3<T>(
            A.x + (B.x - A.x) * t,
            A.y + (B.y - A.y) * t,
            A.z + (B.z - A.z) * t
        );
    }
};

/**
 * @brief 四维向量模板类
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TVector4
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T x; ///< x分量
    T y; ///< y分量
    T z; ///< z分量
    T w; ///< w分量

public:
    /**
     * @brief 默认构造函数
     */
    TVector4() = default;

    /**
     * @brief 带参数的构造函数
     * @param InX x分量值
     * @param InY y分量值
     * @param InZ z分量值
     * @param InW w分量值
     */
    TVector4(T InX, T InY, T InZ, T InW)
        : x(InX)
        , y(InY)
        , z(InZ)
        , w(InW)
    {

    }

    /**
     * @brief 向量加法运算符
     * @param V 另一个向量
     * @return 两个向量的和
     */
    TVector4<T> operator+(const TVector4<T>& V) const
    {
        return TVector4<T>(x + V.x, y + V.y, z + V.z, w + V.w);
    }

    /**
     * @brief 向量减法运算符
     * @param V 另一个向量
     * @return 两个向量的差
     */
    TVector4<T> operator-(const TVector4<T>& V) const
    {
        return TVector4<T>(x - V.x, y - V.y, z - V.z, w - V.w);
    }

    /**
     * @brief 一元负号运算符
     * @return 取反后的向量
     */
    TVector4<T> operator-() const
    {
        return TVector4<T>(-x, -y, -z, -w);
    }

    /**
     * @brief 标量乘法运算符
     * @param scalar 标量值
     * @return 向量与标量的乘积
     */
    TVector4<T> operator*(T scalar) const
    {
        return TVector4<T>(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    /**
     * @brief 标量除法运算符
     * @param scalar 标量值
     * @return 向量与标量的商
     */
    TVector4<T> operator/(T scalar) const
    {
        return TVector4<T>(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    /**
     * @brief 向量加法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector4<T>& operator+=(const TVector4<T>& V)
    {
        x += V.x;
        y += V.y;
        z += V.z;
        w += V.w;
        return *this;
    }

    /**
     * @brief 向量减法复合赋值运算符
     * @param V 另一个向量
     * @return 引用到当前向量
     */
    TVector4<T>& operator-=(const TVector4<T>& V)
    {
        x -= V.x;
        y -= V.y;
        z -= V.z;
        w -= V.w;
        return *this;
    }

    /**
     * @brief 标量乘法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前向量
     */
    TVector4<T>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    /**
     * @brief 标量除法复合赋值运算符
     * @param scalar 标量值
     * @return 引用到当前向量
     */
    TVector4<T>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    /**
     * @brief 向量点积
     * @param V 另一个向量
     * @return 点积结果
     */
    T Dot(const TVector4<T>& V) const
    {
        return x * V.x + y * V.y + z * V.z + w * V.w;
    }

    /**
     * @brief 静态点积计算
     * @param A 第一个向量
     * @param B 第二个向量
     * @return 点积结果
     */
    static T DotProduct(const TVector4<T>& A, const TVector4<T>& B)
    {
        return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
    }

    /**
     * @brief 计算向量长度
     * @return 向量的长度
     */
    T Length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    /**
     * @brief 计算向量长度的平方
     * @return 向量长度的平方
     */
    T LengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    /**
     * @brief 归一化向量
     * @return 归一化后的单位向量
     */
    TVector4<T> Normalize() const
    {
        T len = Length();
        if (len > T(0))
        {
            return *this / len;
        }
        return TVector4<T>(T(0), T(0), T(0), T(0));
    }

    /**
     * @brief 线性插值
     * @param A 起始向量
     * @param B 结束向量
     * @param t 插值参数，范围[0,1]
     * @return 插值结果
     */
    static TVector4<T> Lerp(const TVector4<T>& A, const TVector4<T>& B, T t)
    {
        return TVector4<T>(
            A.x + (B.x - A.x) * t,
            A.y + (B.y - A.y) * t,
            A.z + (B.z - A.z) * t,
            A.w + (B.w - A.w) * t
        );
    }
};

// 类型别名
typedef TVector2<float> Vector2f;
typedef TVector2<double> Vector2d;
typedef TVector2<fpnumber> Vector2;

typedef TVector3<float> Vector3f;
typedef TVector3<double> Vector3d;
typedef TVector3<fpnumber> Vector3;

typedef TVector4<float> Vector4f;
typedef TVector4<double> Vector4d;
typedef TVector4<fpnumber> Vector4;

} // namespace SympConv

#endif // SYMPCONV_VECTOR_H