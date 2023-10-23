#include "bfbmp/header.h"
#include "bfbmp/memstream.h"
#include <string.h>

bfbmp_header_t bfbmp_header_create(void)
{
    bfbmp_header_t header;
    memcpy(header.magic, "FBMP", 4);
    header.version = BFBMP_VERSION;
    header.chunk_size = 0;
    header.total_size = 0;
    header.metadata_position = 0;
    header.image_position = 0;
    header.sound_position = 0;
    header.game_position = 0;
    return header;
}

uint8_t bfbmp_header_read(bfbmp_memstream_t* memstream, bfbmp_header_t* header)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(header);
    bfbmp_memstream_seek(memstream, 0, bfbmp_seek_begin);
    BFBMP_CHECK(bfbmp_memstream_read(memstream, header, 1, sizeof(bfbmp_header_t)));
    BFBMP_CHECK(header->chunk_size);
    BFBMP_CHECK(header->total_size);
    return BFBMP_TRUE;
}
