#include "bfbmp/metadata.h"
#include "bfbmp/memstream.h"
#include "bfbmp/header.h"
#include <string.h>
#include <stdlib.h>

bfbmp_metadata_t bfbmp_metadata_create(void)
{
    bfbmp_metadata_t metadata;
    memcpy(metadata.magic, "META", 4);
    metadata.chunk_size = 0;
    metadata.song_name = NULL;
    metadata.sub_name = NULL;
    metadata.author_name = NULL;
    metadata.mapper_name = NULL;
    metadata.beats_per_minute = 0.0f;
    metadata.beats_per_measure = 0;
    metadata.start_offset = 0.0f;
    return metadata;
}

bfbmp_metadata_t bfbmp_metadata_create_with_names(const char* song, const char* sub, const char* author, const char* mapper)
{
    bfbmp_metadata_t metadata;
    memcpy(metadata.magic, "META", 4);
    metadata.chunk_size = 0;
    metadata.song_name = (char*)malloc(strlen(song));
    memcpy(metadata.song_name, song, strlen(song));
    metadata.sub_name = (char*)malloc(strlen(sub));
    memcpy(metadata.sub_name, sub, strlen(sub));
    metadata.author_name = (char*)malloc(strlen(author));
    memcpy(metadata.author_name, author, strlen(author));
    metadata.mapper_name = (char*)malloc(strlen(mapper));
    memcpy(metadata.mapper_name, mapper, strlen(mapper));
    metadata.beats_per_minute = 0.0f;
    metadata.beats_per_measure = 0;
    metadata.start_offset = 0.0f;
    return metadata;
}

uint8_t bfbmp_metadata_read(bfbmp_memstream_t* memstream, const bfbmp_header_t* header, bfbmp_metadata_t* metadata)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(header);
    BFBMP_CHECK(metadata);
    BFBMP_CHECK(header->metadata_position);
    BFBMP_CHECK(bfbmp_memstream_is_opened(memstream));
    
    uint32_t length;
    uint8_t magic[4];
    bfbmp_memstream_seek(memstream, header->metadata_position, bfbmp_seek_begin);
    
    bfbmp_memstream_read(memstream, magic, 1, 4);
    BFBMP_CHECK(!strncmp((char*)magic, "META", 4));

    bfbmp_memstream_read(memstream, &metadata->chunk_size, 1, 4);
    
    bfbmp_memstream_read(memstream, &length, 1, 4);
    metadata->song_name = malloc(length);
    bfbmp_memstream_read(memstream, metadata->song_name, 1, length);

    bfbmp_memstream_read(memstream, &length, 1, 4);
    metadata->sub_name = malloc(length);
    bfbmp_memstream_read(memstream, metadata->sub_name, 1, length);
    
    bfbmp_memstream_read(memstream, &length, 1, 4);
    metadata->author_name = malloc(length);
    bfbmp_memstream_read(memstream, metadata->author_name, 1, length);

    bfbmp_memstream_read(memstream, &length, 1, 4);
    metadata->mapper_name = malloc(length);
    bfbmp_memstream_read(memstream, metadata->mapper_name, 1, length);

    bfbmp_memstream_read(memstream, &metadata->beats_per_minute, 1, 4);
    bfbmp_memstream_read(memstream, &metadata->beats_per_measure, 1, 1);
    bfbmp_memstream_read(memstream, &metadata->start_offset, 1, 4);
    return BFBMP_TRUE;
}

uint8_t bfbmp_metadata_free(bfbmp_metadata_t* metadata)
{
    BFBMP_CHECK(metadata);
    free(metadata->song_name);
    free(metadata->sub_name);
    free(metadata->author_name);
    free(metadata->mapper_name);
    return BFBMP_TRUE;
}


static uint8_t copy_string(char** dest, const char* source)
{
    if(*dest && strlen(*dest)) free(*dest);

    const size_t str_size = strlen(source);
    BFBMP_CHECK(str_size);
    *dest = (char*)calloc(1, str_size + 1);
    memcpy(*dest, source, str_size + 1);
    return BFBMP_TRUE;
}

uint8_t bfbmp_metadata_set_song_name(bfbmp_metadata_t* metadata, const char* song_name)
{
    BFBMP_CHECK(metadata);
    BFBMP_CHECK(song_name);
    return copy_string(&metadata->song_name, song_name);
}

uint8_t bfbmp_metadata_set_sub_name(bfbmp_metadata_t* metadata, const char* sub_name)
{
    BFBMP_CHECK(metadata);
    BFBMP_CHECK(sub_name);
    return copy_string(&metadata->sub_name, sub_name);
}

uint8_t bfbmp_metadata_set_author_name(bfbmp_metadata_t* metadata, const char* author_name)
{
    BFBMP_CHECK(metadata);
    BFBMP_CHECK(author_name);
    return copy_string(&metadata->author_name, author_name);
}

uint8_t bfbmp_metadata_set_mapper_name(bfbmp_metadata_t* metadata, const char* mapper_name)
{
    BFBMP_CHECK(metadata);
    BFBMP_CHECK(mapper_name);
    return copy_string(&metadata->mapper_name, mapper_name);
}
