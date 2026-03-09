#ifndef SYMPCONV_COLLISION_DETECTION_H
#define SYMPCONV_COLLISION_DETECTION_H

#include "Vector.h"
#include "Ray.h"
#include "AABB.h"
#include "Plane.h"
#include <type_traits>
#include <limits>

namespace SympConv {

/**
 * @brief 计算射线与AABB的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param aabb 轴对齐包围盒
 * @param tMin 最小相交参数
 * @param tMax 最大相交参数
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsAABB(const TRay<T>& ray, const TAABB<T>& aabb, T& tMin, T& tMax)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 处理方向向量分量为0的情况
    TVector3<T> t0, t1;
    
    // x方向
    if (std::abs(ray.mDirection.x) < T(1e-6)) {
        // 射线在x方向上没有分量，检查射线的x坐标是否在AABB的x范围内
        if (ray.mOrigin.x < aabb.mMin.x || ray.mOrigin.x > aabb.mMax.x) {
            return false;
        }
        t0.x = -std::numeric_limits<T>::max();
        t1.x = std::numeric_limits<T>::max();
    } else {
        T invDirX = T(1) / ray.mDirection.x;
        t0.x = (aabb.mMin.x - ray.mOrigin.x) * invDirX;
        t1.x = (aabb.mMax.x - ray.mOrigin.x) * invDirX;
    }
    
    // y方向
    if (std::abs(ray.mDirection.y) < T(1e-6)) {
        // 射线在y方向上没有分量，检查射线的y坐标是否在AABB的y范围内
        if (ray.mOrigin.y < aabb.mMin.y || ray.mOrigin.y > aabb.mMax.y) {
            return false;
        }
        t0.y = -std::numeric_limits<T>::max();
        t1.y = std::numeric_limits<T>::max();
    } else {
        T invDirY = T(1) / ray.mDirection.y;
        t0.y = (aabb.mMin.y - ray.mOrigin.y) * invDirY;
        t1.y = (aabb.mMax.y - ray.mOrigin.y) * invDirY;
    }
    
    // z方向
    if (std::abs(ray.mDirection.z) < T(1e-6)) {
        // 射线在z方向上没有分量，检查射线的z坐标是否在AABB的z范围内
        if (ray.mOrigin.z < aabb.mMin.z || ray.mOrigin.z > aabb.mMax.z) {
            return false;
        }
        t0.z = -std::numeric_limits<T>::max();
        t1.z = std::numeric_limits<T>::max();
    } else {
        T invDirZ = T(1) / ray.mDirection.z;
        t0.z = (aabb.mMin.z - ray.mOrigin.z) * invDirZ;
        t1.z = (aabb.mMax.z - ray.mOrigin.z) * invDirZ;
    }

    TVector3<T> tSmaller = TVector3<T>(
        std::min(t0.x, t1.x),
        std::min(t0.y, t1.y),
        std::min(t0.z, t1.z)
    );

    TVector3<T> tLarger = TVector3<T>(
        std::max(t0.x, t1.x),
        std::max(t0.y, t1.y),
        std::max(t0.z, t1.z)
    );

    tMin = std::max(T(0), std::max(tSmaller.x, std::max(tSmaller.y, tSmaller.z)));
    tMax = std::min(tLarger.x, std::min(tLarger.y, tLarger.z));

    return tMin <= tMax && tMax >= T(0);
}

/**
 * @brief 计算射线与AABB的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param aabb 轴对齐包围盒
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsAABB(const TRay<T>& ray, const TAABB<T>& aabb)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T tMin, tMax;
    return RayIntersectsAABB(ray, aabb, tMin, tMax);
}

/**
 * @brief 计算射线与平面的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param plane 平面
 * @param t 相交参数
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsPlane(const TRay<T>& ray, const TPlane<T>& plane, T& t)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    TVector3<T> normal = plane.GetNormal();
    T denom = normal.Dot(ray.mDirection);
    if (std::abs(denom) < T(1e-6)) {
        // 射线与平面平行
        return false;
    }

    T distance = plane.DistanceTo(ray.mOrigin);
    t = -distance / denom;

    return t >= T(0);
}

/**
 * @brief 计算射线与平面的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param plane 平面
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsPlane(const TRay<T>& ray, const TPlane<T>& plane)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t;
    return RayIntersectsPlane(ray, plane, t);
}

/**
 * @brief 计算射线与球体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param center 球体中心
 * @param radius 球体半径
 * @param t1 第一个相交参数
 * @param t2 第二个相交参数
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsSphere(const TRay<T>& ray, const TVector3<T>& center, T radius, T& t1, T& t2)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    TVector3<T> oc = ray.mOrigin - center;
    T a = ray.mDirection.Dot(ray.mDirection);
    T b = 2.0f * oc.Dot(ray.mDirection);
    T c = oc.Dot(oc) - radius * radius;
    T discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }

    T sqrtDiscriminant = std::sqrt(discriminant);
    t1 = (-b - sqrtDiscriminant) / (2 * a);
    t2 = (-b + sqrtDiscriminant) / (2 * a);

    // 确保t1 <= t2
    if (t1 > t2) {
        std::swap(t1, t2);
    }

    // 检查是否有交点在射线方向上
    if (t2 < 0) {
        return false;
    }

    // 如果t1 < 0，使用t2
    if (t1 < 0) {
        t1 = t2;
    }

    return true;
}

/**
 * @brief 计算射线与球体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param center 球体中心
 * @param radius 球体半径
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsSphere(const TRay<T>& ray, const TVector3<T>& center, T radius)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t1, t2;
    return RayIntersectsSphere(ray, center, radius, t1, t2);
}

/**
 * @brief 计算射线与三角形的相交（Möller-Trumbore算法）
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param v0 三角形第一个顶点
 * @param v1 三角形第二个顶点
 * @param v2 三角形第三个顶点
 * @param t 相交参数
 * @param u 重心坐标u
 * @param v 重心坐标v
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsTriangle(const TRay<T>& ray, const TVector3<T>& v0, const TVector3<T>& v1, const TVector3<T>& v2, T& t, T& u, T& v)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    const T epsilon = T(1e-6);

    TVector3<T> edge1 = v1 - v0;
    TVector3<T> edge2 = v2 - v0;
    TVector3<T> h = ray.mDirection.Cross(edge2);
    T a = edge1.Dot(h);

    if (a > -epsilon && a < epsilon) {
        // 射线与三角形平行
        return false;
    }

    T f = 1.0f / a;
    TVector3<T> s = ray.mOrigin - v0;
    u = f * s.Dot(h);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    TVector3<T> q = s.Cross(edge1);
    v = f * ray.mDirection.Dot(q);

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    t = f * edge2.Dot(q);

    if (t > epsilon) {
        return true;
    }

    return false;
}

/**
 * @brief 计算射线与三角形的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param v0 三角形第一个顶点
 * @param v1 三角形第二个顶点
 * @param v2 三角形第三个顶点
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsTriangle(const TRay<T>& ray, const TVector3<T>& v0, const TVector3<T>& v1, const TVector3<T>& v2)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t, u, v;
    return RayIntersectsTriangle(ray, v0, v1, v2, t, u, v);
}

/**
 * @brief 计算射线与线段的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param start 线段起点
 * @param end 线段终点
 * @param t 相交参数
 * @param s 线段参数 [0, 1]
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsSegment(const TRay<T>& ray, const TVector3<T>& start, const TVector3<T>& end, T& t, T& s)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    TVector3<T> dir = end - start;
    TVector3<T> q = ray.mOrigin - start;
    TVector3<T> cross = ray.mDirection.Cross(dir);
    T dot = q.Dot(cross);

    if (std::abs(dot) > T(1e-6)) {
        return false;
    }

    T a = dir.Dot(dir);
    if (a < T(1e-6)) {
        return false;
    }

    T b = dir.Dot(q);
    s = b / a;

    if (s < 0 || s > 1) {
        return false;
    }

    TVector3<T> closest = start + dir * s;
    TVector3<T> v = closest - ray.mOrigin;
    t = v.Dot(ray.mDirection);

    if (t < 0) {
        return false;
    }

    return true;
}

/**
 * @brief 计算射线与线段的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param start 线段起点
 * @param end 线段终点
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsSegment(const TRay<T>& ray, const TVector3<T>& start, const TVector3<T>& end)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t, s;
    return RayIntersectsSegment(ray, start, end, t, s);
}

/**
 * @brief 计算AABB与AABB的相交
 * @tparam T 浮点类型，如float、double
 * @param aabb1 第一个AABB
 * @param aabb2 第二个AABB
 * @return 是否相交
 */
template<typename T>
bool AABBIntersectsAABB(const TAABB<T>& aabb1, const TAABB<T>& aabb2)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    return aabb1.Intersects(aabb2);
}

/**
 * @brief 计算点是否在AABB内
 * @tparam T 浮点类型，如float、double
 * @param point 点
 * @param aabb AABB
 * @return 是否在AABB内
 */
template<typename T>
bool PointInAABB(const TVector3<T>& point, const TAABB<T>& aabb)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    return aabb.Contains(point);
}

/**
 * @brief 计算点是否在平面的正面
 * @tparam T 浮点类型，如float、double
 * @param point 点
 * @param plane 平面
 * @return 是否在平面的正面
 */
template<typename T>
bool PointOnPlanePositiveSide(const TVector3<T>& point, const TPlane<T>& plane)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    return plane.DistanceTo(point) > T(0);
}

/**
 * @brief 计算点是否在平面的负面
 * @tparam T 浮点类型，如float、double
 * @param point 点
 * @param plane 平面
 * @return 是否在平面的负面
 */
template<typename T>
bool PointOnPlaneNegativeSide(const TVector3<T>& point, const TPlane<T>& plane)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    return plane.DistanceTo(point) < T(0);
}

/**
 * @brief 计算点是否在平面上
 * @tparam T 浮点类型，如float、double
 * @param point 点
 * @param plane 平面
 * @param epsilon 容差
 * @return 是否在平面上
 */
template<typename T>
bool PointOnPlane(const TVector3<T>& point, const TPlane<T>& plane, T epsilon = T(1e-6))
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    return plane.Contains(point, epsilon);
}

} // namespace SympConv

#endif // SYMPCONV_COLLISION_DETECTION_H
