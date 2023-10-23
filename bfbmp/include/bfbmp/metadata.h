#pragma once
#include "def.h"

typedef struct __bfbmp_metadata
{
    uint8_t     magic[4];
    uint32_t    chunk_size;
    char*       song_name;
    char*       sub_name;
    char*       author_name;
    char*       mapper_name;
    float       beats_per_minute;
    uint8_t     beats_per_measure;
    float       start_offset;
} bfbmp_metadata_t;

typedef struct __bfbmp_memstream bfbmp_memstream_t;
typedef struct __bfbmp_header bfbmp_header_t;

BFBMP_PUBLIC bfbmp_metadata_t BFBMP_API bfbmp_metadata_create(void);
BFBMP_PUBLIC bfbmp_metadata_t BFBMP_API bfbmp_metadata_create_with_names(const char* song, const char* sub, const char* author, const char* mapper);
BFBMP_PUBLIC uint8_t          BFBMP_API bfbmp_metadata_read(bfbmp_memstream_t* memstream, const bfbmp_header_t* header, bfbmp_metadata_t* metadata);
BFBMP_PUBLIC uint8_t          BFBMP_API bfbmp_metadata_free(bfbmp_metadata_t* metadata);
BFBMP_PUBLIC uint8_t          BFBMP_API bfbmp_metadata_set_song_name(bfbmp_metadata_t* metadata, const char* song_name);
BFBMP_PUBLIC uint8_t          BFBMP_API bfbmp_metadata_set_sub_name(bfbmp_metadata_t* metadata, const char* sub_name);
BFBMP_PUBLIC uint8_t          BFBMP_API bfbmp_metadata_set_author_name(bfbmp_metadata_t* metadata, const char* author_name);
BFBMP_PUBLIC uint8_t          BFBMP_API bfbmp_metadata_set_mapper_name(bfbmp_metadata_t* metadata, const char* mapper_name);