#ifndef SYMPCONV_AABB_H
#define SYMPCONV_AABB_H

#include "Vector.h"
#include "Ray.h"
#include "Transform.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 轴对齐包围盒模板类
 * @details 表示三维空间中的轴对齐包围盒，由最小和最大坐标定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TAABB
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mMin;      ///< 最小坐标
    TVector3<T> mMax;      ///< 最大坐标

public:
    /**
     * @brief 默认构造函数
     * @details 初始化为空包围盒（最小值为无穷大，最大值为负无穷大）
     */
    TAABB() : 
        mMin(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max()), 
        mMax(-std::numeric_limits<T>::max(), -std::numeric_limits<T>::max(), -std::numeric_limits<T>::max()) {}

    /**
     * @brief 带参数的构造函数
     * @param min 最小坐标
     * @param max 最大坐标
     */
    TAABB(const TVector3<T>& min, const TVector3<T>& max) :
        mMin(min), 
        mMax(max) {}

    /**
     * @brief 从单个点创建包围盒
     * @param point 点
     */
    TAABB(const TVector3<T>& point) :
        mMin(point), 
        mMax(point) {}

    /**
     * @brief 从三个点创建包围盒（三角形）
     * @param v0 第一个点
     * @param v1 第二个点
     * @param v2 第三个点
     */
    TAABB(const TVector3<T>& v0, const TVector3<T>& v1, const TVector3<T>& v2) :
        mMin(std::min({v0.x, v1.x, v2.x}), std::min({v0.y, v1.y, v2.y}), std::min({v0.z, v1.z, v2.z})),
        mMax(std::max({v0.x, v1.x, v2.x}), std::max({v0.y, v1.y, v2.y}), std::max({v0.z, v1.z, v2.z})) {}

    /**
     * @brief 复制构造函数
     * @param other 另一个包围盒
     */
    TAABB(const TAABB<T>& other) = default;

    /**
     * @brief 移动构造函数
     * @param other 另一个包围盒
     */
    TAABB(TAABB<T>&& other) = default;

    /**
     * @brief 赋值运算符
     * @param other 另一个包围盒
     * @return 引用到当前包围盒
     */
    TAABB<T>& operator=(const TAABB<T>& other) = default;

    /**
     * @brief 移动赋值运算符
     * @param other 另一个包围盒
     * @return 引用到当前包围盒
     */
    TAABB<T>& operator=(TAABB<T>&& other) = default;

    /**
     * @brief 重置包围盒为空
     */
    void Reset()
    {
        mMin = TVector3<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
        mMax = TVector3<T>(-std::numeric_limits<T>::max(), -std::numeric_limits<T>::max(), -std::numeric_limits<T>::max());
    }

    /**
     * @brief 扩展包围盒以包含点
     * @param point 点
     */
    void Expand(const TVector3<T>& point)
    {
        mMin.x = std::min(mMin.x, point.x);
        mMin.y = std::min(mMin.y, point.y);
        mMin.z = std::min(mMin.z, point.z);
        mMax.x = std::max(mMax.x, point.x);
        mMax.y = std::max(mMax.y, point.y);
        mMax.z = std::max(mMax.z, point.z);
    }

    /**
     * @brief 扩展包围盒以包含另一个包围盒
     * @param other 另一个包围盒
     */
    void Expand(const TAABB<T>& other)
    {
        mMin.x = std::min(mMin.x, other.mMin.x);
        mMin.y = std::min(mMin.y, other.mMin.y);
        mMin.z = std::min(mMin.z, other.mMin.z);
        mMax.x = std::max(mMax.x, other.mMax.x);
        mMax.y = std::max(mMax.y, other.mMax.y);
        mMax.z = std::max(mMax.z, other.mMax.z);
    }

    /**
     * @brief 扩展包围盒以包含球体
     * @param center 球体中心
     * @param radius 球体半径
     */
    void Expand(const TVector3<T>& center, T radius)
    {
        TVector3<T> offset(radius, radius, radius);
        Expand(center - offset);
        Expand(center + offset);
    }

    /**
     * @brief 计算包围盒的中心点
     * @return 中心点
     */
    TVector3<T> GetCenter() const
    {
        return (mMin + mMax) * T(0.5);
    }

    /**
     * @brief 计算包围盒的大小
     * @return 大小向量
     */
    TVector3<T> GetSize() const
    {
        return mMax - mMin;
    }

    /**
     * @brief 计算包围盒的半径
     * @return 半径
     */
    T GetRadius() const
    {
        return GetSize().Length() * T(0.5);
    }

    /**
     * @brief 计算包围盒的表面积
     * @return 表面积
     */
    T GetSurfaceArea() const
    {
        TVector3<T> size = GetSize();
        return T(2) * (size.x * size.y + size.y * size.z + size.z * size.x);
    }

    /**
     * @brief 计算包围盒的体积
     * @return 体积
     */
    T GetVolume() const
    {
        TVector3<T> size = GetSize();
        return size.x * size.y * size.z;
    }

    /**
     * @brief 检查点是否在包围盒内
     * @param point 点
     * @return 是否在包围盒内
     */
    bool Contains(const TVector3<T>& point) const
    {
        return point.x >= mMin.x && point.x <= mMax.x &&
               point.y >= mMin.y && point.y <= mMax.y &&
               point.z >= mMin.z && point.z <= mMax.z;
    }

    /**
     * @brief 检查另一个包围盒是否完全在当前包围盒内
     * @param other 另一个包围盒
     * @return 是否完全在包围盒内
     */
    bool Contains(const TAABB<T>& other) const
    {
        return Contains(other.mMin) && Contains(other.mMax);
    }

    /**
     * @brief 检查与另一个包围盒是否相交
     * @param other 另一个包围盒
     * @return 是否相交
     */
    bool Intersects(const TAABB<T>& other) const
    {
        return !(mMin.x > other.mMax.x || mMax.x < other.mMin.x ||
                 mMin.y > other.mMax.y || mMax.y < other.mMin.y ||
                 mMin.z > other.mMax.z || mMax.z < other.mMin.z);
    }

    /**
     * @brief 检查与射线是否相交
     * @param ray 射线
     * @param tMin 最小相交参数
     * @param tMax 最大相交参数
     * @return 是否相交
     */
    bool Intersects(const TRay<T>& ray, T& tMin, T& tMax) const
    {
        return ray.Intersects(*this, tMin, tMax);
    }

    /**
     * @brief 检查与射线是否相交
     * @param ray 射线
     * @return 是否相交
     */
    bool Intersects(const TRay<T>& ray) const
    {
        T tMin, tMax;
        return Intersects(ray, tMin, tMax);
    }

    /**
     * @brief 计算与另一个包围盒的交集
     * @param other 另一个包围盒
     * @return 交集包围盒
     */
    TAABB<T> Intersect(const TAABB<T>& other) const
    {
        TAABB<T> result;
        result.mMin.x = std::max(mMin.x, other.mMin.x);
        result.mMin.y = std::max(mMin.y, other.mMin.y);
        result.mMin.z = std::max(mMin.z, other.mMin.z);
        result.mMax.x = std::min(mMax.x, other.mMax.x);
        result.mMax.y = std::min(mMax.y, other.mMax.y);
        result.mMax.z = std::min(mMax.z, other.mMax.z);
        
        // 检查交集是否有效
        if (result.mMin.x > result.mMax.x || result.mMin.y > result.mMax.y || result.mMin.z > result.mMax.z) {
            result.Reset();
        }
        
        return result;
    }

    /**
     * @brief 计算与另一个包围盒的并集
     * @param other 另一个包围盒
     * @return 并集包围盒
     */
    TAABB<T> Union(const TAABB<T>& other) const
    {
        TAABB<T> result = *this;
        result.Expand(other);
        return result;
    }

    /**
     * @brief 变换包围盒
     * @param transform 变换
     * @return 变换后的包围盒
     */
    TAABB<T> Transform(const TTransform<T>& transform) const
    {
        TAABB<T> result;
        
        // 变换8个顶点
        TVector3<T> vertices[8] = {
            mMin,
            TVector3<T>(mMax.x, mMin.y, mMin.z),
            TVector3<T>(mMin.x, mMax.y, mMin.z),
            TVector3<T>(mMax.x, mMax.y, mMin.z),
            TVector3<T>(mMin.x, mMin.y, mMax.z),
            TVector3<T>(mMax.x, mMin.y, mMax.z),
            TVector3<T>(mMin.x, mMax.y, mMax.z),
            mMax
        };
        
        for (int i = 0; i < 8; ++i) {
            result.Expand(transform.TransformPoint(vertices[i]));
        }
        
        return result;
    }

    /**
     * @brief 计算点到包围盒的最短距离
     * @param point 点
     * @return 最短距离
     */
    T DistanceTo(const TVector3<T>& point) const
    {
        if (Contains(point)) {
            return T(0);
        }
        
        TVector3<T> closest;
        closest.x = std::max(mMin.x, std::min(point.x, mMax.x));
        closest.y = std::max(mMin.y, std::min(point.y, mMax.y));
        closest.z = std::max(mMin.z, std::min(point.z, mMax.z));
        
        return (point - closest).Length();
    }

    /**
     * @brief 检查包围盒是否为空
     * @return 是否为空
     */
    bool IsEmpty() const
    {
        return mMin.x > mMax.x || mMin.y > mMax.y || mMin.z > mMax.z;
    }

    /**
     * @brief 缩放包围盒
     * @param scale 缩放因子
     * @return 缩放后的包围盒
     */
    TAABB<T> Scale(T scale) const
    {
        TVector3<T> center = GetCenter();
        TVector3<T> extents = (mMax - center) * scale;
        return TAABB<T>(center - extents, center + extents);
    }

    /**
     * @brief 平移包围盒
     * @param offset 平移量
     * @return 平移后的包围盒
     */
    TAABB<T> Translate(const TVector3<T>& offset) const
    {
        return TAABB<T>(mMin + offset, mMax + offset);
    }

    /**
     * @brief 静态方法：从点集创建包围盒
     * @param points 点集
     * @param count 点的数量
     * @return 包围盒
     */
    static TAABB<T> FromPoints(const TVector3<T>* points, size_t count)
    {
        TAABB<T> result;
        for (size_t i = 0; i < count; ++i) {
            result.Expand(points[i]);
        }
        return result;
    }

    /**
     * @brief 静态方法：从球体创建包围盒
     * @param center 球体中心
     * @param radius 球体半径
     * @return 包围盒
     */
    static TAABB<T> FromSphere(const TVector3<T>& center, T radius)
    {
        TVector3<T> offset(radius, radius, radius);
        return TAABB<T>(center - offset, center + offset);
    }
};

} // namespace SympConv

#endif // SYMPCONV_AABB_H
