#pragma once
#include "def.h"

typedef struct __bfbmp_header
{
    uint8_t magic[4];
    uint32_t version;
    uint32_t chunk_size;
    uint32_t total_size;
    uint32_t metadata_position;
    uint32_t image_position;
    uint32_t sound_position;
    uint32_t game_position;
} bfbmp_header_t;

typedef struct __bfbmp_memstream bfbmp_memstream_t;

BFBMP_PUBLIC bfbmp_header_t BFBMP_API bfbmp_header_create(void);
BFBMP_PUBLIC uint8_t        BFBMP_API bfbmp_header_read(bfbmp_memstream_t* memstream, bfbmp_header_t* header);