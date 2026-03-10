#ifndef SYMPCONV_CONE_SHAPE_H
#define SYMPCONV_CONE_SHAPE_H

#include "ConvexShape.h"
#include "Cone.h"

namespace SympConv {

/**
 * @brief 圆锥体形状模板类
 * @details 实现了圆锥体的凸形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TConeShape : public TConvexShape<T>
{
public:
    /**
     * @brief 构造函数
     * @param cone 圆锥体对象
     */
    TConeShape(const TCone<T>& cone) :
        TConvexShape<T>(EConvexShapeType::Cone), mCone(cone) {}

    /**
     * @brief 获取在指定方向上的支持点
     * @param direction 方向向量
     * @return 支持点
     */
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

    /**
     * @brief 获取圆锥体对象
     * @return 圆锥体对象的常量引用
     */
    const TCone<T>& GetCone() const { return mCone; }

private:
    TCone<T> mCone; ///< 圆锥体对象
};

// 类型别名
typedef TConeShape<float> ConeShapef;  ///< 单精度圆锥体形状
typedef TConeShape<double> ConeShaped; ///< 双精度圆锥体形状
typedef TConeShape<fpnumber> ConeShape; ///< 根据配置的精度圆锥体形状

} // namespace SympConv

#endif // SYMPCONV_CONE_SHAPE_H