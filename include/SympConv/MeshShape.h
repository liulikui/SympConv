#ifndef SYMPCONV_MESH_SHAPE_H
#define SYMPCONV_MESH_SHAPE_H

#include "Shape.h"

namespace SympConv {

/**
 * @brief 网格形状模板类
 * @details 实现了网格的形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TMeshShape : public TShape<T>
{
public:
	/**
	 * @brief 构造函数
	 */
	TMeshShape() :
		TShape<T>(EShapeType::Mesh){}
};

} // namespace SympConv

#endif // SYMPCONV_MESH_SHAPE_H
