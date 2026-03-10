#ifndef SYMPCONV_SPHERE_SHAPE_H
#define SYMPCONV_SPHERE_SHAPE_H

#include "ConvexShape.h"
#include "Sphere.h"

namespace SympConv {

/**
 * @brief 球体形状模板类
 * @details 实现了球体的凸形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TSphereShape : public TConvexShape<T>
{
public:
    /**
     * @brief 构造函数
     * @param sphere 球体对象
     */
    TSphereShape(const TSphere<T>& sphere) :
        TConvexShape<T>(EConvexShapeType::Sphere), mSphere(sphere) {}

    /**
     * @brief 获取在指定方向上的支持点
     * @param direction 方向向量
     * @return 支持点
     */
    Vector3 GetSupport(const Vector3& direction) const override
    {
        TVector3<T> center = mSphere.mCenter;
        T radius = mSphere.mRadius;
        
        // 计算支持点：球心加上方向向量的单位向量乘以半径
        TVector3<T> dir = TVector3<T>(direction.x, direction.y, direction.z);
        T length = dir.Length();
        if (length > T(0)) {
            dir = dir / length;
        } else {
            dir = TVector3<T>(T(1), T(0), T(0)); // 默认为x轴方向
        }
        
        TVector3<T> support = center + dir * radius;
        
        // 直接返回支持点，因为Sphere类没有变换
        return Vector3(support.x, support.y, support.z);
    }

    /**
     * @brief 获取球体对象
     * @return 球体对象的常量引用
     */
    const TSphere<T>& GetSphere() const { return mSphere; }

private:
    TSphere<T> mSphere; ///< 球体对象
};

// 类型别名
typedef TSphereShape<float> SphereShapef;  ///< 单精度球体形状
typedef TSphereShape<double> SphereShaped; ///< 双精度球体形状
typedef TSphereShape<fpnumber> SphereShape; ///< 根据配置的精度球体形状

} // namespace SympConv

#endif // SYMPCONV_SPHERE_SHAPE_H