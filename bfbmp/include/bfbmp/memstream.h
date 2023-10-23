#pragma once
#include "def.h"

typedef enum __bfbmp_seek
{
    bfbmp_seek_begin,
    bfbmp_seek_current,
    bfbmp_seek_end,
} bfbmp_seek;

typedef enum __bfbmp_open_mode_flags
{
    bfbmp_mode_none = 1 << 0,
    bfbmp_mode_read = 1 << 1,
    bfbmp_mode_write = 1 << 2
} bfbmp_open_mode;

typedef struct __bfbmp_memstream
{
    uint64_t position;
    size_t size;
    uint8_t* data;
    uint8_t opened;
    bfbmp_open_mode mode;
} bfbmp_memstream_t;


BFBMP_PUBLIC bfbmp_memstream_t BFBMP_API bfbmp_memstream_create(void* data, size_t size, int8_t mode);
BFBMP_PUBLIC uint8_t           BFBMP_API bfbmp_memstream_open(bfbmp_memstream_t* memstream, void* data, size_t size, int8_t mode);
BFBMP_PUBLIC uint8_t           BFBMP_API bfbmp_memstream_read(bfbmp_memstream_t* memstream, void* buffer, size_t element_count, size_t element_size);
BFBMP_PUBLIC uint8_t           BFBMP_API bfbmp_memstream_write(bfbmp_memstream_t* memstream, const void* data, size_t size);
BFBMP_PUBLIC uint8_t           BFBMP_API bfbmp_memstream_close(bfbmp_memstream_t* memstream);
BFBMP_PUBLIC uint8_t           BFBMP_API bfbmp_memstream_seek(bfbmp_memstream_t* memstream, int64_t offset, bfbmp_seek seek_type);
BFBMP_PUBLIC uint64_t          BFBMP_API bfbmp_memstream_tell(const bfbmp_memstream_t* memstream);
BFBMP_PUBLIC uint8_t           BFBMP_API bfbmp_memstream_is_opened(const bfbmp_memstream_t* memstream);
