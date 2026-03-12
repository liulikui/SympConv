#ifndef SYMPCONV_CYLINDER_SHAPE_H
#define SYMPCONV_CYLINDER_SHAPE_H

#include "ConvexShape.h"
#include "Cylinder.h"
#include <algorithm>

namespace SympConv {

/**
 * @brief 圆柱体形状模板类
 * @details 实现了圆柱体的凸形状接口，用于碰撞检测等场景
 */
class CylinderShape : public ConvexShape
{
public:
    /**
     * @brief 构造函数
     * @param cylinder 圆柱体对象
     */
    CylinderShape(const Cylinder& cylinder) :
        ConvexShape(ShapeType::Cylinder), mCylinder(cylinder) {}

    /**
     * @brief 获取圆柱体对象
     * @return 圆柱体对象的常量引用
     */
    const Cylinder& GetCylinder() const { return mCylinder; }

    /**
     * @brief 在本地坐标系中获取支持点
     * @param dir_local 本地坐标系中的方向向量
     * @return 支持点
     */
    virtual Vector3 GetLocalSupport(const Vector3& dir_local) const override
    {
        // 圆柱体沿Y轴方向，中心点在原点
        // 计算方向向量在XZ平面上的分量
        Vector3 dir_xz(dir_local.x, 0, dir_local.z);
        Real length_xz = dir_xz.Length();
        
        Vector3 support;
        
        if (length_xz > 1e-10)
        {
            // 方向向量在XZ平面上有分量，支持点在侧面
            Vector3 dir_xz_normalized = dir_xz / length_xz;
            support.x = dir_xz_normalized.x * mCylinder.mRadius;
            support.z = dir_xz_normalized.z * mCylinder.mRadius;
            
            // 计算Y分量，取顶面或底面
            support.y = dir_local.y >= 0 ? mCylinder.mHalfHeight : -mCylinder.mHalfHeight;
        }
        else
        {
            // 方向向量沿Y轴方向，支持点在顶面或底面的中心
            support.x = 0;
            support.y = dir_local.y >= 0 ? mCylinder.mHalfHeight : -mCylinder.mHalfHeight;
            support.z = 0;
        }
        
        return support;
    }

    /**
     * @brief 获取在本地坐标系中的惯性张量
     * @param mass 质量
     * @return 惯性张量
     */
    virtual Vector3 GetLocalInertiaTensor(Real mass) const override
    {
        return Vector3(Real(0.0), Real(0.0), Real(0.0));
    }

    /**
     * @brief 获取在本地坐标系中的AABB
     * @return AABB
     */
    virtual AABB GetLocalBounds() const override
    {
        // 圆柱体的半高和半径
        Real halfHeight = mCylinder.mHalfHeight;
        Real radius = mCylinder.mRadius;
        
        // 计算AABB的最小和最大点
        Vector3 min(-radius, -halfHeight, -radius);
        Vector3 max(radius, halfHeight, radius);
        
        // 创建并返回AABB
        return AABB(min, max);
    }

    /**
     * @brief 获取体积
     * @return 体积
     */
    virtual Real GetVolume() const override
    {
        return mCylinder.GetVolume();
    }

private:
    Cylinder mCylinder; ///< 圆柱体对象
};

} // namespace SympConv

#endif // SYMPCONV_CYLINDER_SHAPE_H