#pragma once
#include "config.h"

BFBMP_BEGIN
enum class BFBMP_API compression_type
{
    uncompressed,
    rle,
    deflate
};
BFBMP_END