#ifndef SYMPCONV_RIGIDBODY_H
#define SYMPCONV_RIGIDBODY_H

#include "Config.h"
#include "Collider.h"

namespace SympConv {

/**
 * @brief 刚体
 */
class RigidBody
{
public:
	/**
	 * @brief 构造函数
	 * @param InShapeGroupType 形状组类型
	 * @param InShapeType 形状类型
	 */
	RigidBody(ShapeGroupType InShapeGroupType, ShapeType InShapeType) :
		mShapeGroupType(InShapeGroupType), mShapeType(InShapeType){}

	/**
	 * @brief 虚析构函数
	 */
	virtual ~RigidBody() {}

	/**
	 * @brief 获取形状组类型
	 * @return 形状组类型
	 */
	ShapeGroupType GetShapeGroupType() const { return mShapeGroupType; }

	/**
	 * @brief 获取形状类型
	 * @return 形状类型
	 */
	ShapeType GetShapeType() const { return mShapeType; }

	/**
	 * @brief 在本地坐标系中获取支持点
	 * @param dir_local 本地坐标系中的方向向量
	 * @return 支持点
	 */
	virtual Vector3 GetLocalSupport(const Vector3& dir_local) const = 0;

protected:
	ShapeGroupType mShapeGroupType; ///< 形状组类型
	ShapeType mShapeType; ///< 形状类型
	Transform mTransform; ///< 变换
};

} // namespace SympConv

#endif // SYMPCONV_SHAPE_H
