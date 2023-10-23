#pragma once
#include "bfbmp/beatmap.h"
#include <string>


namespace bfbmp
{
    class beatmap
    {
    public:
        beatmap() : m_beatmap(bfbmp_beatmap_create())
        {
            
        }

        ~beatmap()
        {
            bfbmp_beatmap_free(&m_beatmap);
        }

        bool decode(uint8_t* buffer, size_t size)
        {
            return bfbmp_beatmap_decode_memory(&m_beatmap, buffer, size);
        }

        bool decode(const std::string& filepath)
        {
            return bfbmp_beatmap_decode_file(&m_beatmap, filepath.c_str());
        }

        bool encode(const std::string& filepath, bool compress = false) const
        {
            return bfbmp_beatmap_encode_file(&m_beatmap, filepath.c_str(), compress);
        }

        bool validate() const
        {
            return bfbmp_beatmap_validate(&m_beatmap);
        }

        bfbmp_beatmap_t* native()
        {
            return &m_beatmap;
        }

        const bfbmp_beatmap_t* native() const
        {
            return &m_beatmap;
        }
    
    private:
        bfbmp_beatmap_t m_beatmap;
    };
}
