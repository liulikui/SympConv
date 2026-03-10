#ifndef SYMPCONV_CYLINDER_H
#define SYMPCONV_CYLINDER_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 圆柱体模板类
 * @details 表示三维空间中的圆柱体，由两个端点和半径定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TCylinder
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mStart; ///< 圆柱体起点
    TVector3<T> mEnd;   ///< 圆柱体终点
    T mRadius;          ///< 圆柱体半径

public:
    /**
     * @brief 默认构造函数
     */
    TCylinder() : 
        mStart(T(0), T(0), T(0)), 
        mEnd(T(0), T(0), T(1)),
        mRadius(T(0.5)) {}

    /**
     * @brief 带参数的构造函数
     * @param start 起点
     * @param end 终点
     * @param radius 半径
     */
    TCylinder(const TVector3<T>& start, const TVector3<T>& end, T radius) :
        mStart(start), 
        mEnd(end),
        mRadius(radius) {}

    /**
     * @brief 计算圆柱体的中心点
     * @return 中心点
     */
    TVector3<T> GetCenter() const
    {
        return (mStart + mEnd) * T(0.5);
    }

    /**
     * @brief 计算圆柱体的长度
     * @return 长度
     */
    T GetLength() const
    {
        return (mEnd - mStart).Length();
    }

    /**
     * @brief 计算圆柱体的方向向量
     * @return 方向向量
     */
    TVector3<T> GetDirection() const
    {
        return (mEnd - mStart).Normalize();
    }

    /**
     * @brief 获取圆柱体的AABB
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
     * @brief 检查点是否在圆柱体内
     * @param point 点
     * @return 是否在圆柱体内
     */
    bool Contains(const TVector3<T>& point) const
    {
        TVector3<T> ab = mEnd - mStart;
        T abDotAb = ab.Dot(ab);
        
        // 处理长度为0的圆柱体（退化为球体）
        if (abDotAb < std::numeric_limits<T>::epsilon()) {
            return (point - mStart).LengthSquared() <= mRadius * mRadius;
        }
        
        TVector3<T> ap = point - mStart;
        T t = ap.Dot(ab) / abDotAb;
        
        // 检查点是否在圆柱体的轴线方向范围内
        if (t < 0 || t > 1) {
            return false;
        }
        
        TVector3<T> closest = mStart + ab * t;
        return (point - closest).LengthSquared() <= mRadius * mRadius;
    }
};

// 类型别名
typedef TCylinder<float> Cylinderf;
typedef TCylinder<double> Cylinderd;
typedef TCylinder<fpnumber> Cylinder;

} // namespace SympConv

#endif // SYMPCONV_CYLINDER_H