#ifndef SYMPCONV_MATH_UTILS_H
#define SYMPCONV_MATH_UTILS_H

#include "Export.h"

namespace SympConv {

class MathUtils {
public:
 template <typename T>
 static T Clamp(T value, T min, T max);

 template <typename T>
 static T Lerp(T a, T b, T t);

 template <typename T>
 static T Sign(T value);
};

// 模板实现
 template <typename T>
 T MathUtils::Clamp(T value, T min, T max) {
 if (value < min) return min;
 if (value > max) return max;
 return value;
 }

 template <typename T>
 T MathUtils::Lerp(T a, T b, T t) {
 return a + (b - a) * t;
 }

 template <typename T>
 T MathUtils::Sign(T value) {
 if (value > 0) return 1;
 if (value < 0) return -1;
 return 0;
 }

} // namespace SympConv

#endif // SYMPCONV_MATH_UTILS_H