#pragma once
#include <cstdint>
#include "config.h"
#include "version.h"

BFBMP_BEGIN
struct BFBMP_API header
{
    uint8_t  magic[4] { 'F', 'B', 'M', 'P' };
    version  version = { BFBMP_VERSION_MAJOR, BFBMP_VERSION_MINOR };
    uint32_t chunk_size = 0;
    uint32_t total_size = 0;
    uint32_t metadata_position = 0;
    uint32_t image_position = 0;
    uint32_t sound_position = 0;
    uint32_t game_position = 0;
};
BFBMP_END