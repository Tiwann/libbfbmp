#pragma once
#include "def.h"
#include "header.h"
#include "metadata.h"
#include "level.h"
#include "vector.h"

BFBMP_DECLARE_TEMPLATE_VECTOR_STRUCT(bfbmp_level_t, level)

typedef struct __bfbmp_beatmap
{
    bfbmp_header_t header;
    bfbmp_metadata_t metadata;
    uint8_t* image_data;
    size_t image_size;
    uint8_t* sound_data;
    size_t sound_size;
    bfbmp_vector_level_t game_data;
} bfbmp_beatmap_t;


typedef struct __bfbmp_memstream bfbmp_memstream_t;

BFBMP_PUBLIC bfbmp_beatmap_t BFBMP_API bfbmp_beatmap_create(void);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_decode_memory(bfbmp_beatmap_t* beatmap, uint8_t* buffer, size_t size);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_decode_file(bfbmp_beatmap_t* beatmap, const char* filepath);
BFBMP_PUBLIC void            BFBMP_API bfbmp_beatmap_free(bfbmp_beatmap_t* beatmap);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_encode_file(const bfbmp_beatmap_t* beatmap, const char* filepath, uint8_t compress);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_validate(const bfbmp_beatmap_t* beatmap);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_set_song_name(bfbmp_beatmap_t* beatmap, const char* song_name);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_set_sub_name(bfbmp_beatmap_t* beatmap, const char* sub_name);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_set_author_name(bfbmp_beatmap_t* beatmap, const char* author_name);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_set_mapper_name(bfbmp_beatmap_t* beatmap, const char* mapper_name);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_load_image(bfbmp_beatmap_t* beatmap, const char* filepath);
BFBMP_PUBLIC uint8_t         BFBMP_API bfbmp_beatmap_load_sound(bfbmp_beatmap_t* beatmap, const char* filepath);

