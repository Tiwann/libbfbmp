#pragma once
#include <cstdint>
#include <string>
#include "config.h"

BFBMP_BEGIN
struct BFBMP_API metadata
{
    uint8_t     magic[4] { 'M', 'E', 'T', 'A' };
    uint32_t    chunk_size = 0;
    std::string song_name;
    std::string sub_name;
    std::string author_name;
    std::string mapper_name;
    float       beats_per_minute = 0.0f;
    uint8_t     beats_per_measure = 0;
    float       start_offset = 0.0f;
};
BFBMP_END