#ifndef SYMPCONV_CONE_SHAPE_H
#define SYMPCONV_CONE_SHAPE_H

#include "ConvexShape.h"
#include "Cone.h"

namespace SympConv {

template<typename T>
class TConeShape : public TConvexShape<T>
{
public:
    TConeShape(const TCone<T>& cone) :
        TConvexShape<T>(EConvexShapeType::Cone), mCone(cone) {}

    Vector3 GetSupport(const Vector3& direction) const override
    {
        TVector3<T> vertex = mCone.mVertex;
        TVector3<T> baseCenter = mCone.mBaseCenter;
        T radius = mCone.mRadius;
        
        // 计算圆锥体的轴线向量和高度
        TVector3<T> axis = (vertex - baseCenter).Normalize();
        T height = (vertex - baseCenter).Length();
        
        // 计算方向向量在圆锥体轴方向上的投影
        TVector3<T> dir = TVector3<T>(direction.x, direction.y, direction.z);
        T proj = dir.Dot(axis);
        
        // 计算方向向量在垂直于圆锥体轴方向上的分量
        TVector3<T> perp_dir = dir - axis * proj;
        T perp_len = perp_dir.Length();
        
        TVector3<T> support;
        
        if (perp_len > T(0)) {
            // 计算垂直方向的单位向量
            TVector3<T> perp_unit = perp_dir / perp_len;
            
            // 计算圆锥体表面上的支持点
            // 对于圆锥体，支持点位于圆锥体的侧面或底面边缘
            if (proj > T(0)) {
                // 方向指向圆锥体顶点方向，支持点可能在顶点
                support = vertex;
            } else {
                // 方向指向圆锥体底面方向，支持点在底面边缘
                TVector3<T> baseEdge = baseCenter + perp_unit * radius;
                support = baseEdge;
            }
        } else {
            // 方向与轴平行，支持点在顶点或底面中心
            support = (proj > T(0)) ? vertex : baseCenter;
        }
        
        // 直接返回支持点，因为Cone类没有变换
        return Vector3(support.x, support.y, support.z);
    }

    const TCone<T>& GetCone() const { return mCone; }

private:
    TCone<T> mCone;
};

// 类型别名
typedef TConeShape<float> ConeShapef;
typedef TConeShape<double> ConeShaped;
typedef TConeShape<fpnumber> ConeShape;

} // namespace SympConv

#endif // SYMPCONV_CONE_SHAPE_H