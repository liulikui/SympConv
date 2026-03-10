#ifndef SYMPCONV_CAPSULE_SHAPE_H
#define SYMPCONV_CAPSULE_SHAPE_H

#include "ConvexShape.h"
#include "Capsule.h"

namespace SympConv {

template<typename T>
class TCapsuleShape : public TConvexShape<T>
{
public:
    TCapsuleShape(const TCapsule<T>& capsule) :
        TConvexShape<T>(EConvexShapeType::Capsule), mCapsule(capsule) {}

    Vector3 GetSupport(const Vector3& direction) const override
    {
        TVector3<T> start = mCapsule.mStart;
        TVector3<T> end = mCapsule.mEnd;
        T radius = mCapsule.mRadius;
        
        // 计算胶囊体的方向向量和长度
        TVector3<T> axis = (end - start).Normalize();
        T length = (end - start).Length();
        TVector3<T> center = (start + end) * T(0.5);
        
        // 计算方向向量在胶囊体轴方向上的投影
        TVector3<T> dir = TVector3<T>(direction.x, direction.y, direction.z);
        T proj = dir.Dot(axis);
        
        // 计算方向向量在垂直于胶囊体轴方向上的分量
        TVector3<T> perp_dir = dir - axis * proj;
        T perp_len = perp_dir.Length();
        
        TVector3<T> support;
        
        if (perp_len > T(0)) {
            // 计算垂直方向的单位向量
            TVector3<T> perp_unit = perp_dir / perp_len;
            
            // 计算沿轴方向的偏移量，限制在胶囊体长度范围内
            T axial_offset = std::max(T(-0.5) * length, std::min(T(0.5) * length, proj));
            
            // 计算支持点
            support = center + axis * axial_offset + perp_unit * radius;
        } else {
            // 方向与轴平行，支持点在胶囊体的端点（半球的顶点）
            TVector3<T> end_point = (proj > T(0)) ? end : start;
            support = end_point + axis * ((proj > T(0)) ? radius : -radius);
        }
        
        // 直接返回支持点，因为Capsule类没有变换
        return Vector3(support.x, support.y, support.z);
    }

    const TCapsule<T>& GetCapsule() const { return mCapsule; }

private:
    TCapsule<T> mCapsule;
};

// 类型别名
typedef TCapsuleShape<float> CapsuleShapef;
typedef TCapsuleShape<double> CapsuleShaped;
typedef TCapsuleShape<fpnumber> CapsuleShape;

} // namespace SympConv

#endif // SYMPCONV_CAPSULE_SHAPE_H