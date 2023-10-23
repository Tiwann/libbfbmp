#include "bfbmp/memstream.h"
#include "bfbmp/math.h"
#include <string.h>

uint8_t bfbmp_memstream_open(bfbmp_memstream_t* memstream, void* data, size_t size, int8_t mode)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(data);
    BFBMP_CHECK(size > 0);

    if(bfbmp_memstream_is_opened(memstream)) bfbmp_memstream_close(memstream);
    
    memstream->data = (uint8_t*)data;
    memstream->size = size;
    memstream->position = 0;
    memstream->opened = BFBMP_TRUE;
    memstream->mode = (bfbmp_open_mode)mode;
    return BFBMP_TRUE;
}


uint8_t bfbmp_memstream_read(bfbmp_memstream_t* memstream, void* buffer, size_t element_count, size_t element_size)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(buffer);
    BFBMP_CHECK(memstream->position + element_count * element_size <= memstream->size);
    BFBMP_CHECK(memstream->mode & bfbmp_mode_read);
    BFBMP_CHECK(memcpy(buffer, memstream->data + memstream->position, element_count * element_size));
    memstream->position += element_count * element_size;
    return BFBMP_TRUE;
}

uint8_t bfbmp_memstream_write(bfbmp_memstream_t* memstream, const void* data, size_t size)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(data);
    BFBMP_CHECK(memstream->mode & bfbmp_mode_write);
    BFBMP_CHECK(memstream->position + size <= memstream->size);
    BFBMP_CHECK(memcpy(memstream->data + memstream->position, data, size));
    memstream->position += size;
    return BFBMP_TRUE;
}

uint8_t bfbmp_memstream_close(bfbmp_memstream_t* memstream)
{
    BFBMP_CHECK(memstream);
    memstream->data = (void*)0;
    memstream->position = 0;
    memstream-> size = 0;
    memstream->opened = BFBMP_FALSE;
    return BFBMP_TRUE;
}

uint8_t bfbmp_memstream_seek(bfbmp_memstream_t* memstream, int64_t offset, bfbmp_seek seek_type)
{
    BFBMP_CHECK(memstream);
    uint64_t new_pos = 0;
    
    switch (seek_type)
    {
    case bfbmp_seek_begin:
        new_pos = bfbmp_wrap_uint64_t(offset, 0, memstream->size);
        break;
    case bfbmp_seek_current:
        new_pos = bfbmp_wrap_uint64_t(memstream->position + offset, 0, memstream->size);
        break;
    case bfbmp_seek_end:
        new_pos = bfbmp_wrap_uint64_t(memstream->size + offset, 0, memstream->size);
        break;
    }

    memstream->position = new_pos;
    return BFBMP_TRUE;
}

uint64_t bfbmp_memstream_tell(const bfbmp_memstream_t* memstream)
{
    return memstream->position;
}

uint8_t bfbmp_memstream_is_opened(const bfbmp_memstream_t* memstream)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(memstream->data);
    return memstream->size > 0 && memstream->opened;
}
