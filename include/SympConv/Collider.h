#ifndef SYMPCONV_COLLIDER_H
#define SYMPCONV_COLLIDER_H

#include "Config.h"
#include "Vector.h"
#include "Transform.h"
#include "ShapeType.h"

namespace SympConv {

/**
 * @brief 形状基类模板
 * @details 所有形状的抽象基类，定义了形状的基本接口
 */
class Collider
{
public:
	/**
	 * @brief 构造函数
	 */
	Collider(Shape* shape) : mShape(shape) {}

	/**
	 * @brief 获取变换
	 * @return 变换
	 */
	const Transform& GetTransform() const { return mTransform; }

	/**
	 * @brief 设置变换
	 */
	void SetTransform(const Transform& InTransform) { mTransform = InTransform; }

protected:
	Shape* mShape;
	Transform mTransform; ///< 变换
};

} // namespace SympConv

#endif // SYMPCONV_COLLIDER_H
