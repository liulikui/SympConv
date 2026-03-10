#ifndef SYMPCONV_SHAPE_H
#define SYMPCONV_SHAPE_H

#include "Config.h"

namespace SympConv {

/**
 * @brief 形状类型枚举
 * @details 定义了系统支持的各种形状类型
 */
enum class EShapeType : uint8_t
{
	Plane,      ///< 平面形状
	Convex,     ///< 凸形状
	Mesh,       ///< 网格形状
	HeightField,///< 高度场形状

	User1,      ///< 用户自定义形状类型1
	User2,      ///< 用户自定义形状类型2
	User3,      ///< 用户自定义形状类型3
};

/**
 * @brief 形状基类模板
 * @details 所有形状的抽象基类，定义了形状的基本接口
 * @tparam T 浮点类型，如float、double
 */
template<typename T>
class TShape
{
public:
	/**
	 * @brief 构造函数
	 * @param InShapeType 形状类型
	 */
	TShape(EShapeType InShapeType) :
		mShapeType(InShapeType){}

	/**
	 * @brief 虚析构函数
	 */
	virtual ~TShape() {}

	/**
	 * @brief 获取形状类型
	 * @return 形状类型
	 */
	EShapeType GetShapeType() const { return mShapeType; }

private:
	EShapeType mShapeType; ///< 形状类型
};

} // namespace SympConv

#endif // SYMPCONV_SHAPE_H
