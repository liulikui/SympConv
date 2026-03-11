#ifndef SYMPCONV_SPHERE_H
#define SYMPCONV_SPHERE_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 球体模板类
 * @details 表示三维空间中的球体，由中心和半径定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TSphere
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mCenter; ///< 球体中心
    T mRadius;           ///< 球体半径

public:
    /**
     * @brief 默认构造函数
     */
    TSphere() : 
        mCenter(T(0), T(0), T(0)), 
        mRadius(T(1)) {}

    /**
     * @brief 带参数的构造函数
     * @param center 球体中心
     * @param radius 球体半径
     */
    TSphere(const TVector3<T>& center, T radius) :
        mCenter(center), 
        mRadius(radius) {}

    /**
     * @brief 检查点是否在球体内
     * @param point 点
     * @return 是否在球体内
     */
    bool Contains(const TVector3<T>& point) const
    {
        return (point - mCenter).LengthSquared() <= mRadius * mRadius;
    }

    /**
     * @brief 计算点到球体的最短距离
     * @param point 点
     * @return 最短距离
     */
    T DistanceTo(const TVector3<T>& point) const
    {
        T distance = (point - mCenter).Length();
        return std::max(T(0), distance - mRadius);
    }

    /**
     * @brief 获取球体的AABB
     * @return AABB包围盒
     */
    TAABB<T> GetAABB() const
    {
        TVector3<T> offset(mRadius, mRadius, mRadius);
        return TAABB<T>(mCenter - offset, mCenter + offset);
    }
};

// 类型别名
typedef TSphere<float> Spheref;
typedef TSphere<double> Sphered;
typedef TSphere<Real> Sphere;

} // namespace SympConv

#endif // SYMPCONV_SPHERE_H