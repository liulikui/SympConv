#ifndef SYMPCONV_CONFIG_H
#define SYMPCONV_CONFIG_H

#include <cstdint>
#include <limits>
#include <cfloat>
#include "Enviroment.h"

namespace SympConv {

#if defined(SYMPCONV_DOUBLE_PRECISION_ENABLED)
    using fpnumber = double;
#else
    using fpnumber = float;
#endif
}
#endif // SYMPCONV_CONFIG_H
