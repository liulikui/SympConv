#ifndef SYMPCONV_SPHERE_SHAPE_H
#define SYMPCONV_SPHERE_SHAPE_H

#include "ConvexShape.h"
#include "Sphere.h"

namespace SympConv {

template<typename T>
class TSphereShape : public TConvexShape<T>
{
public:
    TSphereShape(const TSphere<T>& sphere) :
        TConvexShape<T>(EConvexShapeType::Sphere), mSphere(sphere) {}

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

    const TSphere<T>& GetSphere() const { return mSphere; }

private:
    TSphere<T> mSphere;
};

// 类型别名
typedef TSphereShape<float> SphereShapef;
typedef TSphereShape<double> SphereShaped;
typedef TSphereShape<fpnumber> SphereShape;

} // namespace SympConv

#endif // SYMPCONV_SPHERE_SHAPE_H