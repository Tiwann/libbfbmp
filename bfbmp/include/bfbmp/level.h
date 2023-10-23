#pragma once
#include "def.h"
#include "vector.h"

typedef struct __bfbmp_note bfbmp_note_t;
BFBMP_DECLARE_TEMPLATE_VECTOR_STRUCT(bfbmp_note_t, note)

typedef struct __bfbmp_level
{
    char*               name;
    float               scroll_speed;
    bfbmp_vector_note_t notes;
} bfbmp_level_t;

BFBMP_PUBLIC bfbmp_level_t BFBMP_API bfbmp_level_create(void);
BFBMP_PUBLIC bfbmp_level_t BFBMP_API bfbmp_level_create_with_name(const char* name);
BFBMP_PUBLIC uint8_t       BFBMP_API bfbmp_level_free(bfbmp_level_t* level);