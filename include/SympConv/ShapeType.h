#ifndef SYMPCONV_SHAPE_TYPE_H
#define SYMPCONV_SHAPE_TYPE_H

#include <cstdint>

namespace SympConv {

/**
 * @brief 形状分组类型枚举
 * @details 定义了系统支持的各种形状分组类型
 */
enum class ShapeGroupType : uint8_t
{
	Convex,     ///< 凸形状

	Mesh,       ///< 网格形状
	HeightField,///< 高度场形状
	SoftBody,	///< 软体
};

/**
 * @brief 形状类型枚举
 * @details 定义了系统具体支持的各种形状类型
 */
enum class ShapeType : uint8_t
{
	Box,		///< 盒子
	Cylinder,   ///< 圆柱体
	Cone,		///< 圆锥体
	Sphere,		///< 球体
	Capsule,	///< 胶囊体
	Mesh,		///< 网格形状
	HeightField,///< 高度场
	SoftBody,	///< 软体
};

} // namespace SympConv

#endif // SYMPCONV_SHAPE_TYPE_H
