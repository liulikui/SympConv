#include "SympConv/BoxShape.h"
#include "SympConv/CollisionDetection.h"

namespace SympConv {

bool BoxShape::RayCast(const Ray& ray, RayCastResult& result) const
{
    // 盒子的半边长
    Real halfExtentsX = mBox.mHalfExtents.x;
    Real halfExtentsY = mBox.mHalfExtents.y;
    Real halfExtentsZ = mBox.mHalfExtents.z;
    
    // 计算射线与AABB的交点
    Real tMin = 0.0f;
    Real tMax = ray.mMax;
    
    // 检查X轴
    if (std::abs(ray.mDirection.x) < 1e-10) {
        // 射线平行于X轴
        if (ray.mOrigin.x < -halfExtentsX || ray.mOrigin.x > halfExtentsX) {
            return false;
        }
    } else {
        Real t1 = (-halfExtentsX - ray.mOrigin.x) / ray.mDirection.x;
        Real t2 = (halfExtentsX - ray.mOrigin.x) / ray.mDirection.x;
        
        tMin = std::max(tMin, std::min(t1, t2));
        tMax = std::min(tMax, std::max(t1, t2));
    }
    
    // 检查Y轴
    if (std::abs(ray.mDirection.y) < 1e-10) {
        // 射线平行于Y轴
        if (ray.mOrigin.y < -halfExtentsY || ray.mOrigin.y > halfExtentsY) {
            return false;
        }
    } else {
        Real t1 = (-halfExtentsY - ray.mOrigin.y) / ray.mDirection.y;
        Real t2 = (halfExtentsY - ray.mOrigin.y) / ray.mDirection.y;
        
        tMin = std::max(tMin, std::min(t1, t2));
        tMax = std::min(tMax, std::max(t1, t2));
    }
    
    // 检查Z轴
    if (std::abs(ray.mDirection.z) < 1e-10) {
        // 射线平行于Z轴
        if (ray.mOrigin.z < -halfExtentsZ || ray.mOrigin.z > halfExtentsZ) {
            return false;
        }
    } else {
        Real t1 = (-halfExtentsZ - ray.mOrigin.z) / ray.mDirection.z;
        Real t2 = (halfExtentsZ - ray.mOrigin.z) / ray.mDirection.z;
        
        tMin = std::max(tMin, std::min(t1, t2));
        tMax = std::min(tMax, std::max(t1, t2));
    }
    
    // 检查是否有交点
    if (tMin <= tMax && tMax >= 0) {
        // 计算命中点
        result.mPoint = ray.GetPoint(tMin);
        
        // 计算命中法线
        // 确定命中的面
        if (std::abs(result.mPoint.x - halfExtentsX) < 1e-10) {
            result.mNormal = Vector3(1, 0, 0);
        } else if (std::abs(result.mPoint.x + halfExtentsX) < 1e-10) {
            result.mNormal = Vector3(-1, 0, 0);
        } else if (std::abs(result.mPoint.y - halfExtentsY) < 1e-10) {
            result.mNormal = Vector3(0, 1, 0);
        } else if (std::abs(result.mPoint.y + halfExtentsY) < 1e-10) {
            result.mNormal = Vector3(0, -1, 0);
        } else if (std::abs(result.mPoint.z - halfExtentsZ) < 1e-10) {
            result.mNormal = Vector3(0, 0, 1);
        } else if (std::abs(result.mPoint.z + halfExtentsZ) < 1e-10) {
            result.mNormal = Vector3(0, 0, -1);
        }
        
        result.mHit = tMin;
        return true;
    }
    
    return false;
}

} // namespace SympConv