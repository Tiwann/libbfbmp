#pragma once
#include "bfbmp/formats.h"
#include <string>
#include <array>


namespace bfbmp
{
    static bool is_sound_extension_valid(const std::string& filepath)
    {
        return bfbmp_is_sound_extension_valid(filepath.c_str());
    }

    static bool is_image_extension_valid(const std::string& filepath)
    {
        return bfbmp_is_image_extension_valid(filepath.c_str());
    }
    
}
