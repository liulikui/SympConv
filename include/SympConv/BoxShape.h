#ifndef SYMPCONV_BOX_SHAPE_H
#define SYMPCONV_BOX_SHAPE_H

#include "Export.h"
#include "ConvexShape.h"
#include "Box.h"
#include "Ray.h"
#include "CollisionDetection.h"
#include "RayCast.h"

namespace SympConv {

/**
 * @brief 盒子形状模板类
 * @details 实现了盒子的凸形状接口，用于碰撞检测等场景
 */
class SYMPCONV_API BoxShape : public ConvexShape
{
public:
    /**
     * @brief 构造函数
     * @param box 盒子对象
     */
    BoxShape(const Box& box) :
        ConvexShape(ShapeType::Box), mBox(box) {}

    /**
     * @brief 获取盒子对象
     * @return 盒子对象的常量引用
     */
    const Box& GetBox() const { return mBox; }

    /**
     * @brief 在本地坐标系中获取支持点
     * @param dir_local 本地坐标系中的方向向量
     * @return 支持点
     */
    virtual Vector3 GetLocalSupport(const Vector3& dir_local) const override
    {
        Vector3 extents = mBox.mHalfExtents;
        Vector3 local_support(0, 0, 0);
        
        // 计算局部坐标系中的支持点
        local_support.x = extents.x * (dir_local.x > 0 ? 1 : (dir_local.x < 0 ? -1 : 0));
        local_support.y = extents.y * (dir_local.y > 0 ? 1 : (dir_local.y < 0 ? -1 : 0));
        local_support.z = extents.z * (dir_local.z > 0 ? 1 : (dir_local.z < 0 ? -1 : 0));
        
        return local_support;
    }

    /**
     * @brief 获取在本地坐标系中的惯性张量
     * @param mass 质量
     * @return 惯性张量
     */
    virtual Vector3 GetLocalInertiaTensor(Real mass) const override
    {
        // 计算盒子的全长（半长乘以2）
        Real width = mBox.mHalfExtents.x * 2;
        Real height = mBox.mHalfExtents.y * 2;
        Real depth = mBox.mHalfExtents.z * 2;
        
        // 计算惯性张量分量
        // Ix = (1/12) * m * (height² + depth²)
        // Iy = (1/12) * m * (width² + depth²)
        // Iz = (1/12) * m * (width² + height²)
        Real ix = (mass / 12.0f) * (height * height + depth * depth);
        Real iy = (mass / 12.0f) * (width * width + depth * depth);
        Real iz = (mass / 12.0f) * (width * width + height * height);
        
        return Vector3(ix, iy, iz);
    }

    /**
     * @brief 获取在本地坐标系中的AABB
     * @return AABB
     */
    virtual AABB GetLocalBounds() const override
    {
        // 盒子的半长
        Vector3 halfExtents = mBox.mHalfExtents;
        
        // 计算AABB的最小和最大点
        Vector3 min(-halfExtents.x, -halfExtents.y, -halfExtents.z);
        Vector3 max(halfExtents.x, halfExtents.y, halfExtents.z);
        
        // 创建并返回AABB
        return AABB(min, max);
    }

    /**
     * @brief 获取体积
     * @return 体积
     */
    virtual Real GetVolume() const override
    {
        return mBox.GetVolume();
    }

    /**
     * @brief 射线检测
     * @param ray 本地空间射线
     * @param result 命中结果
     * @return 是否有命中
     */
    virtual bool RayCast(const Ray& ray, RayCastResult& result) const override;

private:
    Box mBox; ///< 盒子对象
};

} // namespace SympConv

#endif // SYMPCONV_BOX_SHAPE_H
