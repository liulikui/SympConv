#ifndef SYMPCONV_BOX_SHAPE_H
#define SYMPCONV_BOX_SHAPE_H

#include "ConvexShape.h"
#include "Box.h"

namespace SympConv {

/**
 * @brief 盒子形状模板类
 * @details 实现了盒子的凸形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TBoxShape : public TConvexShape<T>
{
public:
    /**
     * @brief 构造函数
     * @param box 盒子对象
     */
    TBoxShape(const TBox<T>& box) :
        TConvexShape<T>(EConvexShapeType::Box), mBox(box) {}

    /**
     * @brief 获取在指定方向上的支持点
     * @param direction 方向向量
     * @return 支持点
     */
    Vector3 GetSupport(const Vector3& direction) const override
    {
        TVector3<T> extents = mBox.mHalfExtents;
        TVector3<T> local_support(0, 0, 0);
        
        // 将方向向量转换到局部坐标系
        TVector3<T> world_direction(direction.x, direction.y, direction.z);
        TVector3<T> local_direction = mBox.mTransform.InverseTransformDirection(world_direction);
        
        // 计算局部坐标系中的支持点
        local_support.x = extents.x * (local_direction.x > 0 ? 1 : (local_direction.x < 0 ? -1 : 0));
        local_support.y = extents.y * (local_direction.y > 0 ? 1 : (local_direction.y < 0 ? -1 : 0));
        local_support.z = extents.z * (local_direction.z > 0 ? 1 : (local_direction.z < 0 ? -1 : 0));
        
        // 应用变换（旋转和缩放）
        TVector3<T> world_support = mBox.mTransform.TransformVector(local_support);
        
        // 添加中心
        world_support += mBox.mCenter;
        
        return Vector3(world_support.x, world_support.y, world_support.z);
    }

    /**
     * @brief 获取盒子对象
     * @return 盒子对象的常量引用
     */
    const TBox<T>& GetBox() const { return mBox; }

private:
    TBox<T> mBox; ///< 盒子对象
};

// 类型别名
typedef TBoxShape<float> BoxShapef;  ///< 单精度盒子形状
typedef TBoxShape<double> BoxShaped; ///< 双精度盒子形状
typedef TBoxShape<fpnumber> BoxShape; ///< 根据配置的精度盒子形状

} // namespace SympConv

#endif // SYMPCONV_BOX_SHAPE_H
