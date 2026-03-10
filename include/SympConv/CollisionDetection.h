#ifndef SYMPCONV_COLLISION_DETECTION_H
#define SYMPCONV_COLLISION_DETECTION_H

#include "Vector.h"
#include "Ray.h"
#include "AABB.h"
#include "Plane.h"
#include "Box.h"
#include "Sphere.h"
#include "Capsule.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Segment.h"
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
    
    // 检查射线与线段是否平行
    T crossLengthSquared = cross.LengthSquared();
    if (crossLengthSquared > T(1e-6)) {
        // 不平行，计算参数
        T a = dir.Dot(dir);
        if (a < T(1e-6)) {
            // 线段退化为点
            TVector3<T> v = start - ray.mOrigin;
            t = v.Dot(ray.mDirection);
            if (t < 0) {
                return false;
            }
            TVector3<T> point = ray.mOrigin + ray.mDirection * t;
            if ((point - start).LengthSquared() < T(1e-6)) {
                s = 0;
                return true;
            }
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
    } else {
        // 平行，检查是否共线
        TVector3<T> cross2 = q.Cross(ray.mDirection);
        if (cross2.LengthSquared() > T(1e-6)) {
            // 不共线，不相交
            return false;
        }
        
        // 共线，计算线段参数范围
        T t0 = q.Dot(ray.mDirection);
        T t1 = t0 + dir.Dot(ray.mDirection);
        
        // 检查射线方向与线段方向是否相同
        T dotDir = ray.mDirection.Dot(dir);
        if (dotDir > 0) {
            // 同方向，射线从线段起点出发，不会与线段相交
            if (t0 < 0 && t1 > 0) {
                // 射线原点在线段内部
                t = 0;
                s = -t0 / (t1 - t0);
                return true;
            } else if (t0 >= 0) {
                // 射线原点在线段前方，不相交
                return false;
            } else {
                // 射线原点在线段后方，相交
                t = -t0;
                s = 0;
                return true;
            }
        } else if (dotDir < 0) {
            // 反方向，射线向线段方向延伸
            if (t0 > 0 && t1 < 0) {
                // 射线原点在线段内部
                t = 0;
                s = t0 / (t0 - t1);
                return true;
            } else if (t0 <= 0) {
                // 射线原点在线段后方，不相交
                return false;
            } else {
                // 射线原点在线段前方，相交
                t = t0;
                s = 0;
                return true;
            }
        } else {
            // 方向为零，检查射线原点是否在线段上
            TVector3<T> originToStart = ray.mOrigin - start;
            TVector3<T> originToEnd = ray.mOrigin - end;
            T dotStart = originToStart.Dot(dir);
            T dotEnd = originToEnd.Dot(dir);
            
            if (dotStart * dotEnd <= 0) {
                // 原点在线段上
                t = 0;
                s = (dotStart < 0) ? 0 : 1;
                return true;
            }
            return false;
        }
    }
}

/**
 * @brief 计算射线与线段的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param segment 线段
 * @param t 相交参数
 * @param s 线段参数 [0, 1]
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsSegment(const TRay<T>& ray, const TSegment<T>& segment, T& t, T& s)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    return RayIntersectsSegment(ray, segment.mStart, segment.mEnd, t, s);
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
 * @brief 计算射线与线段的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param segment 线段
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsSegment(const TRay<T>& ray, const TSegment<T>& segment)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t, s;
    return RayIntersectsSegment(ray, segment, t, s);
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

/**
 * @brief 计算射线与盒子的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param box 盒子
 * @param t 相交参数
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsBox(const TRay<T>& ray, const TBox<T>& box, T& t)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 首先检查射线是否与盒子的AABB相交
    TAABB<T> aabb = box.GetAABB();
    T tMin, tMax;
    if (!RayIntersectsAABB(ray, aabb, tMin, tMax)) {
        return false;
    }
    
    // 获取盒子的顶点
    TVector3<T> vertices[8];
    box.GetVertices(vertices);
    
    // 定义盒子的6个面
    TPlane<T> planes[6] = {
        // 前面
        TPlane<T>(vertices[0], vertices[1], vertices[3]),
        // 后面
        TPlane<T>(vertices[4], vertices[6], vertices[7]),
        // 左面
        TPlane<T>(vertices[0], vertices[2], vertices[4]),
        // 右面
        TPlane<T>(vertices[1], vertices[7], vertices[5]),
        // 下面
        TPlane<T>(vertices[0], vertices[4], vertices[5]),
        // 上面
        TPlane<T>(vertices[2], vertices[3], vertices[7])
    };
    
    // 检查射线是否与任何一个面相交
    T minT = std::numeric_limits<T>::max();
    bool intersect = false;
    
    for (int i = 0; i < 6; ++i) {
        T tPlane;
        if (RayIntersectsPlane(ray, planes[i], tPlane)) {
            // 检查交点是否在盒子内
            TVector3<T> point = ray.mOrigin + ray.mDirection * tPlane;
            if (box.Contains(point)) {
                if (tPlane < minT) {
                    minT = tPlane;
                    intersect = true;
                }
            }
        }
    }
    
    if (intersect) {
        t = minT;
    }
    
    return intersect;
}

/**
 * @brief 计算射线与盒子的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param box 盒子
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsBox(const TRay<T>& ray, const TBox<T>& box)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t;
    return RayIntersectsBox(ray, box, t);
}

/**
 * @brief 计算盒子与盒子的相交
 * @tparam T 浮点类型，如float、double
 * @param box1 第一个盒子
 * @param box2 第二个盒子
 * @return 是否相交
 */
template<typename T>
bool BoxIntersectsBox(const TBox<T>& box1, const TBox<T>& box2)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 首先检查AABB是否相交
    TAABB<T> aabb1 = box1.GetAABB();
    TAABB<T> aabb2 = box2.GetAABB();
    if (!AABBIntersectsAABB(aabb1, aabb2)) {
        return false;
    }
    
    // 获取盒子的顶点
    TVector3<T> vertices1[8];
    TVector3<T> vertices2[8];
    box1.GetVertices(vertices1);
    box2.GetVertices(vertices2);
    
    // 检查box1的任何顶点是否在box2内
    for (int i = 0; i < 8; ++i) {
        if (box2.Contains(vertices1[i])) {
            return true;
        }
    }
    
    // 检查box2的任何顶点是否在box1内
    for (int i = 0; i < 8; ++i) {
        if (box1.Contains(vertices2[i])) {
            return true;
        }
    }
    
    // 这里可以添加更复杂的碰撞检测算法，如SAT（分离轴定理）
    // 为了简单起见，我们只检查顶点是否在对方盒子内
    
    return false;
}

/**
 * @brief 计算球体与盒子的相交
 * @tparam T 浮点类型，如float、double
 * @param sphere 球体
 * @param box 盒子
 * @return 是否相交
 */
template<typename T>
bool SphereIntersectsBox(const TSphere<T>& sphere, const TBox<T>& box)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 首先检查球体的AABB是否与盒子的AABB相交
    TAABB<T> sphereAABB = sphere.GetAABB();
    TAABB<T> boxAABB = box.GetAABB();
    if (!AABBIntersectsAABB(sphereAABB, boxAABB)) {
        return false;
    }
    
    // 将球体中心转换到盒子的局部坐标系
    TVector3<T> localCenter = box.mTransform.InverseTransformPoint(sphere.mCenter - box.mCenter);
    
    // 计算局部坐标系中球体中心到盒子的最短距离
    TVector3<T> closest;
    closest.x = std::max(-box.mHalfExtents.x, std::min(localCenter.x, box.mHalfExtents.x));
    closest.y = std::max(-box.mHalfExtents.y, std::min(localCenter.y, box.mHalfExtents.y));
    closest.z = std::max(-box.mHalfExtents.z, std::min(localCenter.z, box.mHalfExtents.z));
    
    // 计算距离的平方
    T distanceSquared = (localCenter - closest).LengthSquared();
    
    // 检查距离是否小于等于球体半径的平方
    return distanceSquared <= sphere.mRadius * sphere.mRadius;
}

/**
 * @brief 计算球体与球体的相交
 * @tparam T 浮点类型，如float、double
 * @param sphere1 第一个球体
 * @param sphere2 第二个球体
 * @return 是否相交
 */
template<typename T>
bool SphereIntersectsSphere(const TSphere<T>& sphere1, const TSphere<T>& sphere2)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 计算两个球体中心之间的距离
    TVector3<T> delta = sphere1.mCenter - sphere2.mCenter;
    T distanceSquared = delta.LengthSquared();
    
    // 计算两个球体半径之和的平方
    T radiusSum = sphere1.mRadius + sphere2.mRadius;
    T radiusSumSquared = radiusSum * radiusSum;
    
    // 检查距离是否小于等于半径之和
    return distanceSquared <= radiusSumSquared;
}

/**
 * @brief 计算射线与胶囊体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param capsule 胶囊体
 * @param t 相交参数
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsCapsule(const TRay<T>& ray, const TCapsule<T>& capsule, T& t)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    TVector3<T> ab = capsule.mEnd - capsule.mStart;
    TVector3<T> ao = ray.mOrigin - capsule.mStart;
    TVector3<T> dir = ray.mDirection;
    
    T a = ab.Dot(ab);
    T b = ao.Dot(ab);
    T c = ao.Dot(ao) - capsule.mRadius * capsule.mRadius;
    T d = dir.Dot(ab);
    T e = dir.Dot(ao);
    T f = dir.Dot(dir);
    
    // 二次方程系数
    T A = a * f - d * d;
    T B = 2 * (a * e - b * d);
    T C = a * c - b * b;
    
    if (std::abs(A) < T(1e-6)) {
        // 射线与胶囊体轴线平行
        // 计算射线与两个端点球体的相交
        TSphere<T> sphere1(capsule.mStart, capsule.mRadius);
        TSphere<T> sphere2(capsule.mEnd, capsule.mRadius);
        
        T tSphere1_1, tSphere1_2, tSphere2_1, tSphere2_2;
        bool intersectSphere1 = RayIntersectsSphere(ray, sphere1.mCenter, sphere1.mRadius, tSphere1_1, tSphere1_2);
        bool intersectSphere2 = RayIntersectsSphere(ray, sphere2.mCenter, sphere2.mRadius, tSphere2_1, tSphere2_2);
        
        if (intersectSphere1 || intersectSphere2) {
            T minT = std::numeric_limits<T>::max();
            if (intersectSphere1) {
                minT = std::min(minT, tSphere1_1);
            }
            if (intersectSphere2) {
                minT = std::min(minT, tSphere2_1);
            }
            t = minT;
            return true;
        }
        return false;
    }
    
    T discriminant = B * B - 4 * A * C;
    if (discriminant < 0) {
        return false;
    }
    
    T sqrtDiscriminant = std::sqrt(discriminant);
    T t1 = (-B - sqrtDiscriminant) / (2 * A);
    T t2 = (-B + sqrtDiscriminant) / (2 * A);
    
    if (t1 > t2) {
        std::swap(t1, t2);
    }
    
    if (t2 < 0) {
        return false;
    }
    
    if (t1 < 0) {
        t1 = t2;
    }
    
    // 检查交点是否在胶囊体的线段上
    TVector3<T> point = ray.mOrigin + dir * t1;
    TVector3<T> ap = point - capsule.mStart;
    T tCapsule = ap.Dot(ab) / a;
    
    if (tCapsule >= 0 && tCapsule <= 1) {
        t = t1;
        return true;
    }
    
    // 检查是否与胶囊体的端点球体相交
    TSphere<T> sphere1(capsule.mStart, capsule.mRadius);
    TSphere<T> sphere2(capsule.mEnd, capsule.mRadius);
    
    T tSphere1_1, tSphere1_2, tSphere2_1, tSphere2_2;
    bool intersectSphere1 = RayIntersectsSphere(ray, sphere1.mCenter, sphere1.mRadius, tSphere1_1, tSphere1_2);
    bool intersectSphere2 = RayIntersectsSphere(ray, sphere2.mCenter, sphere2.mRadius, tSphere2_1, tSphere2_2);
    
    if (intersectSphere1 || intersectSphere2) {
        T minT = std::numeric_limits<T>::max();
        if (intersectSphere1) {
            minT = std::min(minT, tSphere1_1);
        }
        if (intersectSphere2) {
            minT = std::min(minT, tSphere2_1);
        }
        t = minT;
        return true;
    }
    
    return false;
}

/**
 * @brief 计算射线与胶囊体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param capsule 胶囊体
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsCapsule(const TRay<T>& ray, const TCapsule<T>& capsule)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t;
    return RayIntersectsCapsule(ray, capsule, t);
}

/**
 * @brief 计算平面与胶囊体的相交
 * @tparam T 浮点类型，如float、double
 * @param plane 平面
 * @param capsule 胶囊体
 * @return 是否相交
 */
template<typename T>
bool PlaneIntersectsCapsule(const TPlane<T>& plane, const TCapsule<T>& capsule)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 计算胶囊体两个端点到平面的距离
    T distanceStart = plane.DistanceTo(capsule.mStart);
    T distanceEnd = plane.DistanceTo(capsule.mEnd);
    
    // 如果两个端点都在平面的同一侧，且距离大于半径，则不相交
    if (distanceStart > capsule.mRadius && distanceEnd > capsule.mRadius) {
        return false;
    }
    if (distanceStart < -capsule.mRadius && distanceEnd < -capsule.mRadius) {
        return false;
    }
    
    // 否则相交
    return true;
}

/**
 * @brief 计算盒子与胶囊体的相交
 * @tparam T 浮点类型，如float、double
 * @param box 盒子
 * @param capsule 胶囊体
 * @return 是否相交
 */
template<typename T>
bool BoxIntersectsCapsule(const TBox<T>& box, const TCapsule<T>& capsule)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 首先检查AABB是否相交
    TAABB<T> boxAABB = box.GetAABB();
    TAABB<T> capsuleAABB = capsule.GetAABB();
    if (!AABBIntersectsAABB(boxAABB, capsuleAABB)) {
        return false;
    }
    
    // 获取盒子的顶点
    TVector3<T> vertices[8];
    box.GetVertices(vertices);
    
    // 检查胶囊体的两个端点是否在盒子内
    if (box.Contains(capsule.mStart) || box.Contains(capsule.mEnd)) {
        return true;
    }
    
    // 检查盒子的任何顶点是否在胶囊体内
    for (int i = 0; i < 8; ++i) {
        if (capsule.Contains(vertices[i])) {
            return true;
        }
    }
    
    // 检查胶囊体的轴线与盒子的面是否相交
    TRay<T> capsuleRay(capsule.mStart, capsule.mEnd - capsule.mStart);
    for (int i = 0; i < 8; i += 4) {
        // 前面
        TPlane<T> frontPlane(vertices[i], vertices[i+1], vertices[i+3]);
        if (PlaneIntersectsCapsule(frontPlane, capsule)) {
            return true;
        }
        // 左面
        TPlane<T> leftPlane(vertices[i], vertices[i+2], vertices[i+4]);
        if (PlaneIntersectsCapsule(leftPlane, capsule)) {
            return true;
        }
        // 下面
        TPlane<T> bottomPlane(vertices[i], vertices[i+4], vertices[i+5]);
        if (PlaneIntersectsCapsule(bottomPlane, capsule)) {
            return true;
        }
    }
    
    return false;
}

/**
 * @brief 计算球体与胶囊体的相交
 * @tparam T 浮点类型，如float、double
 * @param sphere 球体
 * @param capsule 胶囊体
 * @return 是否相交
 */
template<typename T>
bool SphereIntersectsCapsule(const TSphere<T>& sphere, const TCapsule<T>& capsule)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 计算球体中心到胶囊体线段的最短距离
    TVector3<T> ab = capsule.mEnd - capsule.mStart;
    TVector3<T> ao = sphere.mCenter - capsule.mStart;
    T t = ao.Dot(ab) / ab.Dot(ab);
    t = std::max(T(0), std::min(T(1), t));
    TVector3<T> closest = capsule.mStart + ab * t;
    
    // 计算距离的平方
    T distanceSquared = (sphere.mCenter - closest).LengthSquared();
    
    // 检查距离是否小于等于球体和胶囊体半径之和的平方
    T radiusSum = sphere.mRadius + capsule.mRadius;
    return distanceSquared <= radiusSum * radiusSum;
}

/**
 * @brief 计算胶囊体与胶囊体的相交
 * @tparam T 浮点类型，如float、double
 * @param capsule1 第一个胶囊体
 * @param capsule2 第二个胶囊体
 * @return 是否相交
 */
template<typename T>
bool CapsuleIntersectsCapsule(const TCapsule<T>& capsule1, const TCapsule<T>& capsule2)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 计算两个线段之间的最短距离
    TVector3<T> p1 = capsule1.mStart;
    TVector3<T> p2 = capsule1.mEnd;
    TVector3<T> q1 = capsule2.mStart;
    TVector3<T> q2 = capsule2.mEnd;
    
    TVector3<T> d1 = p2 - p1;
    TVector3<T> d2 = q2 - q1;
    TVector3<T> r = p1 - q1;
    
    T a = d1.Dot(d1);
    T e = d2.Dot(d2);
    T f = d2.Dot(r);
    
    T s, t;
    
    if (a <= T(1e-6) && e <= T(1e-6)) {
        // 两个胶囊体都是球体
        TVector3<T> delta = p1 - q1;
        T distanceSquared = delta.LengthSquared();
        T radiusSum = capsule1.mRadius + capsule2.mRadius;
        return distanceSquared <= radiusSum * radiusSum;
    }
    
    if (a <= T(1e-6)) {
        // 第一个胶囊体是球体
        s = 0;
        t = f / e;
        t = std::max(T(0), std::min(T(1), t));
    } else if (e <= T(1e-6)) {
        // 第二个胶囊体是球体
        t = 0;
        s = -(d1.Dot(r)) / a;
        s = std::max(T(0), std::min(T(1), s));
    } else {
        T c = d1.Dot(r);
        T b = d1.Dot(d2);
        T denom = a * e - b * b;
        
        if (denom != 0) {
            s = (b * f - c * e) / denom;
            s = std::max(T(0), std::min(T(1), s));
        } else {
            s = 0;
        }
        
        t = (b * s + f) / e;
        t = std::max(T(0), std::min(T(1), t));
        
        // 调整s以确保t在范围内
        T s_prev = s;
        s = (b * t - c) / a;
        s = std::max(T(0), std::min(T(1), s));
        
        if (s != s_prev) {
            t = (b * s + f) / e;
            t = std::max(T(0), std::min(T(1), t));
        }
    }
    
    TVector3<T> closest1 = p1 + d1 * s;
    TVector3<T> closest2 = q1 + d2 * t;
    T distanceSquared = (closest1 - closest2).LengthSquared();
    T radiusSum = capsule1.mRadius + capsule2.mRadius;
    
    return distanceSquared <= radiusSum * radiusSum;
}

/**
 * @brief 计算射线与圆柱体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param cylinder 圆柱体
 * @param t 相交参数
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsCylinder(const TRay<T>& ray, const TCylinder<T>& cylinder, T& t)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    TVector3<T> ab = cylinder.mEnd - cylinder.mStart;
    TVector3<T> ao = ray.mOrigin - cylinder.mStart;
    TVector3<T> dir = ray.mDirection;
    
    T a = ab.Dot(ab);
    T b = ao.Dot(ab);
    T c = ao.Dot(ao) - cylinder.mRadius * cylinder.mRadius;
    T d = dir.Dot(ab);
    T e = dir.Dot(ao);
    T f = dir.Dot(dir);
    
    // 二次方程系数
    T A = a * f - d * d;
    T B = 2 * (a * e - b * d);
    T C = a * c - b * b;
    
    if (std::abs(A) < T(1e-6)) {
        // 射线与圆柱体轴线平行
        // 计算射线到轴线的距离
        TVector3<T> cross = ao.Cross(ab);
        T distance = cross.Length() / ab.Length();
        
        // 如果距离大于半径，不相交
        if (distance > cylinder.mRadius) {
            return false;
        }
        
        // 检查射线是否与圆柱体的端点球体相交
        TVector3<T> center1 = cylinder.mStart;
        TVector3<T> center2 = cylinder.mEnd;
        T radius = cylinder.mRadius;
        
        T tSphere1_1, tSphere1_2, tSphere2_1, tSphere2_2;
        bool intersectSphere1 = RayIntersectsSphere(ray, center1, radius, tSphere1_1, tSphere1_2);
        bool intersectSphere2 = RayIntersectsSphere(ray, center2, radius, tSphere2_1, tSphere2_2);
        
        if (intersectSphere1 || intersectSphere2) {
            T minT = std::numeric_limits<T>::max();
            if (intersectSphere1 && tSphere1_1 >= 0) {
                minT = std::min(minT, tSphere1_1);
            }
            if (intersectSphere2 && tSphere2_1 >= 0) {
                minT = std::min(minT, tSphere2_1);
            }
            if (minT != std::numeric_limits<T>::max()) {
                t = minT;
                return true;
            }
        }
        
        // 检查射线是否在圆柱体内
        TVector3<T> point = ray.mOrigin;
        TVector3<T> ap = point - cylinder.mStart;
        T tCylinder = ap.Dot(ab) / a;
        
        if (tCylinder >= 0 && tCylinder <= 1) {
            t = 0;
            return true;
        }
        
        return false;
    }
    
    T discriminant = B * B - 4 * A * C;
    if (discriminant < 0) {
        return false;
    }
    
    T sqrtDiscriminant = std::sqrt(discriminant);
    T t1 = (-B - sqrtDiscriminant) / (2 * A);
    T t2 = (-B + sqrtDiscriminant) / (2 * A);
    
    if (t1 > t2) {
        std::swap(t1, t2);
    }
    
    if (t2 < 0) {
        return false;
    }
    
    // 找到最小的非负t值
    T tMin = t1;
    if (tMin < 0) {
        tMin = t2;
    }
    
    // 检查交点是否在圆柱体的线段上
    TVector3<T> point = ray.mOrigin + dir * tMin;
    TVector3<T> ap = point - cylinder.mStart;
    T tCylinder = ap.Dot(ab) / a;
    
    if (tCylinder >= 0 && tCylinder <= 1) {
        t = tMin;
        return true;
    }
    
    // 检查是否与圆柱体的端点相交
    // 计算射线与两个端点球体的相交
    TVector3<T> center1 = cylinder.mStart;
    TVector3<T> center2 = cylinder.mEnd;
    T radius = cylinder.mRadius;
    
    T tSphere1_1, tSphere1_2, tSphere2_1, tSphere2_2;
    bool intersectSphere1 = RayIntersectsSphere(ray, center1, radius, tSphere1_1, tSphere1_2);
    bool intersectSphere2 = RayIntersectsSphere(ray, center2, radius, tSphere2_1, tSphere2_2);
    
    if (intersectSphere1 || intersectSphere2) {
        T minT = std::numeric_limits<T>::max();
        if (intersectSphere1 && tSphere1_1 >= 0) {
            minT = std::min(minT, tSphere1_1);
        }
        if (intersectSphere2 && tSphere2_1 >= 0) {
            minT = std::min(minT, tSphere2_1);
        }
        if (minT != std::numeric_limits<T>::max()) {
            t = minT;
            return true;
        }
    }
    
    // 特殊处理沿着圆柱体轴线方向的射线
    // 检查射线是否与圆柱体的端点相交
    if (std::abs(dir.Dot(ab.Normalize())) > 0.999f) {
        // 射线与圆柱体轴线几乎重合
        // 检查射线原点是否在圆柱体内
        TVector3<T> point = ray.mOrigin;
        TVector3<T> ap = point - cylinder.mStart;
        T tCylinder = ap.Dot(ab) / a;
        
        if (tCylinder >= 0 && tCylinder <= 1) {
            t = 0;
            return true;
        }
        
        // 计算射线与圆柱体端点的距离
        T distanceToStart = (ray.mOrigin - cylinder.mStart).Length();
        T distanceToEnd = (ray.mOrigin - cylinder.mEnd).Length();
        
        if (distanceToStart <= cylinder.mRadius) {
            // 射线原点在起始端点球体内
            t = 0;
            return true;
        }
        
        if (distanceToEnd <= cylinder.mRadius) {
            // 射线原点在结束端点球体内
            t = 0;
            return true;
        }
        
        // 计算射线是否会进入圆柱体
        TVector3<T> dirNormalized = dir.Normalize();
        TVector3<T> abNormalized = ab.Normalize();
        
        if (dirNormalized.Dot(abNormalized) > 0) {
            // 射线沿着圆柱体轴线正方向
            // 计算射线到起始端点的距离
            T tStart = (cylinder.mStart - ray.mOrigin).Dot(dirNormalized);
            if (tStart >= 0) {
                t = tStart;
                return true;
            }
        } else {
            // 射线沿着圆柱体轴线负方向
            // 计算射线到结束端点的距离
            T tEnd = (cylinder.mEnd - ray.mOrigin).Dot(dirNormalized);
            if (tEnd >= 0) {
                t = tEnd;
                return true;
            }
        }
    }
    
    return false;
}

/**
 * @brief 计算射线与圆柱体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param cylinder 圆柱体
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsCylinder(const TRay<T>& ray, const TCylinder<T>& cylinder)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t;
    return RayIntersectsCylinder(ray, cylinder, t);
}

/**
 * @brief 计算平面与圆柱体的相交
 * @tparam T 浮点类型，如float、double
 * @param plane 平面
 * @param cylinder 圆柱体
 * @return 是否相交
 */
template<typename T>
bool PlaneIntersectsCylinder(const TPlane<T>& plane, const TCylinder<T>& cylinder)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 计算圆柱体两个端点到平面的距离
    T distanceStart = plane.DistanceTo(cylinder.mStart);
    T distanceEnd = plane.DistanceTo(cylinder.mEnd);
    
    // 如果两个端点都在平面的同一侧，且距离大于半径，则不相交
    if (distanceStart > cylinder.mRadius && distanceEnd > cylinder.mRadius) {
        return false;
    }
    if (distanceStart < -cylinder.mRadius && distanceEnd < -cylinder.mRadius) {
        return false;
    }
    
    // 否则相交
    return true;
}

/**
 * @brief 计算球体与圆柱体的相交
 * @tparam T 浮点类型，如float、double
 * @param sphere 球体
 * @param cylinder 圆柱体
 * @return 是否相交
 */
template<typename T>
bool SphereIntersectsCylinder(const TSphere<T>& sphere, const TCylinder<T>& cylinder)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 计算球体中心到圆柱体线段的最短距离
    TVector3<T> ab = cylinder.mEnd - cylinder.mStart;
    TVector3<T> ao = sphere.mCenter - cylinder.mStart;
    T t = ao.Dot(ab) / ab.Dot(ab);
    t = std::max(T(0), std::min(T(1), t));
    TVector3<T> closest = cylinder.mStart + ab * t;
    
    // 计算距离的平方
    T distanceSquared = (sphere.mCenter - closest).LengthSquared();
    
    // 检查距离是否小于等于球体和圆柱体半径之和的平方
    T radiusSum = sphere.mRadius + cylinder.mRadius;
    return distanceSquared <= radiusSum * radiusSum;
}

/**
 * @brief 计算圆柱体与圆柱体的相交
 * @tparam T 浮点类型，如float、double
 * @param cylinder1 第一个圆柱体
 * @param cylinder2 第二个圆柱体
 * @return 是否相交
 */
template<typename T>
bool CylinderIntersectsCylinder(const TCylinder<T>& cylinder1, const TCylinder<T>& cylinder2)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 计算两个线段之间的最短距离
    TVector3<T> p1 = cylinder1.mStart;
    TVector3<T> p2 = cylinder1.mEnd;
    TVector3<T> q1 = cylinder2.mStart;
    TVector3<T> q2 = cylinder2.mEnd;
    
    TVector3<T> d1 = p2 - p1;
    TVector3<T> d2 = q2 - q1;
    TVector3<T> r = p1 - q1;
    
    T a = d1.Dot(d1);
    T e = d2.Dot(d2);
    T f = d2.Dot(r);
    
    T s, t;
    
    if (a <= T(1e-6) && e <= T(1e-6)) {
        // 两个圆柱体都是圆形
        TVector3<T> delta = p1 - q1;
        T distanceSquared = delta.LengthSquared();
        T radiusSum = cylinder1.mRadius + cylinder2.mRadius;
        return distanceSquared <= radiusSum * radiusSum;
    }
    
    if (a <= T(1e-6)) {
        // 第一个圆柱体是圆形
        s = 0;
        t = f / e;
        t = std::max(T(0), std::min(T(1), t));
    } else if (e <= T(1e-6)) {
        // 第二个圆柱体是圆形
        t = 0;
        s = -(d1.Dot(r)) / a;
        s = std::max(T(0), std::min(T(1), s));
    } else {
        T c = d1.Dot(r);
        T b = d1.Dot(d2);
        T denom = a * e - b * b;
        
        if (denom != 0) {
            s = (b * f - c * e) / denom;
            s = std::max(T(0), std::min(T(1), s));
        } else {
            s = 0;
        }
        
        t = (b * s + f) / e;
        t = std::max(T(0), std::min(T(1), t));
        
        // 调整s以确保t在范围内
        T s_prev = s;
        s = (b * t - c) / a;
        s = std::max(T(0), std::min(T(1), s));
        
        if (s != s_prev) {
            t = (b * s + f) / e;
            t = std::max(T(0), std::min(T(1), t));
        }
    }
    
    TVector3<T> closest1 = p1 + d1 * s;
    TVector3<T> closest2 = q1 + d2 * t;
    T distanceSquared = (closest1 - closest2).LengthSquared();
    T radiusSum = cylinder1.mRadius + cylinder2.mRadius;
    
    return distanceSquared <= radiusSum * radiusSum;
}

/**
 * @brief 计算射线与圆锥体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param cone 圆锥体
 * @param t 相交参数
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsCone(const TRay<T>& ray, const TCone<T>& cone, T& t)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    TVector3<T> vertex = cone.mVertex;
    TVector3<T> baseCenter = cone.mBaseCenter;
    T radius = cone.mRadius;
    
    // 计算圆锥体的轴线向量和高度
    TVector3<T> axis = baseCenter - vertex;
    T height = axis.Length();
    if (height < T(1e-6)) {
        return false;
    }
    
    // 归一化轴线向量
    TVector3<T> axisNormalized = axis / height;
    
    // 计算半顶角的余弦值
    T cosTheta = radius / std::sqrt(radius * radius + height * height);
    T sinTheta = height / std::sqrt(radius * radius + height * height);
    
    TVector3<T> ro = ray.mOrigin - vertex;
    TVector3<T> rd = ray.mDirection;
    
    // 计算圆锥体方程的系数
    T a = rd.Dot(rd) - std::pow(rd.Dot(axisNormalized), 2) * (1 - std::pow(cosTheta, 2)) - std::pow(rd.Dot(axisNormalized), 2) * std::pow(cosTheta, 2);
    T b = 2 * (ro.Dot(rd) - (ro.Dot(axisNormalized) * rd.Dot(axisNormalized) * (1 - std::pow(cosTheta, 2)) + ro.Dot(axisNormalized) * rd.Dot(axisNormalized) * std::pow(cosTheta, 2)));
    T c = ro.Dot(ro) - std::pow(ro.Dot(axisNormalized), 2) * (1 - std::pow(cosTheta, 2)) - std::pow(ro.Dot(axisNormalized), 2) * std::pow(cosTheta, 2);
    
    // 求解二次方程
    T discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    }
    
    T sqrtDiscriminant = std::sqrt(discriminant);
    T t1 = (-b - sqrtDiscriminant) / (2 * a);
    T t2 = (-b + sqrtDiscriminant) / (2 * a);
    
    // 确保t1 <= t2
    if (t1 > t2) {
        std::swap(t1, t2);
    }
    
    // 检查是否有交点在射线方向上
    if (t2 < 0) {
        return false;
    }
    
    // 找到最小的非负t值
    T tMin = t1;
    if (tMin < 0) {
        tMin = t2;
    }
    
    // 检查交点是否在圆锥体内
    TVector3<T> point = ray.mOrigin + rd * tMin;
    TVector3<T> pointToVertex = point - vertex;
    T tAxis = pointToVertex.Dot(axisNormalized);
    
    if (tAxis < 0 || tAxis > height) {
        return false;
    }
    
    // 检查交点是否在圆锥体的侧面内
    T distanceToAxis = (pointToVertex - axisNormalized * tAxis).Length();
    T radiusAtHeight = (radius / height) * (height - tAxis);
    
    if (distanceToAxis > radiusAtHeight) {
        return false;
    }
    
    t = tMin;
    return true;
}

/**
 * @brief 计算射线与圆锥体的相交
 * @tparam T 浮点类型，如float、double
 * @param ray 射线
 * @param cone 圆锥体
 * @return 是否相交
 */
template<typename T>
bool RayIntersectsCone(const TRay<T>& ray, const TCone<T>& cone)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t;
    return RayIntersectsCone(ray, cone, t);
}

/**
 * @brief 计算线段与圆锥体的相交
 * @tparam T 浮点类型，如float、double
 * @param segment 线段
 * @param cone 圆锥体
 * @param t 相交参数
 * @return 是否相交
 */
template<typename T>
bool SegmentIntersectsCone(const TSegment<T>& segment, const TCone<T>& cone, T& t)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    // 将线段转换为射线
    TRay<T> ray(segment.mStart, segment.mEnd - segment.mStart);
    T segmentLength = (segment.mEnd - segment.mStart).Length();
    
    if (!RayIntersectsCone(ray, cone, t)) {
        return false;
    }
    
    // 检查交点是否在线段范围内
    return t >= 0 && t <= segmentLength;
}

/**
 * @brief 计算线段与圆锥体的相交
 * @tparam T 浮点类型，如float、double
 * @param segment 线段
 * @param cone 圆锥体
 * @return 是否相交
 */
template<typename T>
bool SegmentIntersectsCone(const TSegment<T>& segment, const TCone<T>& cone)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t;
    return SegmentIntersectsCone(segment, cone, t);
}

/**
 * @brief 计算线段与圆锥体的相交
 * @tparam T 浮点类型，如float、double
 * @param start 线段起点
 * @param end 线段终点
 * @param cone 圆锥体
 * @param t 相交参数
 * @return 是否相交
 */
template<typename T>
bool SegmentIntersectsCone(const TVector3<T>& start, const TVector3<T>& end, const TCone<T>& cone, T& t)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    TSegment<T> segment(start, end);
    return SegmentIntersectsCone(segment, cone, t);
}

/**
 * @brief 计算线段与圆锥体的相交
 * @tparam T 浮点类型，如float、double
 * @param start 线段起点
 * @param end 线段终点
 * @param cone 圆锥体
 * @return 是否相交
 */
template<typename T>
bool SegmentIntersectsCone(const TVector3<T>& start, const TVector3<T>& end, const TCone<T>& cone)
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");
    
    T t;
    return SegmentIntersectsCone(start, end, cone, t);
}

} // namespace SympConv

#endif // SYMPCONV_COLLISION_DETECTION_H
