#ifndef SYMPCONV_CONE_H
#define SYMPCONV_CONE_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 圆锥体模板类
 * @details 表示三维空间中的圆锥体，由顶点、底面中心和半径定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TCone
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mVertex; ///< 圆锥体顶点
    TVector3<T> mBaseCenter; ///< 圆锥体底面中心
    T mRadius; ///< 圆锥体底面半径

public:
    /**
     * @brief 默认构造函数
     */
    TCone() : 
        mVertex(T(0), T(0), T(1)), 
        mBaseCenter(T(0), T(0), T(0)),
        mRadius(T(0.5)) {}

    /**
     * @brief 带参数的构造函数
     * @param vertex 顶点
     * @param baseCenter 底面中心
     * @param radius 底面半径
     */
    TCone(const TVector3<T>& vertex, const TVector3<T>& baseCenter, T radius) :
        mVertex(vertex), 
        mBaseCenter(baseCenter),
        mRadius(radius) {}

    /**
     * @brief 计算圆锥体的中心点
     * @return 中心点
     */
    TVector3<T> GetCenter() const
    {
        return (mVertex + mBaseCenter) * T(0.5);
    }

    /**
     * @brief 计算圆锥体的高度
     * @return 高度
     */
    T GetHeight() const
    {
        return (mVertex - mBaseCenter).Length();
    }

    /**
     * @brief 计算圆锥体的方向向量
     * @return 方向向量
     */
    TVector3<T> GetDirection() const
    {
        return (mVertex - mBaseCenter).Normalize();
    }

    /**
     * @brief 获取圆锥体的AABB
     * @return AABB包围盒
     */
    TAABB<T> GetAABB() const
    {
        TVector3<T> offset(mRadius, mRadius, mRadius);
        TVector3<T> min(
            std::min(mVertex.x, mBaseCenter.x) - mRadius,
            std::min(mVertex.y, mBaseCenter.y) - mRadius,
            std::min(mVertex.z, mBaseCenter.z)
        );
        TVector3<T> max(
            std::max(mVertex.x, mBaseCenter.x) + mRadius,
            std::max(mVertex.y, mBaseCenter.y) + mRadius,
            std::max(mVertex.z, mBaseCenter.z)
        );
        return TAABB<T>(min, max);
    }

    /**
     * @brief 检查点是否在圆锥体内
     * @param point 点
     * @return 是否在圆锥体内
     */
    bool Contains(const TVector3<T>& point) const
    {
        TVector3<T> axis = mVertex - mBaseCenter;
        T height = axis.Length();
        if (height < T(1e-6)) {
            return false;
        }
        
        TVector3<T> ap = point - mBaseCenter;
        T t = ap.Dot(axis) / axis.Dot(axis);
        
        // 检查点是否在圆锥体的轴线方向范围内
        if (t < 0 || t > 1) {
            return false;
        }
        
        // 计算圆锥体在该高度处的半径
        T radiusAtHeight = mRadius * (T(1) - t);
        
        // 计算点到轴线的距离
        TVector3<T> closest = mBaseCenter + axis * t;
        T distanceSquared = (point - closest).LengthSquared();
        
        return distanceSquared <= radiusAtHeight * radiusAtHeight;
    }
};

// 类型别名
typedef TCone<float> Conef;
typedef TCone<double> Coned;
typedef TCone<fpnumber> Cone;

} // namespace SympConv

#endif // SYMPCONV_CONE_H