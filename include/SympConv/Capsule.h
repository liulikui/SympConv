#ifndef SYMPCONV_CAPSULE_H
#define SYMPCONV_CAPSULE_H

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include "Math.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 胶囊体模板类
 * @details 表示三维空间中的胶囊体，由两个端点和半径定义
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TCapsule
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    T mHalfHeight;      ///< 胶囊体半高
    T mRadius;          ///< 胶囊体半径

public:
    /**
     * @brief 默认构造函数
     */
    TCapsule() : 
        mHalfHeight(T(0.5)),
        mRadius(T(0.5)) {}

    /**
     * @brief 带参数的构造函数
     * @param height 高
     * @param radius 半径
     */
    TCapsule(T height, T radius) :
        mHalfHeight(height / 2),
        mRadius(radius) {}

    /**
     * @brief 获取体积
     * @return 体积
     */
    Real GetVolume() const
    {
        // 胶囊体的体积 = 圆柱体体积 + 两个半球体体积
        // 圆柱体体积 = π * r² * h
        // 两个半球体体积 = (4/3) * π * r³
        T r = mRadius;
        T h = mHalfHeight * 2;
        return M_PI * r * r * h + (4.0f / 3.0f) * M_PI * r * r * r;
    }
};

// 类型别名
typedef TCapsule<float> Capsulef;
typedef TCapsule<double> Capsuled;
typedef TCapsule<Real> Capsule;

} // namespace SympConv

#endif // SYMPCONV_CAPSULE_H
