#ifndef SYMPCONV_RAY_H
#define SYMPCONV_RAY_H

#include "Vector.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 射线模板类
 * @details 表示三维空间中的射线，由原点和方向向量定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TRay
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mOrigin;      ///< 射线原点
    TVector3<T> mDirection;   ///< 射线方向（应归一化）

public:
    /**
     * @brief 默认构造函数
     * @details 初始化为原点在原点，方向沿Z轴正方向的射线
     */
    TRay() : 
        mOrigin(T(0), T(0), T(0)), 
        mDirection(T(0), T(0), T(1)) {}

    /**
     * @brief 带参数的构造函数
     * @param origin 射线原点
     * @param direction 射线方向
     */
    TRay(const TVector3<T>& origin, const TVector3<T>& direction) :
        mOrigin(origin), 
        mDirection(direction) {}

    /**
     * @brief 复制构造函数
     * @param other 另一个射线
     */
    TRay(const TRay<T>& other) = default;

    /**
     * @brief 移动构造函数
     * @param other 另一个射线
     */
    TRay(TRay<T>&& other) = default;

    /**
     * @brief 赋值运算符
     * @param other 另一个射线
     * @return 引用到当前射线
     */
    TRay<T>& operator=(const TRay<T>& other) = default;

    /**
     * @brief 移动赋值运算符
     * @param other 另一个射线
     * @return 引用到当前射线
     */
    TRay<T>& operator=(TRay<T>&& other) = default;

    /**
     * @brief 计算射线上的点
     * @param t 参数，t=0表示原点，t>0表示射线方向上的点
     * @return 射线上的点
     */
    TVector3<T> GetPoint(T t) const
    {
        return mOrigin + mDirection * t;
    }

    /**
     * @brief 归一化射线方向
     * @return 引用到当前射线
     */
    TRay<T>& Normalize()
    {
        mDirection.Normalize();
        return *this;
    }

    /**
     * @brief 获取归一化后的射线
     * @return 归一化后的射线
     */
    TRay<T> Normalized() const
    {
        TRay<T> result = *this;
        result.Normalize();
        return result;
    }

    /**
     * @brief 计算射线与AABB的相交
     * @param aabb 轴对齐包围盒
     * @param tMin 最小相交参数
     * @param tMax 最大相交参数
     * @return 是否相交
     */
    bool Intersects(const TAABB<T>& aabb, T& tMin, T& tMax) const
    {
        TVector3<T> invDir = TVector3<T>(T(1) / mDirection.x, T(1) / mDirection.y, T(1) / mDirection.z);
        TVector3<T> t0 = (aabb.mMin - mOrigin) * invDir;
        TVector3<T> t1 = (aabb.mMax - mOrigin) * invDir;

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
     * @param aabb 轴对齐包围盒
     * @return 是否相交
     */
    bool Intersects(const TAABB<T>& aabb) const
    {
        T tMin, tMax;
        return Intersects(aabb, tMin, tMax);
    }

    /**
     * @brief 计算射线与平面的相交
     * @param planeOrigin 平面原点
     * @param planeNormal 平面法线（应归一化）
     * @param t 相交参数
     * @return 是否相交
     */
    bool IntersectsPlane(const TVector3<T>& planeOrigin, const TVector3<T>& planeNormal, T& t) const
    {
        T denom = planeNormal.Dot(mDirection);
        if (std::abs(denom) < T(1e-6)) {
            // 射线与平面平行
            return false;
        }

        TVector3<T> delta = planeOrigin - mOrigin;
        t = delta.Dot(planeNormal) / denom;

        return t >= T(0);
    }

    /**
     * @brief 计算射线与球体的相交
     * @param center 球体中心
     * @param radius 球体半径
     * @param t1 第一个相交参数
     * @param t2 第二个相交参数
     * @return 是否相交
     */
    bool IntersectsSphere(const TVector3<T>& center, T radius, T& t1, T& t2) const
    {
        TVector3<T> oc = mOrigin - center;
        T a = mDirection.Dot(mDirection);
        T b = 2.0f * oc.Dot(mDirection);
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
     * @param center 球体中心
     * @param radius 球体半径
     * @return 是否相交
     */
    bool IntersectsSphere(const TVector3<T>& center, T radius) const
    {
        T t1, t2;
        return IntersectsSphere(center, radius, t1, t2);
    }

    /**
     * @brief 计算射线与三角形的相交（Möller-Trumbore算法）
     * @param v0 三角形第一个顶点
     * @param v1 三角形第二个顶点
     * @param v2 三角形第三个顶点
     * @param t 相交参数
     * @param u 重心坐标u
     * @param v 重心坐标v
     * @return 是否相交
     */
    bool IntersectsTriangle(const TVector3<T>& v0, const TVector3<T>& v1, const TVector3<T>& v2, T& t, T& u, T& v) const
    {
        const T epsilon = T(1e-6);

        TVector3<T> edge1 = v1 - v0;
        TVector3<T> edge2 = v2 - v0;
        TVector3<T> h = mDirection.Cross(edge2);
        T a = edge1.Dot(h);

        if (a > -epsilon && a < epsilon) {
            // 射线与三角形平行
            return false;
        }

        T f = 1.0f / a;
        TVector3<T> s = mOrigin - v0;
        u = f * s.Dot(h);

        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        TVector3<T> q = s.Cross(edge1);
        v = f * mDirection.Dot(q);

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
     * @param v0 三角形第一个顶点
     * @param v1 三角形第二个顶点
     * @param v2 三角形第三个顶点
     * @return 是否相交
     */
    bool IntersectsTriangle(const TVector3<T>& v0, const TVector3<T>& v1, const TVector3<T>& v2) const
    {
        T t, u, v;
        return IntersectsTriangle(v0, v1, v2, t, u, v);
    }

    /**
     * @brief 计算射线与线段的相交
     * @param start 线段起点
     * @param end 线段终点
     * @param t 相交参数
     * @param s 线段参数 [0, 1]
     * @return 是否相交
     */
    bool IntersectsSegment(const TVector3<T>& start, const TVector3<T>& end, T& t, T& s) const
    {
        TVector3<T> dir = end - start;
        TVector3<T> q = mOrigin - start;
        TVector3<T> cross = mDirection.Cross(dir);
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

        TVector3<T> c = mDirection.Dot(q);
        t = c / a;

        if (t < 0) {
            return false;
        }

        return true;
    }

    /**
     * @brief 计算射线与线段的相交
     * @param start 线段起点
     * @param end 线段终点
     * @return 是否相交
     */
    bool IntersectsSegment(const TVector3<T>& start, const TVector3<T>& end) const
    {
        T t, s;
        return IntersectsSegment(start, end, t, s);
    }

    /**
     * @brief 计算射线到点的最短距离
     * @param point 目标点
     * @return 最短距离
     */
    T DistanceTo(const TVector3<T>& point) const
    {
        TVector3<T> v = point - mOrigin;
        T t = v.Dot(mDirection);
        TVector3<T> closest;
        if (t < 0) {
            closest = mOrigin;
        } else {
            closest = GetPoint(t);
        }
        return (point - closest).Length();
    }

    /**
     * @brief 计算射线到线段的最短距离
     * @param start 线段起点
     * @param end 线段终点
     * @return 最短距离
     */
    T DistanceToSegment(const TVector3<T>& start, const TVector3<T>& end) const
    {
        TVector3<T> dir = end - start;
        TVector3<T> q = mOrigin - start;
        T a = dir.Dot(dir);
        T b = dir.Dot(q);
        T c = dir.Dot(mDirection);
        T d = q.Dot(mDirection);
        T e = mDirection.Dot(mDirection);

        T det = a * e - c * c;
        T t, s;

        if (det < T(1e-6)) {
            s = 0;
            t = d / e;
        } else {
            s = (b * e - c * d) / det;
            t = (a * d - c * b) / det;
        }

        if (s < 0) s = 0;
        else if (s > 1) s = 1;

        if (t < 0) {
            TVector3<T> closestPoint = start + dir * s;
            return (mOrigin - closestPoint).Length();
        }

        TVector3<T> rayPoint = GetPoint(t);
        TVector3<T> segmentPoint = start + dir * s;
        return (rayPoint - segmentPoint).Length();
    }

    /**
     * @brief 变换射线
     * @param transform 变换
     * @return 变换后的射线
     */
    TRay<T> Transform(const TTransform<T>& transform) const
    {
        TRay<T> result;
        result.mOrigin = transform.TransformPoint(mOrigin);
        result.mDirection = transform.TransformDirection(mDirection);
        return result;
    }

    /**
     * @brief 反向变换射线
     * @param transform 变换
     * @return 反向变换后的射线
     */
    TRay<T> InverseTransform(const TTransform<T>& transform) const
    {
        TRay<T> result;
        result.mOrigin = transform.InverseTransformPoint(mOrigin);
        result.mDirection = transform.InverseTransformDirection(mDirection);
        return result;
    }
};

} // namespace SympConv

#endif // SYMPCONV_RAY_H
