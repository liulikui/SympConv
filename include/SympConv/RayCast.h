#ifndef SYMPCONV_RAY_CAST_H
#define SYMPCONV_RAY_CAST_H

#include "Vector.h"
#include <cmath>
#include <vector>

namespace SympConv {

struct RayCastResult
{
	Vector3 mPoint;
	Vector3 mNormal;
	Real mHit;
};

struct RayCastResults
{
	std::vector<RayCastResult> mHits;
};

} // namespace SympConv

#endif // SYMPCONV_RAY_CAST_H
