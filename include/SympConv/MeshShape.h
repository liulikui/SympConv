#ifndef SYMPCONV_MESH_SHAPE_H
#define SYMPCONV_MESH_SHAPE_H

#include "Shape.h"

namespace SympConv {

template<typename T>
class TMeshShape : public TShape<T>
{
public:
	TMeshShape() :
		TShape<T>(InShapeType){}
};

} // namespace SympConv

#endif // SYMPCONV_MESH_SHAPE_H
