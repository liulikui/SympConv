#ifndef SYMPCONV_CYLINDER_SHAPE_H
#define SYMPCONV_CYLINDER_SHAPE_H

#include "ConvexShape.h"
#include "Cylinder.h"

namespace SympConv {

/**
 * @brief 圆柱体形状模板类
 * @details 实现了圆柱体的凸形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TCylinderShape : public TConvexShape<T>
{
public:
    /**
     * @brief 构造函数
     * @param cylinder 圆柱体对象
     */
    TCylinderShape(const TCylinder<T>& cylinder) :
        TConvexShape<T>(EConvexShapeType::Cylinder), mCylinder(cylinder) {}

    /**
     * @brief 获取在指定方向上的支持点
     * @param direction 方向向量
     * @return 支持点
     */
    Vector3 GetSupport(const Vector3& direction) const override
    {
        TVector3<T> start = mCylinder.mStart;
        TVector3<T> end = mCylinder.mEnd;
        T radius = mCylinder.mRadius;
        
        // 计算圆柱体的方向向量和长度
        TVector3<T> axis = (end - start).Normalize();
        T length = (end - start).Length();
        TVector3<T> center = (start + end) * T(0.5);
        
        // 计算方向向量在圆柱体轴方向上的投影
        TVector3<T> dir = TVector3<T>(direction.x, direction.y, direction.z);
        T proj = dir.Dot(axis);
        
        // 计算方向向量在垂直于圆柱体轴方向上的分量
        TVector3<T> perp_dir = dir - axis * proj;
        T perp_len = perp_dir.Length();
        
        TVector3<T> support;
        
        if (perp_len > T(0)) {
            // 计算垂直方向的单位向量
            TVector3<T> perp_unit = perp_dir / perp_len;
            
            // 计算沿轴方向的偏移量，限制在圆柱体长度范围内
            T axial_offset = std::max(T(-0.5) * length, std::min(T(0.5) * length, proj));
            
            // 计算支持点
            support = center + axis * axial_offset + perp_unit * radius;
        } else {
            // 方向与轴平行，支持点在圆柱体的端点
            support = (proj > T(0)) ? end : start;
        }
        
        // 直接返回支持点，因为Cylinder类没有变换
        return Vector3(support.x, support.y, support.z);
    }

    /**
     * @brief 获取圆柱体对象
     * @return 圆柱体对象的常量引用
     */
    const TCylinder<T>& GetCylinder() const { return mCylinder; }

private:
    TCylinder<T> mCylinder; ///< 圆柱体对象
};

// 类型别名
typedef TCylinderShape<float> CylinderShapef;  ///< 单精度圆柱体形状
typedef TCylinderShape<double> CylinderShaped; ///< 双精度圆柱体形状
typedef TCylinderShape<fpnumber> CylinderShape; ///< 根据配置的精度圆柱体形状

} // namespace SympConv

#endif // SYMPCONV_CYLINDER_SHAPE_H