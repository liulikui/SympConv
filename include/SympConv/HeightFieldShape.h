#ifndef SYMPCONV_HEIGHT_FIELD_SHAPE_H
#define SYMPCONV_HEIGHT_FIELD_SHAPE_H

#include "Shape.h"

namespace SympConv {

template<typename T>
class THeightFieldShape : public TShape<T>
{
public:
	THeightFieldShape() :
		TShape<T>(EShapeType::HeightField){}
};

} // namespace SympConv

#endif // SYMPCONV_HEIGHT_FIELD_SHAPE_H
