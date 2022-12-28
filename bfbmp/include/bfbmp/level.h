#pragma once
#include <string>
#include <vector>
#include "config.h"
#include "note.h"

BFBMP_BEGIN
struct BFBMP_API level
{
    std::string       name;
    float             scroll_speed = 0.0f;
    std::vector<note> notes;
};
BFBMP_END