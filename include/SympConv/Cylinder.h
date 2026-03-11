#ifndef SYMPCONV_CYLINDER_H
#define SYMPCONV_CYLINDER_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 圆柱体模板类
 * @details 表示三维空间中的圆柱体，由两个端点和半径定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TCylinder
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T mHeight;      ///< 圆柱体的高
    T mRadius;      ///< 圆柱体半径

public:
    /**
     * @brief 默认构造函数
     */
    TCylinder() : 
        mHeight(T(1)),
        mRadius(T(0.5)) {}

    /**
     * @brief 带参数的构造函数
     * @param start 起点
     * @param end 终点
     * @param radius 半径
     */
    TCylinder(T height, T radius) :
        mHeight(height),
        mRadius(radius) {}
};

// 类型别名
typedef TCylinder<float> Cylinderf;
typedef TCylinder<double> Cylinderd;
typedef TCylinder<fpnumber> Cylinder;

} // namespace SympConv

#endif // SYMPCONV_CYLINDER_H