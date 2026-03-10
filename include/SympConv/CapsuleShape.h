#ifndef SYMPCONV_CAPSULE_SHAPE_H
#define SYMPCONV_CAPSULE_SHAPE_H

#include "ConvexShape.h"
#include "Capsule.h"

namespace SympConv {

/**
 * @brief 胶囊体形状模板类
 * @details 实现了胶囊体的凸形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TCapsuleShape : public TConvexShape<T>
{
public:
    /**
     * @brief 构造函数
     * @param capsule 胶囊体对象
     */
    TCapsuleShape(const TCapsule<T>& capsule) :
        TConvexShape<T>(EConvexShapeType::Capsule), mCapsule(capsule) {}

    /**
     * @brief 获取在指定方向上的支持点
     * @param direction 方向向量
     * @return 支持点
     */
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

    /**
     * @brief 获取胶囊体对象
     * @return 胶囊体对象的常量引用
     */
    const TCapsule<T>& GetCapsule() const { return mCapsule; }

private:
    TCapsule<T> mCapsule; ///< 胶囊体对象
};

// 类型别名
typedef TCapsuleShape<float> CapsuleShapef;  ///< 单精度胶囊体形状
typedef TCapsuleShape<double> CapsuleShaped; ///< 双精度胶囊体形状
typedef TCapsuleShape<fpnumber> CapsuleShape; ///< 根据配置的精度胶囊体形状

} // namespace SympConv

#endif // SYMPCONV_CAPSULE_SHAPE_H