#ifndef SYMPCONV_CAPSULE_H
#define SYMPCONV_CAPSULE_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 胶囊体模板类
 * @details 表示三维空间中的胶囊体，由两个端点和半径定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TCapsule
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mStart; ///< 胶囊体起点
    TVector3<T> mEnd;   ///< 胶囊体终点
    T mRadius;          ///< 胶囊体半径

public:
    /**
     * @brief 默认构造函数
     */
    TCapsule() : 
        mStart(T(0), T(0), T(0)), 
        mEnd(T(0), T(0), T(1)),
        mRadius(T(0.5)) {}

    /**
     * @brief 带参数的构造函数
     * @param start 起点
     * @param end 终点
     * @param radius 半径
     */
    TCapsule(const TVector3<T>& start, const TVector3<T>& end, T radius) :
        mStart(start), 
        mEnd(end),
        mRadius(radius) {}

    /**
     * @brief 计算胶囊体的中心点
     * @return 中心点
     */
    TVector3<T> GetCenter() const
    {
        return (mStart + mEnd) * T(0.5);
    }

    /**
     * @brief 计算胶囊体的长度
     * @return 长度
     */
    T GetLength() const
    {
        return (mEnd - mStart).Length();
    }

    /**
     * @brief 计算胶囊体的方向向量
     * @return 方向向量
     */
    TVector3<T> GetDirection() const
    {
        return (mEnd - mStart).Normalize();
    }

    /**
     * @brief 获取胶囊体的AABB
     * @return AABB包围盒
     */
    TAABB<T> GetAABB() const
    {
        TVector3<T> offset(mRadius, mRadius, mRadius);
        TVector3<T> min(
            std::min(mStart.x, mEnd.x),
            std::min(mStart.y, mEnd.y),
            std::min(mStart.z, mEnd.z)
        );
        TVector3<T> max(
            std::max(mStart.x, mEnd.x),
            std::max(mStart.y, mEnd.y),
            std::max(mStart.z, mEnd.z)
        );
        min = min - offset;
        max = max + offset;
        return TAABB<T>(min, max);
    }

    /**
     * @brief 检查点是否在胶囊体内
     * @param point 点
     * @return 是否在胶囊体内
     */
    bool Contains(const TVector3<T>& point) const
    {
        TVector3<T> ab = mEnd - mStart;
        TVector3<T> ap = point - mStart;
        T t = ap.Dot(ab) / ab.Dot(ab);
        t = std::max(T(0), std::min(T(1), t));
        TVector3<T> closest = mStart + ab * t;
        return (point - closest).LengthSquared() <= mRadius * mRadius;
    }
};

// 类型别名
typedef TCapsule<float> Capsulef;
typedef TCapsule<double> Capsuled;
typedef TCapsule<fpnumber> Capsule;

} // namespace SympConv

#endif // SYMPCONV_CAPSULE_H
