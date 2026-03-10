#ifndef SYMPCONV_CONVEX_SHAPE_H
#define SYMPCONV_CONVEX_SHAPE_H

#include "Shape.h"
#include "Vector.h"

namespace SympConv {

/**
 * @brief 凸形状类型枚举
 * @details 定义了系统支持的各种凸形状类型
 */
enum class EConvexShapeType : uint8_t
{
	Box,        ///< 盒子形状
	Sphere,     ///< 球体形状
	Cylinder,   ///< 圆柱体形状
	Capsule,    ///< 胶囊体形状
	Cone,       ///< 圆锥体形状
	ConvexHull, ///< 凸包形状
};

/**
 * @brief 凸形状基类模板
 * @details 所有凸形状的抽象基类，定义了凸形状的基本接口
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TConvexShape : public TShape<T>
{
public:
	/**
	 * @brief 构造函数
	 * @param InConvexShapeType 凸形状类型
	 */
	TConvexShape(EConvexShapeType InConvexShapeType) :
		TShape<T>(EShapeType::Convex), mConvexShapeType(InConvexShapeType) {}

	/**
	 * @brief 获取凸形状类型
	 * @return 凸形状类型
	 */
	EConvexShapeType GetConvexShapeType() const { return mConvexShapeType; }

	/**
	 * @brief 获取在指定方向上的支持点
	 * @param direction 方向向量
	 * @return 支持点
	 */
	virtual Vector3 GetSupport(const Vector3& direction) const = 0;

private:
	EConvexShapeType mConvexShapeType; ///< 凸形状类型
};

} // namespace SympConv

#endif // SYMPCONV_CONVEX_SHAPE_H
