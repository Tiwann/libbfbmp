#pragma once
#include "def.h"

#define BFBMP_TEMPLATE_FUNC_WRAP(T) BFBMP_PUBLIC T BFBMP_API bfbmp_wrap_##T(T val, T lower, T upper)

BFBMP_TEMPLATE_FUNC_WRAP(int64_t);
BFBMP_TEMPLATE_FUNC_WRAP(uint64_t);
BFBMP_TEMPLATE_FUNC_WRAP(int32_t);
BFBMP_TEMPLATE_FUNC_WRAP(uint32_t);

#define BFBMP_DEFINE_TEMPLATE_WRAP_VALUE(T) \
T bfbmp_wrap_##T(T val, T lower, T upper) \
{ \
    T range = upper - lower; \
    return val < lower ?  upper - ((lower - val) % range) : val > upper ? lower + ((val - upper) % range) : val; \
}