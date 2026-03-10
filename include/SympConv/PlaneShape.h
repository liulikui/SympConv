#ifndef SYMPCONV_PLANE_SHAPE_H
#define SYMPCONV_PLANE_SHAPE_H

#include "Shape.h"

namespace SympConv {

template<typename T>
class TPlaneShape : public TShape<T>
{
public:
	TPlaneShape() :
		TShape<T>(EShapeType::Plane){}
};

} // namespace SympConv

#endif // SYMPCONV_PLANE_SHAPE_H
