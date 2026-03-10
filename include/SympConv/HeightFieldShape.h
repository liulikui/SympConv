#ifndef SYMPCONV_SHAPE_H
#define SYMPCONV_SHAPE_H

#include "Config.h"

namespace SympConv {

enum class EShapeType : uint8_t
{
	Plane,
	Convex,
	Mesh,
	HeightField,

	User1,
	User2,
	User3,
};

template<typename T>
class TShape
{
public:
	TShape(EShapeType InShapeType) :
		mShapeType(InShapeType){}

	EShapeType GetShapeType() const { return mShapeType; }

private:
	EShapeType mShapeType;
};

} // namespace SympConv

#endif // SYMPCONV_PLANE_H
