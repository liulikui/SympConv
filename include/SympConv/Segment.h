#ifndef SYMPCONV_SEGMENT_H
#define SYMPCONV_SEGMENT_H

#include "Config.h"
#include "Vector.h"
#include <type_traits>

namespace SympConv {

template<typename T> struct TAABB;
template<typename T> struct TTransform;

/**
 * @brief 线段模板类
 * @details 表示三维空间中的线段，由起点和终点定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TSegment
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mStart;      ///< 线段起点
    TVector3<T> mEnd;        ///< 线段终点

public:
    /**
     * @brief 默认构造函数
     * @details 初始化为起点和终点都在原点的线段
     */
    TSegment() : 
        mStart(T(0), T(0), T(0)), 
        mEnd(T(0), T(0), T(0)) {}

    /**
     * @brief 带参数的构造函数
     * @param start 线段起点
     * @param end 线段终点
     */
    TSegment(const TVector3<T>& start, const TVector3<T>& end) :
        mStart(start), 
        mEnd(end) {}

    /**
     * @brief 复制构造函数
     * @param other 另一个线段
     */
    TSegment(const TSegment<T>& other) = default;

    /**
     * @brief 移动构造函数
     * @param other 另一个线段
     */
    TSegment(TSegment<T>&& other) = default;

    /**
     * @brief 赋值运算符
     * @param other 另一个线段
     * @return 引用到当前线段
     */
    TSegment<T>& operator=(const TSegment<T>& other) = default;

    /**
     * @brief 移动赋值运算符
     * @param other 另一个线段
     * @return 引用到当前线段
     */
    TSegment<T>& operator=(TSegment<T>&& other) = default;

    /**
     * @brief 计算线段上的点
     * @param t 参数，t=0表示起点，t=1表示终点，0<t<1表示线段上的点
     * @return 线段上的点
     */
    TVector3<T> GetPoint(T t) const
    {
        return mStart + (mEnd - mStart) * t;
    }

    /**
     * @brief 计算线段的长度
     * @return 线段长度
     */
    T Length() const
    {
        return (mEnd - mStart).Length();
    }

    /**
     * @brief 计算线段长度的平方
     * @return 线段长度的平方
     */
    T LengthSquared() const
    {
        return (mEnd - mStart).LengthSquared();
    }

    /**
     * @brief 计算线段的中点
     * @return 线段中点
     */
    TVector3<T> GetCenter() const
    {
        return (mStart + mEnd) * T(0.5);
    }

    /**
     * @brief 计算线段的方向向量
     * @return 线段方向向量
     */
    TVector3<T> GetDirection() const
    {
        return (mEnd - mStart).Normalize();
    }

    /**
     * @brief 计算点到线段的最短距离
     * @param point 目标点
     * @return 最短距离
     */
    T DistanceTo(const TVector3<T>& point) const
    {
        TVector3<T> ab = mEnd - mStart;
        TVector3<T> ap = point - mStart;
        T t = ap.Dot(ab) / ab.Dot(ab);
        t = std::max(T(0), std::min(T(1), t));
        TVector3<T> closest = mStart + ab * t;
        return (point - closest).Length();
    }

    /**
     * @brief 变换线段
     * @param transform 变换
     * @return 变换后的线段
     */
    TSegment<T> Transform(const TTransform<T>& transform) const
    {
        TSegment<T> result;
        result.mStart = transform.TransformPoint(mStart);
        result.mEnd = transform.TransformPoint(mEnd);
        return result;
    }

    /**
     * @brief 反向变换线段
     * @param transform 变换
     * @return 反向变换后的线段
     */
    TSegment<T> InverseTransform(const TTransform<T>& transform) const
    {
        TSegment<T> result;
        result.mStart = transform.InverseTransformPoint(mStart);
        result.mEnd = transform.InverseTransformPoint(mEnd);
        return result;
    }
};

// 类型别名
typedef TSegment<float> Segmentf;
typedef TSegment<double> Segmentd;
typedef TSegment<Real> Segment;

} // namespace SympConv

#endif // SYMPCONV_SEGMENT_H