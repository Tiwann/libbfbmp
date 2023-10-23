#include "bfbmp/note.h"

bfbmp_note_t bfbmp_note_create(void)
{
    bfbmp_note_t note;
    note.type = bfbmp_note_normal;
    note.position = 0.0f;
    note.duration = 0.0f;
    return note;
}
