#pragma once
#include "def.h"

typedef enum
{
    bfbmp_note_normal,
    bfbmp_note_long
} bfbmp_note_type;

typedef struct __bfbmp_note
{
    bfbmp_note_type type;
    float position;
    float duration;
} bfbmp_note_t;


BFBMP_PUBLIC bfbmp_note_t BFBMP_API bfbmp_note_create(void);
