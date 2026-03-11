#ifndef SYMPCONV_SHAPE_H
#define SYMPCONV_SHAPE_H

#include "Config.h"
#include "Vector.h"
#include "ShapeType.h"
#include "AABB.h"

namespace SympConv {

/**
 * @brief 形状基类
 * @details 所有形状的抽象基类，定义了形状的基本接口
 */
class Shape
{
public:
	/**
	 * @brief 构造函数
	 * @param InShapeGroupType 形状组类型
	 * @param InShapeType 形状类型
	 */
	Shape(ShapeGroupType InShapeGroupType, ShapeType InShapeType) :
		mShapeGroupType(InShapeGroupType), mShapeType(InShapeType){}

	/**
	 * @brief 虚析构函数
	 */
	virtual ~Shape() {}

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
	 * @brief 获取在本地坐标系中的支持点
	 * @param dir_local 本地坐标系中的方向向量
	 * @return 支持点
	 */
	virtual Vector3 GetLocalSupport(const Vector3& dir_local) const = 0;

	/**
	 * @brief 获取在本地坐标系中的惯性张量
	 * @param mass 质量
	 * @return 惯性张量
	 */
	virtual Vector3 GetLocalInertiaTensor(fpnumber mass) const = 0;

	/**
	 * @brief 获取在本地坐标系中的AABB
	 * @return AABB
	 */
	virtual AABB GetLocalBounds() const = 0;

protected:
	ShapeGroupType mShapeGroupType; ///< 形状组类型
	ShapeType mShapeType; ///< 形状类型
};

} // namespace SympConv

#endif // SYMPCONV_SHAPE_H
