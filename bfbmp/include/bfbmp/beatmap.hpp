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

        beatmap(const beatmap&) = delete;
        beatmap(beatmap&&) = delete;
        

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

        bool load_image(const std::string& filepath)
        {
            return bfbmp_beatmap_load_image(&m_beatmap, filepath.c_str());
        }

        bool load_sound(const std::string& filepath)
        {
            return bfbmp_beatmap_load_sound(&m_beatmap, filepath.c_str());
        }

        void set_metadata(const metadata& metadata)
        {
            m_beatmap.metadata = *metadata.native();
        }

        metadata get_metadata() const
        {
            return {&m_beatmap.metadata};
        }

        void set_song_name(const std::string& song_name)
        {
            bfbmp_beatmap_set_song_name(&m_beatmap, song_name.c_str());
        }

        void set_sub_name(const std::string& sub_name)
        {
            bfbmp_beatmap_set_sub_name(&m_beatmap, sub_name.c_str());
        }

        void set_mapper_name(const std::string& mapper_name)
        {
            bfbmp_beatmap_set_mapper_name(&m_beatmap, mapper_name.c_str());
        }

        void set_author_name(const std::string& author_name)
        {
            bfbmp_beatmap_set_author_name(&m_beatmap, author_name.c_str());
        }

        void set_bpm(float bpm)
        {
            m_beatmap.metadata.beats_per_minute = bpm;
        }

        void set_beats_per_measure(uint8_t beats_per_measure)
        {
            m_beatmap.metadata.beats_per_measure = beats_per_measure;
        }

        void set_start_offset(float offset)
        {
            m_beatmap.metadata.start_offset = offset;
        }
        
        std::string get_song_name() const
        {
            return m_beatmap.metadata.song_name;
        }

        std::string get_sub_name() const
        {
            return m_beatmap.metadata.sub_name;
        }

        std::string get_author_name() const
        {
            return m_beatmap.metadata.author_name;
        }

        std::string get_mapper_name() const
        {
            return m_beatmap.metadata.mapper_name;
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
