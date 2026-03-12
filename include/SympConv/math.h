#ifndef SYMPCONV_MATH_H
#define SYMPCONV_MATH_H

#include "Config.h"

namespace SympConv {

// 数学常量
#define M_PI 3.14159265358979323846

inline bool RealEqual(SympConv::Real a, SympConv::Real b, SympConv::Real epsilon = 1e-6f)
{
	return std::abs(a - b) < epsilon;
}

// 模板实现
template <typename T>
T Clamp(T value, T min, T max)
{
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}

template <typename T>
T Lerp(T a, T b, T t)
{
	return a + (b - a) * t;
}

template <typename T>
T Sign(T value)
{
	if (value > 0) {
		return 1;
	}
	if (value < 0) {
		return -1;
	}
	return 0;
}

} // namespace SympConv

#endif // SYMPCONV_MATH_H