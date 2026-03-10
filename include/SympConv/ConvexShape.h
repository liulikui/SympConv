#ifndef SYMPCONV_CONVEX_SHAPE_H
#define SYMPCONV_CONVEX_SHAPE_H

#include "Shape.h"
#include "Vector.h"

namespace SympConv {

enum class EConvexShapeType : uint8_t
{
	Box,
	Sphere,
	Cylinder,
	Capsule,
	Cone,
	ConvexHull,
};


template<typename T>
class TConvexShape : public TShape<T>
{
public:
	TConvexShape(EConvexShapeType InConvexShapeType) :
		TShape<T>(EShapeType::Convex), mConvexShapeType(InConvexShapeType) {}

	EConvexShapeType GetConvexShapeType() const { return mConvexShapeType; }

	virtual Vector3 GetSupport(const Vector3& direction) const = 0;

private:
	EConvexShapeType mConvexShapeType;
};

} // namespace SympConv

#endif // SYMPCONV_CONVEX_SHAPE_H
