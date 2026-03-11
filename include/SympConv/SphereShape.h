#ifndef SYMPCONV_SPHERE_SHAPE_H
#define SYMPCONV_SPHERE_SHAPE_H

#include "ConvexShape.h"
#include "Sphere.h"

namespace SympConv {

/**
 * @brief 球体形状模板类
 * @details 实现了球体的凸形状接口，用于碰撞检测等场景
 */
class SphereShape : public ConvexShape
{
public:
    /**
     * @brief 构造函数
     * @param sphere 球体对象
     */
    SphereShape(fpnumber radius) :
        ConvexShape(ShapeType::Sphere), mRadius(radius) {}

    /**
     * @brief 在本地坐标系中获取支持点
     * @param dir_local 本地坐标系中的方向向量
     * @return 支持点
     */
    virtual Vector3 GetLocalSupport(const Vector3& dir_local) const override
    {
        // 计算方向向量的长度
        fpnumber dir_length = dir_local.Length();
        if (dir_length < 1e-10)
        {
            // 零方向向量，返回原点
            return Vector3(0, 0, 0);
        }
        
        // 归一化方向向量并乘以半径
        Vector3 support = dir_local / dir_length * mRadius;
        
        return support;
    }

    /**
     * @brief 获取在本地坐标系中的惯性张量
     * @param mass 质量
     * @return 惯性张量
     */
    virtual Vector3 GetLocalInertiaTensor(fpnumber mass) const override
    {
        // 球体的转动惯量：I = (2/5) * m * r²
        // 由于球体是对称的，绕三个轴的转动惯量都相等
        fpnumber inertia = (2.0f / 5.0f) * mass * mRadius * mRadius;
        return Vector3(inertia, inertia, inertia);
    }

private:
    fpnumber mRadius; ///< 半径
};

} // namespace SympConv

#endif // SYMPCONV_SPHERE_SHAPE_H