///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  (C) Erwann Messoah 2022
//  Binary Format Beatmap reader/writer
//  C++ Header only library
//  Version 0.3
//
//  BUILD OPTIONS
//  You can #define BFBMP_MALLOC | BFBMP_REALLOC | BFBMP_FREE | BFBMP_MEMSET
//  if you want to use your custom memory allocation functions.
//
//  You can #define BFBMP_NO_FILE if you want to disable all file io functions
//  -> bfbmp_read_all_file(), bfbmp_read_metadata_file(), bfbmp_read_audio_file(), bfbmp_read_img_file()
//
//
//  USAGE
//  (inside a c/cpp file)
//  #define BFBMP_IMPLEMENTATION
//  #include "bfbmp.h"
//
//  For reading bfbmp file.
//  bfbmp* p_bfbmp = bfbmp_init();
//  bfbmp_read_file(filepath, p_bfbmp) or bfbmp_read_memory(buffer, p_bfbmp)
//
//  For writing bfbmp file.
//  bfbmp* p_bfbmp = bfbmp_init_data(params);
//  bfbmp_write_file(filename, p_bfbmp);
//
//  Don't remember to free up the bfbmp object when you no longer need it.
//  bfbmp_free(p_bfbmp);
//
// +────────────────────────────────+───────────────+───────────────────────────────────────────────────+
// | Binary Format Beatmap (bfbmp)  |               |                                                   |
// +────────────────────────────────+───────────────+───────────────────────────────────────────────────+
// | HEADER                         |               |                                                   |
// | Type                           | Size (Bytes)  | Description                                       |
// | char[4]                        | 4             | File ID "FBMP"                                    |
// | int32                          | 4             | File format version, moslty '0'                   |
// | int32                          | 4             | Total file size, should be 12  + meta chunk size  |
// |                                |               |                                                   |
// | META CHUNK                     |               |                                                   |
// | Type                           | Size (Bytes)  | Description                                       |
// | char[4]                        | 4             | Meta chunk ID "meta"                              |
// | int32                          | 4             | Meta chunk size                                   |
// | int32                          | 4             | Song name length                                  |
// | string                         | length        | Song name                                         |
// | int32                          | 4             | Sub name length                                   |
// | string                         | length        | Sub name                                          |
// | int32                          | 4             | Author name length                                |
// | string                         | length        | Author name                                       |
// | int32                          | 4             | Mapper name length                                |
// | string                         | length        | Mapper name                                       |
// | int32                          | 4             | Song BPM                                          |
// | float                          | 4             | Start Offset                                      |
// | int32                          | 4             | Beats per measure                                 |
// |                                |               |                                                   |
// | IMG CHUNK                      |               |                                                   |
// | Type                           | Size (Bytes)  | Description                                       |
// | char[4]                        | 4             | IMG chunk ID "!img"                               |
// | int32                          | 4             | IMG data size                                     |
// | char[size]                     | size          | IMG data                                          |
// |                                |               |                                                   |
// | SND CHUNK                      |               |                                                   |
// | Type                           | Size (Bytes)  | Description                                       |
// | char[4]                        | 4             | "SND chunk ID "!snd"                              |
// | int32                          | 4             | SND data size                                     |
// | char[size]                     | size          | SND data                                          |
// +────────────────────────────────+───────────────+───────────────────────────────────────────────────+
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef BFBMP_HEADER
#define BFBMP_HEADER

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(BFBMP_NO_STDIO)
#include <stdio.h>
#endif

#if !defined(BFBMP_MALLOC) || !defined(BFBMP_REALLOC) || !defined(BFBMP_FREE)
#define BFBMP_MALLOC(size)              malloc(size)
#define BFBMP_REALLOC(ptr, new_size)    realloc(ptr, new_size)
#define BFBMP_FREE(ptr)                 free(ptr)
#define BFBMP_MEMSET(ptr, value, size)  memset(ptr, value, size)
#define BFBMP_MEMCPY(dst, src, size)    memcpy(dst, src, size)
#endif

#define BFBMP_VERSION_BIGGER    0
#define BFBMP_VERSION_MAJOR     0
#define BFBMP_VERSION_MINOR     3
#define BFBMP_VERSION_PATCH     0
#if defined(BFBMP_ENCODE_BIG_ENDIAN)
#define BFBMP_VERSION ((BFBMP_VERSION_BIGGER << 24 & 0xFF000000) | (BFBMP_VERSION_MAJOR << 16 & 0xFF0000) | (BFBMP_VERSION_MINOR << 8 & 0xFF00) | BFBMP_VERSION_PATCH & 0xFF)
#else
#define BFBMP_VERSION ((BFBMP_VERSION_PATCH << 24 & 0xFF000000) | (BFBMP_VERSION_MINOR << 16 & 0xFF0000) | (BFBMP_VERSION_MAJOR << 8 & 0xFF00) | BFBMP_VERSION_BIGGER & 0xFF)
#endif

#define BFBMP_API       extern
#define BFBMP_PRIVATE   static
#define BFBMP_INLINE    inline

#define BFBMP_TRUE 		1
#define BFBMP_FALSE 	0
#define BFBMP_NULL_CHARACTER '\0'

#if defined(BFBMP_USE_ASSERT)
#include <assert.h>
#define BFBMP_ASSERT(condition, message) assert((condition) && (message))
#endif

#define BFBMP_CHECK(condition, result) if(!(condition)) return result


typedef char		    bfbmp_char;
typedef uint8_t 	    bfbmp_bool;
typedef int8_t 		    bfbmp_int8;
typedef int16_t 	    bfbmp_int16;
typedef int32_t		    bfbmp_int32;
typedef uint8_t 	    bfbmp_uint8;
typedef uint16_t 	    bfbmp_uint16;
typedef uint32_t 	    bfbmp_uint32;
typedef size_t 	        bfbmp_size;
typedef float 		    bfbmp_float;
typedef double		    bfbmp_float64;
typedef void* 		    bfbmp_address;
typedef FILE*           bfbmp_file;


typedef void(*bfbmp_read_callback)(bfbmp_char* buffer, bfbmp_size size);
typedef void(*bfbmp_write_callback)(const bfbmp_char* filepath, bfbmp_size size);

typedef enum
{
    bfbmp_success = 0,
    bfbmp_error,
    bfbmp_io_error,
    bfbmp_nullptr_error,
    bfbmp_id_mismatch,
    bfbmp_read_error,
    bfbmp_not_ready,
    bfbmp_wrong_file_extension,
    bfbmp_no_audio_data,
    bfbmp_no_image_data,
    bfbmp_empty_buffer
} bfbmp_result;

typedef struct
{
    bfbmp_char*  song_name;
    bfbmp_char*  sub_name;
    bfbmp_char*  author_name;
    bfbmp_char*  mapper_name;
    bfbmp_int32  beats_per_minute;
    bfbmp_int32  beats_per_measure;
    bfbmp_float  start_offset;
} bfbmp_metadata;

typedef struct
{
    bfbmp_uint8* data;
    bfbmp_size size;
} bfbmp_data;

typedef struct
{
    bfbmp_uint8 bigger;
    bfbmp_uint8 major;
    bfbmp_uint8 minor;
    bfbmp_uint8 patch;
} bfbmp_version;

typedef struct
{
    bfbmp_version       version;
    bfbmp_size          size;
    bfbmp_metadata      metadata;
    bfbmp_data          cover;
    bfbmp_data          audio;
} bfbmp;

// Version 0.4, not yet implemented
typedef enum
{
    bfbmp_wav,
    bfbmp_ogg,
    bfbmp_mp3,
    bfbmp_flac
} bfbmp_audio_type;

/////////////////////////////////////////////////////////////////////
/// PRIVATE API
/////////////////////////////////////////////////////////////////////
BFBMP_PRIVATE   bfbmp_uint8*    bfbmp__read_file_to_memory(const bfbmp_char* filepath, bfbmp_size* out_file_size);
BFBMP_PRIVATE   bfbmp_result    bfbmp__free_buffer(void* buffer);
BFBMP_PRIVATE   void            bfbmp__read(bfbmp_address dest, const void* buffer, bfbmp_size size);
BFBMP_PRIVATE   void            bfbmp__read_string(bfbmp_address dest, const void* buffer, bfbmp_size size);

/////////////////////////////////////////////////////////////////////
/// PUBLIC API
/////////////////////////////////////////////////////////////////////
BFBMP_API       bfbmp*          bfbmp_init(void);
BFBMP_API       bfbmp*          bfbmp_init_data(
                                const bfbmp_char* song_name,
                                const bfbmp_char* sub_name,
                                const bfbmp_char* author_name,
                                const bfbmp_char* mapper_name,
                                bfbmp_int32 bpm,
                                bfbmp_float start_off,
                                bfbmp_int32 bpmeasure,
                                const bfbmp_char* img_path,
                                const bfbmp_char* snd_path
                                );
BFBMP_API       void            bfbmp_free(bfbmp* pbfbmp);
BFBMP_API       bfbmp_uint32    bfbmp_get_version(void);
BFBMP_API       bfbmp_result    bfbmp_read_memory(bfbmp_uint8* buffer, bfbmp_size size, bfbmp* pbfbmp);
BFBMP_API       bfbmp_result    bfbmp_read_metadata_file(const bfbmp_char* filepath, bfbmp_metadata* metadata);
BFBMP_API       bfbmp_result    bfbmp_read_file(const bfbmp_char* filepath, bfbmp* pbfbmp);
BFBMP_API       bfbmp_result    bfbmp_read_metadata_memory(bfbmp_uint8* buffer, bfbmp_size size, bfbmp_metadata* metadata);
BFBMP_API       bfbmp_result    bfbmp_write_file(const bfbmp_char* filepath, bfbmp* pbfbmp);
/////////////////////////////////////////////////////////////////////
/// IMPLEMENTATION
/////////////////////////////////////////////////////////////////////
#if defined(BFBMP_IMPLEMENTATION)
BFBMP_API bfbmp* bfbmp_init(void)
{
    bfbmp* pbfbmp = BFBMP_MALLOC(sizeof(bfbmp));
    BFBMP_MEMSET(pbfbmp, 0, sizeof(bfbmp));
    const bfbmp_uint32 version = bfbmp_get_version();
    BFBMP_MEMCPY(&pbfbmp->version, &version, sizeof(bfbmp_version));
    return pbfbmp;
}

BFBMP_API bfbmp* bfbmp_init_data(
const bfbmp_char* song_name,
const bfbmp_char* sub_name,
const bfbmp_char* author_name,
const bfbmp_char* mapper_name,
bfbmp_int32 bpm,
bfbmp_float start_off,
bfbmp_int32 bpmeasure,
const bfbmp_char* img_path,
const bfbmp_char* snd_path
)
{
    bfbmp* pbfbmp = BFBMP_MALLOC(sizeof(bfbmp));
    BFBMP_MEMSET(pbfbmp, 0, sizeof(bfbmp));
    const bfbmp_uint32 version = bfbmp_get_version();
    BFBMP_MEMCPY(&pbfbmp->version, &version, sizeof(bfbmp_version));

    pbfbmp->metadata.song_name = BFBMP_MALLOC(strlen(song_name) + 1);
    BFBMP_MEMCPY(pbfbmp->metadata.song_name, song_name, strlen(song_name));
    pbfbmp->metadata.song_name[strlen(song_name)] = BFBMP_NULL_CHARACTER;
    
    pbfbmp->metadata.sub_name = BFBMP_MALLOC(strlen(sub_name) + 1);
    BFBMP_MEMCPY(pbfbmp->metadata.sub_name, sub_name, strlen(sub_name));
    pbfbmp->metadata.sub_name[strlen(sub_name)] = BFBMP_NULL_CHARACTER;

    pbfbmp->metadata.author_name = BFBMP_MALLOC(strlen(author_name) + 1);
    BFBMP_MEMCPY(pbfbmp->metadata.author_name, author_name, strlen(author_name));
    pbfbmp->metadata.author_name[strlen(author_name)] = BFBMP_NULL_CHARACTER;

    pbfbmp->metadata.mapper_name = BFBMP_MALLOC(strlen(mapper_name) + 1);
    BFBMP_MEMCPY(pbfbmp->metadata.mapper_name, mapper_name, strlen(mapper_name));
    pbfbmp->metadata.mapper_name[strlen(mapper_name)] = BFBMP_NULL_CHARACTER;
    
    pbfbmp->metadata.beats_per_minute = bpm;
    pbfbmp->metadata.start_offset = start_off;
    pbfbmp->metadata.beats_per_measure = bpmeasure;

    pbfbmp->cover.data = bfbmp__read_file_to_memory(img_path, &pbfbmp->cover.size);
    pbfbmp->audio.data = bfbmp__read_file_to_memory(snd_path, &pbfbmp->audio.size);
    return pbfbmp;
}

BFBMP_API void bfbmp_free(bfbmp* pbfbmp)
{
    BFBMP_FREE(pbfbmp->metadata.song_name);
    BFBMP_FREE(pbfbmp->metadata.sub_name);
    BFBMP_FREE(pbfbmp->metadata.author_name);
    BFBMP_FREE(pbfbmp->metadata.mapper_name);
    BFBMP_FREE(pbfbmp->cover.data);
    BFBMP_FREE(pbfbmp->audio.data);
    BFBMP_FREE(pbfbmp);
}

BFBMP_API bfbmp_uint32 bfbmp_get_version(void)
{
    return BFBMP_VERSION;
}

BFBMP_PRIVATE bfbmp_uint8* bfbmp__read_file_to_memory(const bfbmp_char* filepath, bfbmp_size* out_file_size)
{
    bfbmp_file file = fopen(filepath, "rb");
    fseek(file, 0, SEEK_END);
    const bfbmp_size size = ftell(file);
    fseek(file, 0, SEEK_SET);
    bfbmp_uint8* buffer = BFBMP_MALLOC(size);
    fread(buffer, size, 1, file);
    if(out_file_size) *out_file_size = size;
    return buffer;
}

BFBMP_PRIVATE bfbmp_result bfbmp__free_buffer(void* buffer)
{
    BFBMP_FREE(buffer);
    return bfbmp_success;
}

BFBMP_PRIVATE void bfbmp__read(bfbmp_address dest, bfbmp_uint8* buffer, bfbmp_size size)
{
    
    BFBMP_MEMCPY(dest, buffer, size);
}

BFBMP_PRIVATE void bfbmp__read_string(bfbmp_address dest, bfbmp_uint8* buffer, bfbmp_size size)
{
    bfbmp_uint8* str = dest;
    BFBMP_MEMCPY(str, buffer, size);
    str[size] = BFBMP_NULL_CHARACTER;
}

BFBMP_API bfbmp_result bfbmp_read_memory(bfbmp_uint8* buffer, bfbmp_size size, bfbmp* pbfbmp)
{
    bfbmp_char id[5];
    bfbmp__read_string(id, buffer, 4);
    buffer += 4;
    if(strcmp("FBMP", id) != 0)
        return bfbmp_id_mismatch;

    bfbmp__read(&pbfbmp->version, buffer, 4);
    buffer += 4;
    bfbmp__read(&pbfbmp->size, buffer, 4);
    buffer += 4;

    bfbmp__read_string(id, buffer, 4);
    buffer += 4;
    if(strcmp("META", id) != 0)
        return bfbmp_id_mismatch;

    buffer += 4;
    
    bfbmp_size length = 0;
    bfbmp_char* string = NULL;
    
    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    pbfbmp->metadata.song_name = string;
    buffer += length;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    pbfbmp->metadata.sub_name = string;
    buffer += length;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    pbfbmp->metadata.author_name = string;
    buffer += length;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    pbfbmp->metadata.mapper_name = string;
    buffer += length;
    
    bfbmp__read(&pbfbmp->metadata.beats_per_minute, buffer, 4);
    buffer += 4;
    bfbmp__read(&pbfbmp->metadata.start_offset, buffer, 4);
    buffer += 4;
    bfbmp__read(&pbfbmp->metadata.beats_per_measure, buffer, 4);
    buffer += 4;

    bfbmp__read_string(id, buffer, 4);
    buffer += 4;
    if(strcmp("!IMG", id) != 0)
        return bfbmp_id_mismatch;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    pbfbmp->cover.size = length;
    pbfbmp->cover.data = BFBMP_MALLOC(length);
    bfbmp__read(pbfbmp->cover.data, buffer, length);
    buffer += length;

    bfbmp__read_string(id, buffer, 4);
    buffer += 4;
    if(strcmp("!SND", id) != 0)
        return bfbmp_id_mismatch;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    pbfbmp->audio.size = length;
    pbfbmp->audio.data = BFBMP_MALLOC(length);
    bfbmp__read(pbfbmp->audio.data, buffer, length);
    buffer += length;
    return bfbmp_success;
}

BFBMP_API bfbmp_result bfbmp_read_file(const bfbmp_char* filepath, bfbmp* pbfbmp)
{
    bfbmp_size size;
    bfbmp_uint8* file = bfbmp__read_file_to_memory(filepath, &size);
    return bfbmp_read_memory(file, size, pbfbmp);
}

BFBMP_API bfbmp_result bfbmp_read_metadata_memory(bfbmp_uint8* buffer, bfbmp_size size, bfbmp_metadata* metadata)
{
    bfbmp_char id[5];
    bfbmp__read_string(id, buffer, 4);
    buffer += 4;
    if(strcmp("FBMP", id) != 0)
        return bfbmp_id_mismatch;
    
    buffer += 8;

    bfbmp__read_string(id, buffer, 4);
    buffer += 4;
    if(strcmp("META", id) != 0)
        return bfbmp_id_mismatch;

    buffer += 4;
    
    bfbmp_size length = 0;
    bfbmp_char* string = NULL;
    
    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    metadata->song_name = string;
    buffer += length;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    metadata->sub_name = string;
    buffer += length;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    metadata->author_name = string;
    buffer += length;

    bfbmp__read(&length, buffer, 4);
    buffer += 4;
    string = BFBMP_MALLOC(length);
    bfbmp__read(string, buffer, length);
    string[length] = BFBMP_NULL_CHARACTER;
    metadata->mapper_name = string;
    buffer += length;
    
    bfbmp__read(&metadata->beats_per_minute, buffer, 4);
    buffer += 4;
    bfbmp__read(&metadata->start_offset, buffer, 4);
    buffer += 4;
    bfbmp__read(&metadata->beats_per_measure, buffer, 4);
    return bfbmp_success;
}

BFBMP_API bfbmp_result bfbmp_read_metadata_file(const bfbmp_char* filepath, bfbmp_metadata* metadata)
{
    bfbmp_size size;
    bfbmp_uint8* file = bfbmp__read_file_to_memory(filepath, &size);
    return bfbmp_read_metadata_memory(file, size, metadata);
}

BFBMP_API bfbmp_result bfbmp_write_file(const bfbmp_char* filepath, bfbmp* pbfbmp)
{
    bfbmp_file file = fopen(filepath, "wb");
    if(!file) return bfbmp_nullptr_error;
    bfbmp_size size = 0;
    fwrite("FBMP", 4, 1, file);
    fwrite(&pbfbmp->version, sizeof(bfbmp_version), 1, file);
    fwrite("____", 4, 1, file);
    const bfbmp_int32 file_begin = ftell(file);

    fwrite("META", 4, 1, file);
    fwrite("____", 4, 1, file);
    const bfbmp_int32 meta_begin = ftell(file);
    size = strlen(pbfbmp->metadata.song_name);
    fwrite(&size, 4, 1, file);
    fwrite(pbfbmp->metadata.song_name, size, 1, file);
    
    size = strlen(pbfbmp->metadata.sub_name);
    fwrite(&size, 4, 1, file);
    fwrite(pbfbmp->metadata.sub_name, size, 1, file);

    size = strlen(pbfbmp->metadata.author_name);
    fwrite(&size, 4, 1, file);
    fwrite(pbfbmp->metadata.author_name, size, 1, file);

    size = strlen(pbfbmp->metadata.mapper_name);
    fwrite(&size, 4, 1, file);
    fwrite(pbfbmp->metadata.mapper_name, size, 1, file);

    fwrite(&pbfbmp->metadata.beats_per_minute, 4, 1, file);
    fwrite(&pbfbmp->metadata.start_offset, 4, 1, file);
    fwrite(&pbfbmp->metadata.beats_per_measure, 4, 1, file);
    const bfbmp_int32 meta_end = ftell(file);
    fwrite("!IMG", 4, 1, file);
    fwrite(&pbfbmp->cover.size, 4, 1, file);
    fwrite(pbfbmp->cover.data, pbfbmp->cover.size, 1, file);

    fwrite("!SND", 4, 1, file);
    fwrite(&pbfbmp->audio.size, 4, 1, file);
    fwrite(pbfbmp->audio.data, pbfbmp->audio.size, 1, file);
    const bfbmp_int32 file_end = ftell(file);

    fseek(file, meta_begin - 4, SEEK_SET);
    const bfbmp_int32 meta_size = meta_end - meta_begin;
    fwrite(&meta_size, 4, 1, file);

    fseek(file, file_begin - 4, SEEK_SET);
    const bfbmp_int32 file_size = file_end - file_begin;
    fwrite(&file_size, 4, 1, file);

    fclose(file);
    return bfbmp_success;
}
#endif // IMPLEMENTATION
#endif  // BFBMP_HEADER