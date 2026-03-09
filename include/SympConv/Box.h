#ifndef SYMPCONV_BOX_H
#define SYMPCONV_BOX_H

#include "Config.h"
#include "Vector.h"
#include "Transform.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 盒子模板类
 * @details 表示三维空间中的盒子，由中心、半长和变换定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TBox
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mCenter;      ///< 盒子中心
    TVector3<T> mHalfExtents; ///< 半长（从中心到边的距离）
    TTransform<T> mTransform; ///< 盒子的变换（旋转和缩放）

public:
    /**
     * @brief 默认构造函数
     */
    TBox() : 
        mCenter(T(0), T(0), T(0)), 
        mHalfExtents(T(0.5), T(0.5), T(0.5)),
        mTransform() {}

    /**
     * @brief 带参数的构造函数
     * @param center 盒子中心
     * @param halfExtents 半长
     * @param transform 变换
     */
    TBox(const TVector3<T>& center, const TVector3<T>& halfExtents, const TTransform<T>& transform) :
        mCenter(center), 
        mHalfExtents(halfExtents),
        mTransform(transform) {}

    /**
     * @brief 带参数的构造函数（无变换）
     * @param center 盒子中心
     * @param halfExtents 半长
     */
    TBox(const TVector3<T>& center, const TVector3<T>& halfExtents) :
        mCenter(center), 
        mHalfExtents(halfExtents),
        mTransform() {}

    /**
     * @brief 计算盒子的8个顶点
     * @param vertices 输出顶点数组
     */
    void GetVertices(TVector3<T> vertices[8]) const
    {
        // 局部坐标系中的顶点
        TVector3<T> localVertices[8] = {
            TVector3<T>(-mHalfExtents.x, -mHalfExtents.y, -mHalfExtents.z),
            TVector3<T>(mHalfExtents.x, -mHalfExtents.y, -mHalfExtents.z),
            TVector3<T>(-mHalfExtents.x, mHalfExtents.y, -mHalfExtents.z),
            TVector3<T>(mHalfExtents.x, mHalfExtents.y, -mHalfExtents.z),
            TVector3<T>(-mHalfExtents.x, -mHalfExtents.y, mHalfExtents.z),
            TVector3<T>(mHalfExtents.x, -mHalfExtents.y, mHalfExtents.z),
            TVector3<T>(-mHalfExtents.x, mHalfExtents.y, mHalfExtents.z),
            TVector3<T>(mHalfExtents.x, mHalfExtents.y, mHalfExtents.z)
        };

        // 应用变换并添加中心
        for (int i = 0; i < 8; ++i) {
            vertices[i] = mTransform.TransformPoint(localVertices[i]) + mCenter;
        }
    }

    /**
     * @brief 检查点是否在盒子内
     * @param point 点
     * @return 是否在盒子内
     */
    bool Contains(const TVector3<T>& point) const
    {
        // 将点转换到盒子的局部坐标系
        TVector3<T> localPoint = mTransform.InverseTransformPoint(point - mCenter);

        // 检查是否在局部坐标系的盒子内
        return localPoint.x >= -mHalfExtents.x && localPoint.x <= mHalfExtents.x &&
               localPoint.y >= -mHalfExtents.y && localPoint.y <= mHalfExtents.y &&
               localPoint.z >= -mHalfExtents.z && localPoint.z <= mHalfExtents.z;
    }

    /**
     * @brief 获取盒子的AABB
     * @return AABB包围盒
     */
    TAABB<T> GetAABB() const
    {
        TVector3<T> vertices[8];
        GetVertices(vertices);
        return TAABB<T>::FromPoints(vertices, 8);
    }
};

// 类型别名
typedef TBox<float> Boxf;
typedef TBox<double> Boxd;
typedef TBox<fpnumber> Box;

} // namespace SympConv

#endif // SYMPCONV_BOX_H