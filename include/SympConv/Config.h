#ifndef SYMPCONV_CONFIG_H
#define SYMPCONV_CONFIG_H

namespace SympConv {

#if defined(SYMPCONV_DOUBLE_PRECISION_ENABLED)
    using Real = double;
#else
    using Real = float;
#endif
}
#endif // SYMPCONV_CONFIG_H
