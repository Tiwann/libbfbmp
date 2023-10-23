#include "bfbmp/beatmap.h"
#include "bfbmp/memstream.h"
#include "bfbmp/note.h"
#include "miniz/miniz.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BFBMP_IMPL_TEMPLATE_VECTOR_FUNCTIONS_STATIC(bfbmp_level_t, level)
BFBMP_IMPL_TEMPLATE_VECTOR_FUNCTIONS_STATIC(bfbmp_note_t, note)

BFBMP_PRIVATE int64_t bfbmp_ftell(FILE* stream)
{
#if defined(_MSC_VER)
    return _ftelli64(stream);
#elif defined(__GNUC__)
#define _FILE_OFFSET_BITS == 64
    return ftello(stream);
#endif
}

BFBMP_PRIVATE int32_t bfbmp_fseek(FILE* stream, int64_t offset, int32_t seek_mode)
{
#if defined(_MSC_VER)
    return _fseeki64(stream, offset, seek_mode);
#elif defined(__GNUC__)
#define _FILE_OFFSET_BITS == 64
    return fseeko(stream, offset, seek_mode);
#endif
}

BFBMP_PRIVATE uint8_t bfbmp_beatmap_image_read_private(bfbmp_memstream_t* memstream, bfbmp_beatmap_t* beatmap)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(beatmap);
    BFBMP_CHECK(memstream->opened);
    BFBMP_CHECK(beatmap->header.image_position);

    bfbmp_memstream_seek(memstream, beatmap->header.image_position, bfbmp_seek_begin);
    uint8_t magic[4];

    bfbmp_memstream_read(memstream, magic, 1, 4);
    BFBMP_CHECK(!strncmp((char*)magic, "!IMG", 4));
    bfbmp_memstream_read(memstream, &beatmap->image_size, 1, 4);
    BFBMP_CHECK(beatmap->image_size);
    beatmap->image_data = (uint8_t*)malloc(beatmap->image_size);
    bfbmp_memstream_read(memstream, beatmap->image_data, 1, beatmap->image_size);
    return BFBMP_TRUE;
}

BFBMP_PRIVATE uint8_t bfbmp_beatmap_sound_read_private(bfbmp_memstream_t* memstream, bfbmp_beatmap_t* beatmap)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(beatmap);
    BFBMP_CHECK(memstream->opened);
    BFBMP_CHECK(beatmap->header.sound_position);

    bfbmp_memstream_seek(memstream, beatmap->header.sound_position, bfbmp_seek_begin);
    uint8_t magic[4];
    
    bfbmp_memstream_read(memstream, magic, 1, 4);
    BFBMP_CHECK(!strncmp((char*)magic, "!SND", 4));
    bfbmp_memstream_read(memstream, &beatmap->sound_size, 1, 4);
    BFBMP_CHECK(beatmap->sound_size);
    beatmap->sound_data = (uint8_t*)malloc(beatmap->sound_size);
    bfbmp_memstream_read(memstream, beatmap->sound_data, 1, beatmap->sound_size);
    return BFBMP_TRUE;
}

BFBMP_PRIVATE uint8_t bfbmp_beatmap_game_read_private(bfbmp_memstream_t* memstream, bfbmp_beatmap_t* beatmap)
{
    BFBMP_CHECK(memstream);
    BFBMP_CHECK(beatmap);
    BFBMP_CHECK(memstream->opened);
    BFBMP_CHECK(beatmap->header.game_position);

    bfbmp_memstream_seek(memstream, beatmap->header.game_position, bfbmp_seek_begin);
    uint8_t magic[4];
    bfbmp_memstream_read(memstream, magic, 1, 4);

    BFBMP_CHECK(!strncmp((char*)magic, "!GAM", 4));

    uint32_t str_size;
    uint32_t lvl_size;
    uint32_t note_size;
    uint32_t chunk_size;

    bfbmp_memstream_read(memstream, &chunk_size, 1, 4);
    bfbmp_memstream_read(memstream, &lvl_size, 1, 4);
    
    for(uint32_t i = 0; i < lvl_size; i++)
    {
        bfbmp_memstream_read(memstream, magic, 1, 4);
        BFBMP_CHECK(!strncmp((char*)magic, "!LVL", 4));
        bfbmp_memstream_read(memstream, &str_size, 1, 4);
        bfbmp_level_t level = bfbmp_level_create();
        level.name = calloc(1, str_size);
        bfbmp_memstream_read(memstream, level.name, 1, str_size);
        bfbmp_memstream_read(memstream, &level.scroll_speed, 1, 4);
       
        bfbmp_memstream_read(memstream, magic, 1, 4);
        BFBMP_CHECK(!strncmp((char*)magic, "!NOT", 4));
        
        bfbmp_memstream_read(memstream, &note_size, 1, 4);
        for(uint32_t j = 0; j < note_size; j++)
        {
            bfbmp_note_t note = bfbmp_note_create();
            bfbmp_memstream_read(memstream, &note.type, 1, 4);
            bfbmp_memstream_read(memstream, &note.position, 1, 4);
            
            if(note.type == bfbmp_note_long) bfbmp_memstream_read(memstream, &note.duration, 1, 4);
            BFBMP_CHECK(bfbmp_vector_note_push(&level.notes, note));
        }
        BFBMP_CHECK(bfbmp_vector_level_push(&beatmap->game_data, level));
    }
    return BFBMP_TRUE;
}


bfbmp_beatmap_t bfbmp_beatmap_create(void)
{
    bfbmp_beatmap_t beatmap;
    beatmap.header = bfbmp_header_create();
    beatmap.metadata = bfbmp_metadata_create();
    beatmap.image_data = NULL;
    beatmap.image_size = 0;
    beatmap.sound_data = NULL;
    beatmap.sound_size = 0;
    beatmap.game_data = bfbmp_vector_level_create();
    return beatmap;
}

uint8_t bfbmp_beatmap_decode_memory(bfbmp_beatmap_t* beatmap, uint8_t* buffer, size_t size)
{
    BFBMP_CHECK(buffer);
    BFBMP_CHECK(beatmap);

    bfbmp_beatmap_free(beatmap);
    *beatmap = bfbmp_beatmap_create();
    
    bfbmp_memstream_t memstream;
    BFBMP_CHECK(bfbmp_memstream_open(&memstream, buffer, size, bfbmp_mode_read));
    
    BFBMP_CHECK(bfbmp_header_read(&memstream, &beatmap->header));
    BFBMP_CHECK(bfbmp_metadata_read(&memstream, &beatmap->header, &beatmap->metadata));
    BFBMP_CHECK(bfbmp_beatmap_image_read_private(&memstream, beatmap));
    BFBMP_CHECK(bfbmp_beatmap_sound_read_private(&memstream, beatmap));
    BFBMP_CHECK(bfbmp_beatmap_game_read_private(&memstream, beatmap));
    
    bfbmp_memstream_close(&memstream);
    return BFBMP_TRUE;
}

uint8_t bfbmp_beatmap_decode_file(bfbmp_beatmap_t* beatmap, const char* filepath)
{
    FILE* file = fopen(filepath, "rb");
    BFBMP_CHECK(file);
    bfbmp_fseek(file, 0, SEEK_END);
    const size_t file_size = bfbmp_ftell(file);
    bfbmp_fseek(file, 0, SEEK_SET);

    uint8_t* content = malloc(file_size);
    const size_t read = fread(content, 1, file_size, file);
    BFBMP_CHECK(read == file_size);
    fclose(file);

    const uint8_t result = bfbmp_beatmap_decode_memory(beatmap, content, file_size);
    free(content);
    return result;
}

void bfbmp_beatmap_free(bfbmp_beatmap_t* beatmap)
{
    if (!beatmap) return;

    bfbmp_metadata_free(&beatmap->metadata);
    for(size_t lvl = 0; lvl < beatmap->game_data.count; lvl++)
        bfbmp_vector_note_free(&beatmap->game_data.data[lvl].notes);
    
    bfbmp_vector_level_free(&beatmap->game_data);
    free(beatmap->image_data);
    free(beatmap->sound_data);
}

uint8_t bfbmp_beatmap_encode_file(const bfbmp_beatmap_t* beatmap, const char* filepath, uint8_t compress)
{
    BFBMP_CHECK(beatmap);
    BFBMP_CHECK(filepath);
    //BFBMP_CHECK(bfbmp_beatmap_validate(beatmap));

    FILE* file = fopen(filepath, "wb");
    BFBMP_CHECK(file);

    const int64_t header_beg = bfbmp_ftell(file);
    const uint32_t version = BFBMP_VERSION;
    const uint32_t zero = BFBMP_ZERO;
    
    fwrite("FBMP", 4, 1, file);
    fwrite(&version, 4, 1, file);
    fwrite("____", 4, 1, file);
    fwrite("____", 4, 1, file);
    
    fwrite(&zero, 4, 1, file);
    fwrite(&zero, 4, 1, file);
    fwrite(&zero, 4, 1, file);
    fwrite(&zero, 4, 1, file);
    const int64_t header_end = bfbmp_ftell(file);
    
    const int64_t meta_beg = bfbmp_ftell(file);
    uint32_t str_size;
    fwrite("META", 4, 1, file);
    fwrite("____", 4, 1, file);
    
    str_size = (uint32_t)strlen(beatmap->metadata.song_name) + 1;
    fwrite(&str_size, 4, 1, file);
    fwrite(beatmap->metadata.song_name, 1, str_size, file);

    str_size = (uint32_t)strlen(beatmap->metadata.sub_name) + 1;
    fwrite(&str_size, 4, 1, file);
    fwrite(beatmap->metadata.sub_name, 1, str_size, file);

    str_size = (uint32_t)strlen(beatmap->metadata.mapper_name) + 1;
    fwrite(&str_size, 4, 1, file);
    fwrite(beatmap->metadata.mapper_name, 1, str_size, file);

    str_size = (uint32_t)strlen(beatmap->metadata.author_name) + 1;
    fwrite(&str_size, 4, 1, file);
    fwrite(beatmap->metadata.author_name, 1, str_size, file);
    
    fwrite(&beatmap->metadata.beats_per_minute, 4, 1, file);
    fwrite(&beatmap->metadata.beats_per_measure, 1, 1, file);
    fwrite(&beatmap->metadata.start_offset, 4, 1, file);

    const int64_t meta_end = bfbmp_ftell(file);
    
    int64_t img_beg = 0;
    if(beatmap->image_data && beatmap->image_size)
    {
        img_beg = bfbmp_ftell(file);
        fwrite("!IMG", 4, 1, file);

        /*uint8_t* new_data = NULL;
        size_t new_size = 0;
        
        if(compress)
        {
            const int result = mz_compress(new_data, (mz_ulong*)&new_size, beatmap->image_data, beatmap->image_size);
            if(result != MZ_OK) return BFBMP_FALSE;
        } else
        {
            new_data = beatmap->image_data;
            new_size = beatmap->image_size;
        }

        fwrite(&new_size, 4, 1, file);
        fwrite(new_data, new_size, 1, file);*/

        fwrite(&beatmap->image_size, 4, 1, file);
        fwrite(beatmap->image_data, beatmap->image_size, 1, file);
    }
    const int64_t img_end = bfbmp_ftell(file); (void)img_end;
    
    int64_t snd_beg = 0;
    if(beatmap->sound_data && beatmap->sound_size)
    {
        snd_beg = bfbmp_ftell(file);
        fwrite("!SND", 4, 1, file);
        fwrite(&beatmap->sound_size, 4, 1, file);
        fwrite(beatmap->sound_data, beatmap->sound_size, 1, file);
    }
    const int64_t snd_end = bfbmp_ftell(file); (void)snd_beg;

    int64_t data_beg = 0;
    if(beatmap->game_data.data && beatmap->game_data.count)
    {
        data_beg = bfbmp_ftell(file);
        fwrite("!GAM", 4, 1, file);
        fwrite("____", 4, 1, file);
        fwrite(&beatmap->game_data.count, 4, 1, file);
        
        for(size_t level = 0; level < beatmap->game_data.count; level++)
        {
            fwrite("!LVL", 4, 1, file);
            const bfbmp_level_t* const current_level = beatmap->game_data.data + level;
            str_size = (uint32_t)strlen(current_level->name) + 1;
            fwrite(&str_size, 4, 1, file);
            fwrite(current_level->name, str_size, 1, file);
            fwrite(&current_level->scroll_speed, 4, 1, file);
            
            fwrite("!NOT", 4, 1, file);
            fwrite(&current_level->notes.count, 4, 1, file);
            for(size_t note = 0; note < current_level->notes.count; note++)
            {
                const bfbmp_note_t* const current_note = current_level->notes.data + note;
                fwrite(&current_note->type, 4, 1, file);
                fwrite(&current_note->position, 4, 1, file);
                if(current_note->type == bfbmp_note_long)
                    fwrite(&current_note->duration, 4, 1, file);
            }
        }
    }
    const int64_t data_end = bfbmp_ftell(file);

    // chunk size
    bfbmp_fseek(file, header_beg + 8, SEEK_SET);
    const int64_t header_size = header_end - header_beg;
    fwrite(&header_size, 4, 1, file);

    // total size
    bfbmp_fseek(file, header_beg + 12, SEEK_SET);
    fwrite(&data_end, 4, 1, file);

    // meta chunk size
    bfbmp_fseek(file, meta_beg + 4, SEEK_SET);
    const int64_t meta_size = meta_end - meta_beg;
    fwrite(&meta_size, 4, 1, file);

    // data chunk size
    bfbmp_fseek(file, data_beg + 4, SEEK_SET);
    const int64_t data_size = data_end - data_beg;
    fwrite(&data_size, 4, 1, file);
    
    // meta chunk position
    bfbmp_fseek(file, header_beg + 16, SEEK_SET);
    fwrite(&header_end, 4, 1, file);
    
    // img chunk position
    bfbmp_fseek(file, header_beg + 20, SEEK_SET);
    fwrite(&img_beg, 4, 1, file);

    // snd chunk position
    bfbmp_fseek(file, header_beg + 24, SEEK_SET);
    fwrite(&snd_beg, 4, 1, file);

    // data chunk position
    bfbmp_fseek(file, header_beg + 28, SEEK_SET);
    fwrite(&data_beg, 4, 1, file);
    
    fclose(file);
    return BFBMP_TRUE;
}

uint8_t bfbmp_beatmap_validate(const bfbmp_beatmap_t* beatmap)
{
    BFBMP_CHECK(beatmap);
    BFBMP_CHECK(beatmap->metadata.song_name && strlen(beatmap->metadata.song_name));
    BFBMP_CHECK(beatmap->metadata.sub_name && strlen(beatmap->metadata.sub_name));
    BFBMP_CHECK(beatmap->metadata.mapper_name && strlen(beatmap->metadata.mapper_name));
    BFBMP_CHECK(beatmap->metadata.author_name && strlen(beatmap->metadata.author_name));
    BFBMP_CHECK(beatmap->metadata.beats_per_measure);
    BFBMP_CHECK(beatmap->metadata.beats_per_minute == 0.0f);

    BFBMP_CHECK(beatmap->image_data && beatmap->image_size);
    BFBMP_CHECK(beatmap->sound_data && beatmap->sound_size);
    BFBMP_CHECK(beatmap->game_data.data && beatmap->game_data.count);
    return BFBMP_TRUE;
}

uint8_t bfbmp_beatmap_set_song_name(bfbmp_beatmap_t* beatmap, const char* song_name)
{
    BFBMP_CHECK(beatmap);
    return bfbmp_metadata_set_song_name(&beatmap->metadata, song_name);
}

uint8_t bfbmp_beatmap_set_sub_name(bfbmp_beatmap_t* beatmap, const char* sub_name)
{
    BFBMP_CHECK(beatmap);
    return bfbmp_metadata_set_sub_name(&beatmap->metadata, sub_name);
}

uint8_t bfbmp_beatmap_set_author_name(bfbmp_beatmap_t* beatmap, const char* author_name)
{
    BFBMP_CHECK(beatmap);
    return bfbmp_metadata_set_author_name(&beatmap->metadata, author_name);
}

uint8_t bfbmp_beatmap_set_mapper_name(bfbmp_beatmap_t* beatmap, const char* mapper_name)
{
    BFBMP_CHECK(beatmap);
    return bfbmp_metadata_set_mapper_name(&beatmap->metadata, mapper_name);
}

typedef struct bfbmp_file
{
    uint8_t* data;
    size_t   size;
} bfbmp_file_t;

static bfbmp_file_t* bfbmp_file_load(const char* filepath)
{
    bfbmp_file_t* file = (bfbmp_file_t*)malloc(sizeof(bfbmp_file_t));
    if(!file)
    {
        free(file);
        return NULL;
    }

    FILE* stream = fopen(filepath, "rb");
    if(!stream)
    {
        free(file);
        return NULL;
    }

    fseek(stream, 0, SEEK_END);
    file->size = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    file->data = malloc(file->size);
    fread(file->data, 1, file->size, stream);
    fclose(stream);
    return file;
}

uint8_t bfbmp_beatmap_load_image(bfbmp_beatmap_t* beatmap, const char* filepath)
{
    BFBMP_CHECK(beatmap);
    const bfbmp_file_t* file = bfbmp_file_load(filepath);
    BFBMP_CHECK(file);
    beatmap->image_data = file->data;
    beatmap->image_size = file->size;
    return BFBMP_TRUE;
}

uint8_t bfbmp_beatmap_load_sound(bfbmp_beatmap_t* beatmap, const char* filepath)
{
    BFBMP_CHECK(beatmap);
    const bfbmp_file_t* file = bfbmp_file_load(filepath);
    BFBMP_CHECK(file);
    beatmap->sound_data = file->data;
    beatmap->sound_size = file->size;
    return BFBMP_TRUE;
}
