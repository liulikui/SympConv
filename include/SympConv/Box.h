#ifndef SYMPCONV_BOX_H
#define SYMPCONV_BOX_H

#include "Config.h"
#include "Vector.h"
#include "Transform.h"
#include "AABB.h"
#include <type_traits>

namespace SympConv {

/**
 * @brief 盒子模板类
 * @details 表示三维空间中的盒子，由半长
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
struct TBox
{
    static_assert(std::is_floating_point_v<T>, "T must be floating point");

public:
    TVector3<T> mHalfExtents; ///< 半长（从中心到边的距离）

public:
    /**
     * @brief 默认构造函数
     */
    TBox() : 
        mHalfExtents(T(0.5), T(0.5), T(0.5)) { }

    /**
     * @brief 带参数的构造函数
     * @param halfExtents 半长
     */
    TBox(const TVector3<T>& halfExtents) :
        mHalfExtents(halfExtents) { }

    /**
     * @brief 获取体积
     * @return 体积
     */
    fpnumber GetVolume() const
    {
        // 盒子的体积 = 2 * 半长x * 2 * 半长y * 2 * 半长z = 8 * 半长x * 半长y * 半长z
        return 8.0f * mHalfExtents.x * mHalfExtents.y * mHalfExtents.z;
    }
};

// 类型别名
typedef TBox<float> Boxf;
typedef TBox<double> Boxd;
typedef TBox<fpnumber> Box;

} // namespace SympConv

#endif // SYMPCONV_BOX_H