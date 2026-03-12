#include "SympConv/SphereShape.h"
#include "SympConv/CollisionDetection.h"

namespace SympConv {

bool SphereShape::RayCast(const Ray& ray, RayCastResult& result) const
{
    // 球体的中心在原点
    Vector3 center(0, 0, 0);
    Real t1, t2;
    
    if (RayIntersectsSphere(ray, center, mRadius, t1, t2)) {
        result.mPoint = ray.GetPoint(t1);
        // 计算命中法线（从球心指向命中点）
        result.mNormal = (result.mPoint - center).Normalize();
        result.mHit = t1;
        return true;
    }
    
    return false;
}

} // namespace SympConv