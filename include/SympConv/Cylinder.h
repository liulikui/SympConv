#ifndef SYMPCONV_CYLINDER_H
#define SYMPCONV_CYLINDER_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include "math.h"
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
    T mHalfHeight;      ///< 圆柱体的半高（沿Y轴）
    T mRadius;      ///< 圆柱体半径

public:
    /**
     * @brief 默认构造函数
     */
    TCylinder() : 
        mHalfHeight(T(0.5)),
        mRadius(T(0.5)) {}

    /**
     * @brief 带参数的构造函数
     * @param height 总高度
     * @param radius 半径
     */
    TCylinder(T height, T radius) :
        mHalfHeight(height / 2),
        mRadius(radius) {}

    /**
     * @brief 获取体积
     * @return 体积
     */
    fpnumber GetVolume() const
    {
        // 圆柱体的体积 = π * r² * h
        T r = mRadius;
        T h = mHalfHeight * 2;
        return M_PI * r * r * h;
    }
};

// 类型别名
typedef TCylinder<float> Cylinderf;
typedef TCylinder<double> Cylinderd;
typedef TCylinder<fpnumber> Cylinder;

} // namespace SympConv

#endif // SYMPCONV_CYLINDER_H