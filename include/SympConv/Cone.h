#ifndef SYMPCONV_CONE_H
#define SYMPCONV_CONE_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include <type_traits>
#include <algorithm>

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
        return (mBaseCenter - mVertex).Normalize();
    }

    /**
     * @brief 获取圆锥体的AABB
     * @return AABB包围盒
     */
    TAABB<T> GetAABB() const
    {
        TVector3<T> min(
            std::min(mVertex.x, mBaseCenter.x) - mRadius,
            std::min(mVertex.y, mBaseCenter.y) - mRadius,
            std::min(mVertex.z, mBaseCenter.z) - mRadius
        );
        TVector3<T> max(
            std::max(mVertex.x, mBaseCenter.x) + mRadius,
            std::max(mVertex.y, mBaseCenter.y) + mRadius,
            std::max(mVertex.z, mBaseCenter.z) + mRadius
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
        TVector3<T> axis = mBaseCenter - mVertex;
        T height = axis.Length();
        if (height < T(1e-6)) {
            // 高度为0的圆锥退化为一个点
            return (point - mVertex).LengthSquared() < T(1e-12);
        }
        
        TVector3<T> ap = point - mVertex;
        T t = ap.Dot(axis) / (height * height);
        
        // 检查点是否在圆锥体的轴线方向范围内
        if (t < -T(1e-6) || t > T(1.000001)) {
            return false;
        }
        
        // 计算圆锥体在该高度处的半径
        T radiusAtHeight = mRadius * t;
        
        // 计算点到轴线的距离
        TVector3<T> closest = mVertex + axis * t;
        T distanceSquared = (point - closest).LengthSquared();
        T radiusSquared = radiusAtHeight * radiusAtHeight;
        
        // 计算epsilon值，根据圆锥的大小和方向
        T epsilon;
        
        // 检查圆锥是否为非坐标轴对齐
        bool isAxisAligned = (std::abs(axis.x) < T(1e-6) || std::abs(axis.y) < T(1e-6) || std::abs(axis.z) < T(1e-6));
        
        if (radiusSquared < T(1e-6)) {
            // 小圆锥使用较小的epsilon值
            epsilon = T(1e-9);
        } else if (radiusSquared > T(1e6)) {
            // 大圆锥使用相对误差
            epsilon = T(1e-6) * radiusSquared;
        } else if (!isAxisAligned) {
            // 非坐标轴对齐的圆锥使用更大的epsilon值，因为计算误差更大
            epsilon = T(5e-3);
        } else {
            // 坐标轴对齐的圆锥使用中等大小的epsilon值
            epsilon = T(1e-4);
        }
        
        return distanceSquared <= radiusSquared + epsilon;
    }
};

// 类型别名
typedef TCone<float> Conef;
typedef TCone<double> Coned;
typedef TCone<fpnumber> Cone;

} // namespace SympConv

#endif // SYMPCONV_CONE_H