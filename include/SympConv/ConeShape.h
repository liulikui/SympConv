#ifndef SYMPCONV_CONE_SHAPE_H
#define SYMPCONV_CONE_SHAPE_H

#include "Shape.h"
#include "Cone.h"

namespace SympConv {

/**
 * @brief 圆锥体形状模板类
 * @details 实现了圆锥体的凸形状接口，用于碰撞检测等场景
 */
class ConeShape : public Shape
{
public:
    /**
     * @brief 构造函数
     * @param cone 圆锥体对象
     */
    ConeShape(const Cone& cone) :
        Shape(ShapeGroupType::Convex, ShapeType::Cone), mCone(cone) {}

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
        fpnumber length_xz = dir_xz.Length();
        
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
    virtual Vector3 GetLocalInertiaTensor(fpnumber mass) const override
    {
        // 圆锥体绕Y轴（对称轴）的转动惯量：Iy = (3/10) * m * r²
        // 圆锥体绕X轴和Z轴的转动惯量：Ix = Iz = (3/20) * m * (r² + 4h²)
        fpnumber radius = mCone.mRadius;
        fpnumber height = mCone.mHeight;
        
        fpnumber iy = (3.0f / 10.0f) * mass * radius * radius;
        fpnumber ix_iz = (3.0f / 20.0f) * mass * (radius * radius + 4.0f * height * height);
        
        return Vector3(ix_iz, iy, ix_iz);
    }

    /**
     * @brief 获取圆锥体对象
     * @return 圆锥体对象的常量引用
     */
    const Cone& GetCone() const { return mCone; }

private:
    Cone mCone; ///< 圆锥体对象
};

} // namespace SympConv

#endif // SYMPCONV_CONE_SHAPE_H