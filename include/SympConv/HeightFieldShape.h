#ifndef SYMPCONV_HEIGHT_FIELD_SHAPE_H
#define SYMPCONV_HEIGHT_FIELD_SHAPE_H

#include "Shape.h"

namespace SympConv {

/**
 * @brief 高度场形状模板类
 * @details 实现了高度场的形状接口，用于碰撞检测等场景
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class THeightFieldShape : public TShape<T>
{
public:
	/**
	 * @brief 构造函数
	 */
	THeightFieldShape() :
		TShape<T>(EShapeType::HeightField){}
};

} // namespace SympConv

#endif // SYMPCONV_HEIGHT_FIELD_SHAPE_H
