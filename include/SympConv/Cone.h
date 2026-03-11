#ifndef SYMPCONV_CONE_H
#define SYMPCONV_CONE_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include "math.h"
#include <type_traits>
#include <algorithm>

namespace SympConv {

/**
 * @brief 圆锥体模板类，圆心在坐标原点
 * @details 表示三维空间中的圆锥体，由圆锥高和半径定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TCone
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T mHeight; ///< 圆锥的高
    T mRadius; ///< 圆锥体底面半径

public:
    /**
     * @brief 默认构造函数
     */
    TCone() : 
        mHeight(T(1)),
        mRadius(T(0.5)) {}

    /**
     * @brief 带参数的构造函数
     * @param height 高
     * @param radius 底面半径
     */
    TCone(T height, T radius) :
        mHeight(height),
        mRadius(radius) {}

    /**
     * @brief 获取体积
     * @return 体积
     */
    fpnumber GetVolume() const
    {
        // 圆锥体的体积 = (1/3) * π * r² * h
        T r = mRadius;
        T h = mHeight;
        return (1.0f / 3.0f) * M_PI * r * r * h;
    }
};

// 类型别名
typedef TCone<float> Conef;
typedef TCone<double> Coned;
typedef TCone<fpnumber> Cone;

} // namespace SympConv

#endif // SYMPCONV_CONE_H