#pragma once

#include <cmath>

namespace SympConvTest {

inline bool FloatEqual(float a, float b, float epsilon = 1e-4f) {
    return std::abs(a - b) < epsilon;
}

} // namespace SympConvTest