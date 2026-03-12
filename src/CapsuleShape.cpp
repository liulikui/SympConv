#include "SympConv/CapsuleShape.h"
#include "SympConv/CollisionDetection.h"

namespace SympConv {

bool CapsuleShape::RayCast(const Ray& ray, RayCastResult& result) const
{
    // 胶囊体沿Y轴方向，中心点在原点
    Real radius = mCapsule.mRadius;
    Real halfHeight = mCapsule.mHalfHeight;
    
    Vector3 ro = ray.mOrigin;
    Vector3 rd = ray.mDirection;

    // 一般情况处理
    Real closestT = -1;
    
    // 检查侧面（圆柱体部分）
    Real a = rd.x * rd.x + rd.z * rd.z;
    Real b = 2 * (ro.x * rd.x + ro.z * rd.z);
    Real c = ro.x * ro.x + ro.z * ro.z - radius * radius;
    
    if (a > 1e-10) {
        Real discriminant = b * b - 4 * a * c;
        if (discriminant >= 0) {
            Real sqrtDiscriminant = std::sqrt(discriminant);
            Real t1 = (-b - sqrtDiscriminant) / (2 * a);
            Real t2 = (-b + sqrtDiscriminant) / (2 * a);
            
            if (t1 > 0) {
                Vector3 hitPoint = ray.GetPoint(t1);
                if (hitPoint.y >= -halfHeight && hitPoint.y <= halfHeight) {
                    closestT = t1;
                }
            }
            if (t2 > 0 && (t2 < closestT || closestT < 0)) {
                Vector3 hitPoint = ray.GetPoint(t2);
                if (hitPoint.y >= -halfHeight && hitPoint.y <= halfHeight) {
                    closestT = t2;
                }
            }
        }
    }
    
    // 检查顶部半球
    {
        Vector3 topCenter(0, halfHeight, 0);
        Vector3 oc = ro - topCenter;
        Real a = rd.Dot(rd);
        Real b = 2.0 * oc.Dot(rd);
        Real c = oc.Dot(oc) - radius * radius;
        Real discriminant = b * b - 4 * a * c;
        
        if (discriminant >= 0) {
            Real t1 = (-b - std::sqrt(discriminant)) / (2 * a);
            Real t2 = (-b + std::sqrt(discriminant)) / (2 * a);
            
            if (t1 > 0) {
                Vector3 hitPoint = ray.GetPoint(t1);
                if (hitPoint.y >= halfHeight - 1e-6) {
                    if (t1 < closestT || closestT < 0) {
                        closestT = t1;
                    }
                }
            }
            if (t2 > 0) {
                Vector3 hitPoint = ray.GetPoint(t2);
                if (hitPoint.y >= halfHeight - 1e-6) {
                    if (t2 < closestT || closestT < 0) {
                        closestT = t2;
                    }
                }
            }
        }
    }
    
    // 检查底部半球
    {
        Vector3 bottomCenter(0, -halfHeight, 0);
        Vector3 oc = ro - bottomCenter;
        Real a = rd.Dot(rd);
        Real b = 2.0 * oc.Dot(rd);
        Real c = oc.Dot(oc) - radius * radius;
        Real discriminant = b * b - 4 * a * c;
        
        if (discriminant >= 0) {
            Real t1 = (-b - std::sqrt(discriminant)) / (2 * a);
            Real t2 = (-b + std::sqrt(discriminant)) / (2 * a);
            
            if (t1 > 0) {
                Vector3 hitPoint = ray.GetPoint(t1);
                if (hitPoint.y <= -halfHeight + 1e-6) {
                    if (t1 < closestT || closestT < 0) {
                        closestT = t1;
                    }
                }
            }
            if (t2 > 0) {
                Vector3 hitPoint = ray.GetPoint(t2);
                if (hitPoint.y <= -halfHeight + 1e-6) {
                    if (t2 < closestT || closestT < 0) {
                        closestT = t2;
                    }
                }
            }
        }
    }
    
    if (closestT > 0) {
        result.mPoint = ray.GetPoint(closestT);
        
        // 计算命中法线
        Vector3 closestPointOnAxis;
        if (result.mPoint.y <= -halfHeight + 1e-6) {
            closestPointOnAxis = Vector3(0, -halfHeight, 0);
        } else if (result.mPoint.y >= halfHeight - 1e-6) {
            closestPointOnAxis = Vector3(0, halfHeight, 0);
        } else {
            closestPointOnAxis = Vector3(0, result.mPoint.y, 0);
        }
        
        result.mNormal = (result.mPoint - closestPointOnAxis).Normalize();
        result.mHit = closestT;
        return true;
    }
    
    return false;
}

} // namespace SympConv