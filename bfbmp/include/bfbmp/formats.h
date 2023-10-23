#pragma once
#include "def.h"

const uint8_t* const bfbmp_g_sound_extensions[4] = { BFBMP_2UC(".wav"), BFBMP_2UC(".mp3"), BFBMP_2UC(".flac"), BFBMP_2UC(".ogg") };
const uint8_t* const        bfbmp_g_sound_ids[4] = { BFBMP_2UC("RIFF"), BFBMP_2UC("ID3\x3"), BFBMP_2UC("fLaC"), BFBMP_2UC("OggS") };
const uint8_t* const bfbmp_g_image_extensions[2] = { BFBMP_2UC(".png"), BFBMP_2UC(".jpg") };
const uint8_t            bfbmp_g_image_ids[2][4] = { { 0x89, 0x50, 0x4e, 0x47 } , { 0xff, 0xd8, 0xff, 0xe0 } };

BFBMP_PUBLIC uint8_t BFBMP_API bfbmp_is_sound_extension_valid(const char* filepath); 
BFBMP_PUBLIC uint8_t BFBMP_API bfbmp_is_image_extension_valid(const char* filepath); 
BFBMP_PUBLIC uint8_t BFBMP_API bfbmp_is_sound_id_valid(const void* buff); 
BFBMP_PUBLIC uint8_t BFBMP_API bfbmp_is_image_id_valid(const void* buff);