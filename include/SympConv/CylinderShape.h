#ifndef SYMPCONV_CYLINDER_SHAPE_H
#define SYMPCONV_CYLINDER_SHAPE_H

#include "ConvexShape.h"
#include "Cylinder.h"

namespace SympConv {

template<typename T>
class TCylinderShape : public TConvexShape<T>
{
public:
    TCylinderShape(const TCylinder<T>& cylinder) :
        TConvexShape<T>(EConvexShapeType::Cylinder), mCylinder(cylinder) {}

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

    const TCylinder<T>& GetCylinder() const { return mCylinder; }

private:
    TCylinder<T> mCylinder;
};

// 类型别名
typedef TCylinderShape<float> CylinderShapef;
typedef TCylinderShape<double> CylinderShaped;
typedef TCylinderShape<fpnumber> CylinderShape;

} // namespace SympConv

#endif // SYMPCONV_CYLINDER_SHAPE_H