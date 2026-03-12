#ifndef SYMPCONV_CAPSULE_SHAPE_H
#define SYMPCONV_CAPSULE_SHAPE_H

#include <cmath>
#include "ConvexShape.h"
#include "Capsule.h"
#include "Math.h"

namespace SympConv {

/**
 * @brief 胶囊体形状模板类
 * @details 实现了胶囊体的凸形状接口，用于碰撞检测等场景
 */
class CapsuleShape : public ConvexShape
{
public:
    /**
     * @brief 构造函数
     * @param capsule 胶囊体对象
     */
    CapsuleShape(const Capsule& capsule) :
        ConvexShape(ShapeType::Capsule), mCapsule(capsule) {}

    /**
     * @brief 获取胶囊体对象
     * @return 胶囊体对象的常量引用
     */
    const Capsule& GetCapsule() const { return mCapsule; }

    /**
     * @brief 在本地坐标系中获取支持点
     * @param dir_local 本地坐标系中的方向向量
     * @return 支持点
     */
    virtual Vector3 GetLocalSupport(const Vector3& dir_local) const override
    {
        // 胶囊体沿Y轴方向，中心点在原点
        // 两个端点的位置
        Vector3 p1(0, -mCapsule.mHalfHeight, 0);
        Vector3 p2(0, mCapsule.mHalfHeight, 0);
        
        // 计算方向向量的长度
        Real dir_length = dir_local.Length();
        if (dir_length < 1e-10)
        {
            // 零方向向量，返回上顶点加上半径
            return Vector3(0, mCapsule.mHalfHeight + mCapsule.mRadius, 0);
        }
        
        // 归一化方向向量
        Vector3 dir_normalized = dir_local / dir_length;
        
        // 计算方向向量在Y轴上的投影长度
        Real proj_y = dir_normalized.y;
        
        // 计算胶囊体轴线上的点
        Vector3 axis_point;
        if (proj_y >= 0)
        {
            // 方向向上或在XZ平面上，使用上顶点
            axis_point = p2;
        }
        else
        {
            // 方向向下，使用下顶点
            axis_point = p1;
        }
        
        // 计算垂直于Y轴的方向向量
        Vector3 dir_perp = dir_normalized - Vector3(0, proj_y, 0);
        Real perp_length = dir_perp.Length();
        
        if (perp_length < 1e-10)
        {
            // 方向向量沿着Y轴，直接返回顶点加上半径
            return axis_point + dir_normalized * mCapsule.mRadius;
        }
        
        // 归一化垂直方向向量
        dir_perp = dir_perp / perp_length;
        
        // 计算支持点：顶点加上垂直方向的半径
        Vector3 support = axis_point + dir_perp * mCapsule.mRadius;
        
        return support;
    }

    /**
     * @brief 获取在本地坐标系中的惯性张量
     * @param mass 质量
     * @return 惯性张量
     */
    virtual Vector3 GetLocalInertiaTensor(Real mass) const override
    {
        // 胶囊体沿Y轴方向
        Real radius = mCapsule.mRadius;
        Real height = mCapsule.mHalfHeight * 2; // 总高度
        
        // 计算体积
        Real volume_cylinder = M_PI * radius * radius * height;
        Real volume_hemispheres = (4.0f / 3.0f) * M_PI * radius * radius * radius;
        Real volume_total = volume_cylinder + volume_hemispheres;
        
        // 计算各部分质量
        Real mass_cylinder = mass * volume_cylinder / volume_total;
        Real mass_hemisphere = mass * (volume_hemispheres / 2.0f) / volume_total;
        
        // 计算圆柱体的惯性张量
        Real ix_cylinder = (1.0f / 12.0f) * mass_cylinder * (3.0f * radius * radius + height * height);
        Real iz_cylinder = ix_cylinder;
        Real iy_cylinder = (1.0f / 2.0f) * mass_cylinder * radius * radius;
        
        // 计算一个半球体的惯性张量
        Real ix_hemisphere = (2.0f / 5.0f) * mass_hemisphere * radius * radius + mass_hemisphere * (height / 2.0f) * (height / 2.0f);
        Real iz_hemisphere = ix_hemisphere;
        Real iy_hemisphere = (2.0f / 5.0f) * mass_hemisphere * radius * radius;
        
        // 总惯性张量（两个半球体）
        Real ix_total = ix_cylinder + 2.0f * ix_hemisphere;
        Real iy_total = iy_cylinder + 2.0f * iy_hemisphere;
        Real iz_total = iz_cylinder + 2.0f * iz_hemisphere;
        
        return Vector3(ix_total, iy_total, iz_total);
    }

    /**
     * @brief 获取在本地坐标系中的AABB
     * @return AABB
     */
    virtual AABB GetLocalBounds() const override
    {
        // 胶囊体的半高和半径
        Real halfHeight = mCapsule.mHalfHeight;
        Real radius = mCapsule.mRadius;
        
        // 计算AABB的最小和最大点
        Vector3 min(-radius, -halfHeight - radius, -radius);
        Vector3 max(radius, halfHeight + radius, radius);
        
        // 创建并返回AABB
        return AABB(min, max);
    }

    /**
     * @brief 获取体积
     * @return 体积
     */
    virtual Real GetVolume() const override
    {
        return mCapsule.GetVolume();
    }

private:
    Capsule mCapsule; ///< 胶囊体对象
};

} // namespace SympConv

#endif // SYMPCONV_CAPSULE_SHAPE_H