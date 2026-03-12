#ifndef SYMPCONV_RAY_H
#define SYMPCONV_RAY_H

#include "Config.h"
#include "Vector.h"
#include <type_traits>
#include <cmath>
#include <limits>

namespace SympConv {

template<typename T> struct TAABB;
template<typename T> struct TTransform;

/**
 * @brief 射线模板类
 * @details 表示三维空间中的射线，由原点和方向向量定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TRay
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mOrigin;      ///< 射线原点
    TVector3<T> mDirection;   ///< 射线方向（应归一化）
    T tMin;                   ///< 射线起点
    T tMax;                   ///< 射线终点

public:
    /**
     * @brief 默认构造函数
     * @details 初始化为原点在原点，方向沿Z轴正方向的射线
     */
    TRay() : 
        mOrigin(T(0), T(0), T(0)), 
        mDirection(T(0), T(0), T(1)),
        tMin(T(0)), tMax(T(1)) {}

    /**
     * @brief 带参数的构造函数
     * @param origin 射线原点
     * @param direction 射线方向
     */
    TRay(const TVector3<T>& origin, const TVector3<T>& direction, T inMin, T inMax) :
        mOrigin(origin), 
        mDirection(direction),
        tMin(inMin), tMax(inMax){}

    /**
     * @brief 复制构造函数
     * @param other 另一个射线
     */
    TRay(const TRay<T>& other) = default;

    /**
     * @brief 移动构造函数
     * @param other 另一个射线
     */
    TRay(TRay<T>&& other) = default;

    /**
     * @brief 赋值运算符
     * @param other 另一个射线
     * @return 引用到当前射线
     */
    TRay<T>& operator=(const TRay<T>& other) = default;

    /**
     * @brief 移动赋值运算符
     * @param other 另一个射线
     * @return 引用到当前射线
     */
    TRay<T>& operator=(TRay<T>&& other) = default;

    /**
     * @brief 计算射线上的点
     * @param t 参数，t=0表示原点，t>0表示射线方向上的点
     * @return 射线上的点
     */
    TVector3<T> GetPoint(T t) const
    {
        return mOrigin + mDirection * t;
    }

    /**
     * @brief 归一化射线方向
     * @return 引用到当前射线
     */
    TRay<T>& Normalize()
    {
        mDirection = mDirection.Normalize();
        return *this;
    }

    /**
     * @brief 获取归一化后的射线
     * @return 归一化后的射线
     */
    TRay<T> Normalized() const
    {
        TRay<T> result = *this;
        result.Normalize();
        return result;
    }

    /**
     * @brief 计算射线到点的最短距离
     * @param point 目标点
     * @return 最短距离
     */
    T DistanceTo(const TVector3<T>& point) const
    {
        TVector3<T> v = point - mOrigin;
        T t = v.Dot(mDirection);
        // 限制t在tMin和tMax之间
        if (t < tMin) {
            t = tMin;
        } else if (t > tMax) {
            t = tMax;
        }
        TVector3<T> closest = GetPoint(t);
        return (point - closest).Length();
    }

    /**
     * @brief 计算射线到射线的最短距离
     * @param other 另外一条射线
     * @return 最短距离
     */
    T DistanceTo(const TRay<T>& other) const
    {
        TVector3<T> w = mOrigin - other.mOrigin;
        T a = mDirection.Dot(mDirection);
        T b = mDirection.Dot(other.mDirection);
        T c = other.mDirection.Dot(other.mDirection);
        T d = mDirection.Dot(w);
        T e = other.mDirection.Dot(w);

        T denominator = a * c - b * b;
        if (denominator < std::numeric_limits<T>::epsilon()) {
            // 射线平行，计算其中一条射线上的最近点到另一条射线的距离
            T t1 = -d / a;
            if (t1 < tMin) t1 = tMin;
            if (t1 > tMax) t1 = tMax;
            TVector3<T> p1 = GetPoint(t1);
            return other.DistanceTo(p1);
        }

        // 计算参数t和s
        T t = (b * e - c * d) / denominator;
        T s = (a * e - b * d) / denominator;

        // 限制t和s在各自的范围内
        if (t < tMin) t = tMin;
        if (t > tMax) t = tMax;
        if (s < other.tMin) s = other.tMin;
        if (s > other.tMax) s = other.tMax;

        // 计算两条射线上的点
        TVector3<T> p1 = GetPoint(t);
        TVector3<T> p2 = other.GetPoint(s);

        // 计算两点之间的距离
        return (p1 - p2).Length();
    }

    /**
     * @brief 变换射线
     * @param transform 变换
     * @return 变换后的射线
     */
    TRay<T> Transform(const TTransform<T>& transform) const
    {
        TRay<T> result;
        result.mOrigin = transform.TransformPoint(mOrigin);
        result.mDirection = transform.TransformDirection(mDirection);
        return result;
    }

    /**
     * @brief 反向变换射线
     * @param transform 变换
     * @return 反向变换后的射线
     */
    TRay<T> InverseTransform(const TTransform<T>& transform) const
    {
        TRay<T> result;
        result.mOrigin = transform.InverseTransformPoint(mOrigin);
        result.mDirection = transform.InverseTransformDirection(mDirection);
        return result;
    }
};

// 类型别名
typedef TRay<float> Rayf;
typedef TRay<double> Rayd;
typedef TRay<Real> Ray;

} // namespace SympConv

#endif // SYMPCONV_RAY_H
