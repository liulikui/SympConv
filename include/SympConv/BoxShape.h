#ifndef SYMPCONV_BOX_SHAPE_H
#define SYMPCONV_BOX_SHAPE_H

#include "ConvexShape.h"
#include "Box.h"

namespace SympConv {

template<typename T>
class TBoxShape : public TConvexShape<T>
{
public:
    TBoxShape(const TBox<T>& box) :
        TConvexShape<T>(EConvexShapeType::Box), mBox(box) {}

    Vector3 GetSupport(const Vector3& direction) const override
    {
        TVector3<T> extents = mBox.mHalfExtents;
        TVector3<T> local_support(0, 0, 0);
        
        // 计算局部坐标系中的支持点
        if (direction.x > 0) {
            local_support.x = extents.x;
        } else if (direction.x < 0) {
            local_support.x = -extents.x;
        } else {
            local_support.x = 0;
        }
        
        if (direction.y > 0) {
            local_support.y = extents.y;
        } else if (direction.y < 0) {
            local_support.y = -extents.y;
        } else {
            local_support.y = 0;
        }
        
        if (direction.z > 0) {
            local_support.z = extents.z;
        } else if (direction.z < 0) {
            local_support.z = -extents.z;
        } else {
            local_support.z = 0;
        }
        
        // 应用变换并添加中心
        TVector3<T> support = mBox.mTransform.TransformPoint(local_support) + mBox.mCenter;
        
        return Vector3(support.x, support.y, support.z);
    }

    const TBox<T>& GetBox() const { return mBox; }

private:
    TBox<T> mBox;
};

// 类型别名
typedef TBoxShape<float> BoxShapef;
typedef TBoxShape<double> BoxShaped;
typedef TBoxShape<fpnumber> BoxShape;

} // namespace SympConv

#endif // SYMPCONV_BOX_SHAPE_H
