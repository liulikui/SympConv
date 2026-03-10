#ifndef SYMPCONV_PLANE_SHAPE_H
#define SYMPCONV_PLANE_SHAPE_H

#include "Shape.h"

namespace SympConv {

/**
 * @brief 平面形状模板类
 * @details 实现了平面的形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TPlaneShape : public TShape<T>
{
public:
	/**
	 * @brief 构造函数
	 */
	TPlaneShape() :
		TShape<T>(EShapeType::Plane){}
};

} // namespace SympConv

#endif // SYMPCONV_PLANE_SHAPE_H
