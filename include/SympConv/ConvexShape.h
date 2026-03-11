#ifndef SYMPCONV_CONVEX_SHAPE_H
#define SYMPCONV_CONVEX_SHAPE_H

#include "Shape.h"

namespace SympConv {

/**
 * @brief 凸形状基类模板
 * @details 所有凸形状的抽象基类，定义了凸形状的基本接口
 */
class ConvexShape : public Shape
{
public:
	/**
	 * @brief 构造函数
	 * @param InConvexShapeType 凸形状类型
	 */
	ConvexShape(ShapeType InShapeType) :
		Shape(ShapeGroupType::Convex, InShapeType) {}

	/**
	 * @brief 在本地坐标系中获取支持点
	 * @param dir_local 本地坐标系中的方向向量
	 * @return 支持点
	 */
	virtual Vector3 GetLocalSupport(const Vector3& dir_local) const = 0;
};

} // namespace SympConv

#endif // SYMPCONV_CONVEX_SHAPE_H
