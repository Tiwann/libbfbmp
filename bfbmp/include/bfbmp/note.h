#pragma once
#include "config.h"
#include <cstdint>

BFBMP_BEGIN
enum class BFBMP_API note_type : uint32_t
{
    note_normal,
    note_long
};

struct BFBMP_API note
{
    note_type type = note_type::note_normal;
    float position = 0.0f;
    float duration = 0.0f;
};
BFBMP_END