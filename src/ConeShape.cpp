#include "SympConv/ConeShape.h"
#include "SympConv/CollisionDetection.h"
#include <cmath>

namespace SympConv {

bool ConeShape::RayCast(const Ray& ray, RayCastResult& result) const
{
    // 圆锥体的底面在y=0平面，顶点在(0, height, 0)
    Real radius = mCone.mRadius;
    Real height = mCone.mHeight;
    
    Vector3 ro = ray.mOrigin;
    Vector3 rd = ray.mDirection;
    
    // 一般情况处理
    // 圆锥体的半顶角的正切值
    Real tanTheta = radius / height;
    Real invTanTheta = height / radius;
    
    // 计算射线与圆锥体的交点
    // 圆锥体的轴线是Y轴
    // 射线与圆锥体的方程：(x^2 + z^2) = (r/h)^2 * (h - y)^2
    // 展开并代入射线方程：r(t) = ro + t*rd
    
    // 计算二次方程的系数
    Real a = rd.x * rd.x + rd.z * rd.z - tanTheta * tanTheta * rd.y * rd.y;
    Real b = 2 * (ro.x * rd.x + ro.z * rd.z + tanTheta * tanTheta * height * rd.y - tanTheta * tanTheta * ro.y * rd.y);
    Real c = ro.x * ro.x + ro.z * ro.z - tanTheta * tanTheta * (height - ro.y) * (height - ro.y);
    
    Real t = 0;
    bool hit = false;
    
    if (a < 1e-10) {
        // 射线与圆锥体轴线平行
        if (std::sqrt(ro.x * ro.x + ro.z * ro.z) <= tanTheta * (height - ro.y)) {
            // 射线在圆锥体内，计算与底面的交点
            if (rd.y < 0) {
                t = -ro.y / rd.y;
                if (t > 0) {
                    hit = true;
                }
            }
        }
    } else {
        // 解二次方程
        Real discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return false;
        }
        
        Real sqrtDiscriminant = std::sqrt(discriminant);
        Real t1 = (-b - sqrtDiscriminant) / (2 * a);
        Real t2 = (-b + sqrtDiscriminant) / (2 * a);
        
        // 找到在射线方向上的最近交点
        if (t1 > 0 && t2 > 0) {
            t = std::min(t1, t2);
        } else if (t1 > 0) {
            t = t1;
        } else if (t2 > 0) {
            t = t2;
        } else {
            return false;
        }
        
        // 检查交点是否在圆锥体的高度范围内
        Vector3 hitPoint = ray.GetPoint(t);
        if (hitPoint.y >= 0 && hitPoint.y <= height) {
            hit = true;
        }
    }
    
    // 检查底面
    if (!hit) {
        // 检查底面
        Plane bottomPlane(Vector3(0, -1, 0), Vector3(0, 0, 0));
        Real t1;
        if (RayIntersectsPlane(ray, bottomPlane, t1)) {
            if (t1 > 0) {
                Vector3 hitPoint = ray.GetPoint(t1);
                if (std::sqrt(hitPoint.x * hitPoint.x + hitPoint.z * hitPoint.z) <= radius) {
                    t = t1;
                    hit = true;
                }
            }
        }
    }
    
    if (hit) {
        result.mPoint = ray.GetPoint(t);
        
        // 计算命中法线
        if (result.mPoint.y <= 1e-6) {
            // 命中底面
            result.mNormal = Vector3(0, -1, 0);
        } else {
            // 命中侧面
            // 计算圆锥体上的点到轴线的距离
            Real r = std::sqrt(result.mPoint.x * result.mPoint.x + result.mPoint.z * result.mPoint.z);
            // 计算圆锥体在该高度的半径
            Real coneRadiusAtY = tanTheta * (height - result.mPoint.y);
            // 计算法线
            result.mNormal = Vector3(result.mPoint.x, invTanTheta * coneRadiusAtY, result.mPoint.z).Normalize();
        }
        
        result.mHit = t;
        return true;
    }
    
    return false;
}

} // namespace SympConv