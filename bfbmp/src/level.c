#include "bfbmp/level.h"
#include "bfbmp/note.h"
#include <stdlib.h>
#include <string.h>

BFBMP_IMPL_TEMPLATE_VECTOR_FUNCTIONS_STATIC(bfbmp_note_t, note)

bfbmp_level_t bfbmp_level_create(void)
{
    bfbmp_level_t level;
    level.name = NULL;
    level.scroll_speed = 0.0f;
    level.notes = bfbmp_vector_note_create();
    return level;
}


bfbmp_level_t bfbmp_level_create_with_name(const char* name)
{
    bfbmp_level_t level;
    const size_t str_size = strlen(name);
    level.name = name ? (char*)malloc(str_size) : NULL;
    memcpy(level.name, name, str_size);
    level.scroll_speed = 0.0f;
    level.notes = bfbmp_vector_note_create();
    return level;
}


uint8_t bfbmp_level_free(bfbmp_level_t* level)
{
    BFBMP_CHECK(level);
    BFBMP_CHECK(level->name);
    free(level->name);
    return BFBMP_TRUE;
}
