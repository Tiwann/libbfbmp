#include "bfbmp/formats.h"
#include <string.h>

static uint8_t str_ends_with(const char* str, const char* suffix)
{
    const size_t str_length = strlen(str);
    const size_t suffix_length = strlen(suffix);
    return str_length < suffix_length ? BFBMP_FALSE : !strcmp(str + (str_length - suffix_length), suffix);
}

uint8_t bfbmp_is_sound_extension_valid(const char* filepath)
{
    uint8_t result = BFBMP_FALSE;
    for(uint32_t i = 0; i < 4; i++)
    {
        result = str_ends_with(filepath, BFBMP_2C(bfbmp_g_sound_extensions[i]));
        if(result == BFBMP_TRUE) break;
    }
    return result;
}

uint8_t bfbmp_is_image_extension_valid(const char* filepath)
{
    uint8_t result = BFBMP_FALSE;
    for(uint32_t i = 0; i < 4; i++)
    {
        result = str_ends_with(filepath, BFBMP_2C(bfbmp_g_image_extensions[i]));
        if(result == BFBMP_TRUE) break;
    }
    return result;
}

uint8_t bfbmp_is_sound_id_valid(const void* buff)
{
    uint8_t result = BFBMP_FALSE;
    for(uint32_t i = 0; i < 4; i++)
    {
        result = (BFBMP_2UC(buff))[0] == bfbmp_g_sound_ids[i][0]
              && (BFBMP_2UC(buff))[1] == bfbmp_g_sound_ids[i][1]
              && (BFBMP_2UC(buff))[2] == bfbmp_g_sound_ids[i][2]
              && (BFBMP_2UC(buff))[3] == bfbmp_g_sound_ids[i][3];
        if(result == BFBMP_TRUE) break;
    }
    return result;
}

uint8_t bfbmp_is_image_id_valid(const void* buff)
{
    uint8_t result = BFBMP_FALSE;
    for(uint32_t i = 0; i < 2; i++)
    {
        result = (BFBMP_2UC(buff))[0] == bfbmp_g_image_ids[i][0]
              && (BFBMP_2UC(buff))[1] == bfbmp_g_image_ids[i][1]
              && (BFBMP_2UC(buff))[2] == bfbmp_g_image_ids[i][2]
              && (BFBMP_2UC(buff))[3] == bfbmp_g_image_ids[i][3];
        if(result == BFBMP_TRUE) break;
    }
    return result;
}
