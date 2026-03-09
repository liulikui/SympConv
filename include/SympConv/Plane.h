#ifndef SYMPCONV_PLANE_H
#define SYMPCONV_PLANE_H

#include "Config.h"
#include <type_traits>
#include <cmath>
#include "Vector.h"

namespace SympConv {

/**
 * @brief 平面模板类
 * @tparam T 浮点类型，如float、double
 * 
 * 平面表示为 ax + by + cz + d = 0，其中 (a, b, c) 是法向量，d 是常数项
 */
template<typename T>
struct TPlane
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T x; ///< 法向量x分量
    T y; ///< 法向量y分量
    T z; ///< 法向量z分量
    T w; ///< 平面常数项，等于 -d

public:
    /**
     * @brief 默认构造函数
     */
    TPlane()
        : x(0), y(0), z(0), w(0)
    {
    }

    /**
     * @brief 带参数的构造函数
     * @param InX 法向量x分量
     * @param InY 法向量y分量
     * @param InZ 法向量z分量
     * @param InW 平面常数项
     */
    TPlane(T InX, T InY, T InZ, T InW)
        : x(InX)
        , y(InY)
        , z(InZ)
        , w(InW)
    {

    }

    /**
     * @brief 从法向量和点创建平面
     * @param normal 平面法向量
     * @param point 平面上的点
     */
    TPlane(const TVector3<T>& normal, const TVector3<T>& point)
    {
        x = normal.x;
        y = normal.y;
        z = normal.z;
        w = -normal.Dot(point);
    }

    /**
     * @brief 从三个点创建平面
     * @param p1 第一个点
     * @param p2 第二个点
     * @param p3 第三个点
     */
    TPlane(const TVector3<T>& p1, const TVector3<T>& p2, const TVector3<T>& p3)
    {
        TVector3<T> v1 = p2 - p1;
        TVector3<T> v2 = p3 - p1;
        TVector3<T> normal = v1.Cross(v2);
        
        x = normal.x;
        y = normal.y;
        z = normal.z;
        w = -normal.Dot(p1);
    }

    /**
     * @brief 计算点到平面的距离
     * @param point 要计算距离的点
     * @return 点到平面的距离，正值表示点在法向量方向的一侧
     */
    T DistanceTo(const TVector3<T>& point) const
    {
        return x * point.x + y * point.y + z * point.z + w;
    }

    /**
     * @brief 检查点是否在平面上
     * @param point 要检查的点
     * @param epsilon 容差
     * @return 如果点在平面上返回true，否则返回false
     */
    bool Contains(const TVector3<T>& point, T epsilon = T(1e-6)) const
    {
        return std::abs(DistanceTo(point)) < epsilon;
    }

    /**
     * @brief 获取平面的法向量
     * @return 平面的法向量
     */
    TVector3<T> GetNormal() const
    {
        return TVector3<T>(x, y, z);
    }

    /**
     * @brief 归一化平面
     * @return 归一化后的平面
     */
    TPlane<T> Normalize() const
    {
        T length = std::sqrt(x * x + y * y + z * z);
        if (length > T(0))
        {
            T invLength = T(1) / length;
            return TPlane<T>(x * invLength, y * invLength, z * invLength, w * invLength);
        }
        return *this;
    }

    /**
     * @brief 计算平面与平面的交线
     * @param other 另一个平面
     * @param lineOrigin 交线的原点
     * @param lineDirection 交线的方向
     * @return 如果两个平面相交返回true，否则返回false
     */
    bool Intersect(const TPlane<T>& other, TVector3<T>& lineOrigin, TVector3<T>& lineDirection) const
    {
        TVector3<T> n1 = GetNormal();
        TVector3<T> n2 = other.GetNormal();
        
        lineDirection = n1.Cross(n2);
        T lengthSquared = lineDirection.LengthSquared();
        
        if (lengthSquared < T(1e-12))
        {
            return false; // 平面平行
        }
        
        // 计算交线的一个点
        T det = n1.x * n2.y - n1.y * n2.x;
        if (std::abs(det) > T(1e-6))
        {
            T x = (n2.x * w - n1.x * other.w) / det;
            T y = (n1.y * other.w - n2.y * w) / det;
            lineOrigin = TVector3<T>(x, y, T(0));
        }
        else
        {
            det = n1.x * n2.z - n1.z * n2.x;
            if (std::abs(det) > T(1e-6))
            {
                T x = (n2.x * w - n1.x * other.w) / det;
                T z = (n1.z * other.w - n2.z * w) / det;
                lineOrigin = TVector3<T>(x, T(0), z);
            }
            else
            {
                det = n1.y * n2.z - n1.z * n2.y;
                if (std::abs(det) > T(1e-6))
                {
                    T y = (n2.y * w - n1.y * other.w) / det;
                    T z = (n1.z * other.w - n2.z * w) / det;
                    lineOrigin = TVector3<T>(T(0), y, z);
                }
                else
                {
                    return false; // 平面重合
                }
            }
        }
        
        return true;
    }
};

// 类型别名
typedef TPlane<float> Planef;
typedef TPlane<double> Planed;
typedef TPlane<fpnumber> Plane;

} // namespace SympConv

#endif // SYMPCONV_PLANE_H
