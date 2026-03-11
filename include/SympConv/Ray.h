#ifndef SYMPCONV_RAY_H
#define SYMPCONV_RAY_H

#include "Config.h"
#include "Vector.h"
#include <type_traits>

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

public:
    /**
     * @brief 默认构造函数
     * @details 初始化为原点在原点，方向沿Z轴正方向的射线
     */
    TRay() : 
        mOrigin(T(0), T(0), T(0)), 
        mDirection(T(0), T(0), T(1)) {}

    /**
     * @brief 带参数的构造函数
     * @param origin 射线原点
     * @param direction 射线方向
     */
    TRay(const TVector3<T>& origin, const TVector3<T>& direction) :
        mOrigin(origin), 
        mDirection(direction) {}

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
        mDirection.Normalize();
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
        TVector3<T> closest;
        if (t < 0) {
            closest = mOrigin;
        } else {
            closest = GetPoint(t);
        }
        return (point - closest).Length();
    }

    /**
     * @brief 计算射线到线段的最短距离
     * @param start 线段起点
     * @param end 线段终点
     * @return 最短距离
     */
    T DistanceToSegment(const TVector3<T>& start, const TVector3<T>& end) const
    {
        TVector3<T> dir = end - start;
        TVector3<T> q = mOrigin - start;
        T a = dir.Dot(dir);
        T b = dir.Dot(q);
        T c = dir.Dot(mDirection);
        T d = q.Dot(mDirection);
        T e = mDirection.Dot(mDirection);

        T det = a * e - c * c;
        T t, s;

        if (det < T(1e-6)) {
            s = 0;
            t = d / e;
        } else {
            s = (b * e - c * d) / det;
            t = (a * d - c * b) / det;
        }

        if (s < 0) {
            s = 0;
        } else if (s > 1) {
            s = 1;
        }

        if (t < 0) {
            TVector3<T> closestPoint = start + dir * s;
            return (mOrigin - closestPoint).Length();
        }

        TVector3<T> rayPoint = GetPoint(t);
        TVector3<T> segmentPoint = start + dir * s;
        return (rayPoint - segmentPoint).Length();
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
