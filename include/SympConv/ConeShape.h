#ifndef SYMPCONV_CONE_SHAPE_H
#define SYMPCONV_CONE_SHAPE_H

#include "Export.h"
#include "ConvexShape.h"
#include "Cone.h"
#include "Ray.h"
#include "RayCast.h"
#include "Plane.h"
#include <cmath>

namespace SympConv {

/**
 * @brief 圆锥体形状模板类
 * @details 实现了圆锥体的凸形状接口，用于碰撞检测等场景
 */
class SYMPCONV_API ConeShape : public ConvexShape
{
public:
    /**
     * @brief 构造函数
     * @param cone 圆锥体对象
     */
    ConeShape(const Cone& cone) :
        ConvexShape(ShapeType::Cone), mCone(cone) {}

    /**
     * @brief 获取圆锥体对象
     * @return 圆锥体对象的常量引用
     */
    const Cone& GetCone() const { return mCone; }

    /**
     * @brief 在本地坐标系中获取支持点
     * @param dir_local 本地坐标系中的方向向量
     * @return 支持点
     */
    Vector3 GetLocalSupport(const Vector3& dir_local) const override
    {
        // 圆锥体的底面在y=0平面，顶点在(0, height, 0)（左手系，y轴朝上）
        Vector3 support;
        
        // 计算方向向量在xz平面上的分量
        Vector3 dir_xz(dir_local.x, 0, dir_local.z);
        Real length_xz = dir_xz.Length();
        
        if (dir_local.y > 0)
        {
            // 方向向上，支持点是圆锥的顶点
            support = Vector3(0, mCone.mHeight, 0);
        }
        else if (length_xz > 1e-10)
        {
            // 方向向下且在xz平面上有分量，支持点在底面上
            Vector3 dir_xz_normalized = dir_xz / length_xz;
            support.x = dir_xz_normalized.x * mCone.mRadius;
            support.y = 0;
            support.z = dir_xz_normalized.z * mCone.mRadius;
        }
        else
        {
            // 方向直接向下，支持点是底面中心
            support = Vector3(0, 0, 0);
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
        // 圆锥体绕Y轴（对称轴）的转动惯量：Iy = (3/10) * m * r²
        // 圆锥体绕X轴和Z轴的转动惯量：Ix = Iz = (3/20) * m * (r² + 4h²)
        Real radius = mCone.mRadius;
        Real height = mCone.mHeight;
        
        Real iy = (3.0f / 10.0f) * mass * radius * radius;
        Real ix_iz = (3.0f / 20.0f) * mass * (radius * radius + 4.0f * height * height);
        
        return Vector3(ix_iz, iy, ix_iz);
    }

    /**
     * @brief 获取在本地坐标系中的AABB
     * @return AABB
     */
    virtual AABB GetLocalBounds() const override
    {
        // 圆锥体的高度和半径
        Real height = mCone.mHeight;
        Real radius = mCone.mRadius;
        
        // 计算AABB的最小和最大点
        Vector3 min(-radius, 0, -radius);
        Vector3 max(radius, height, radius);
        
        // 创建并返回AABB
        return AABB(min, max);
    }

    /**
     * @brief 获取体积
     * @return 体积
     */
    virtual Real GetVolume() const override
    {
        return mCone.GetVolume();
    }

    /**
     * @brief 射线检测
     * @param ray 本地空间射线
     * @param result 命中结果
     * @return 是否有命中
     */
    virtual bool RayCast(const Ray& ray, RayCastResult& result) const override;

private:
    Cone mCone; ///< 圆锥体对象
};

} // namespace SympConv

#endif // SYMPCONV_CONE_SHAPE_H