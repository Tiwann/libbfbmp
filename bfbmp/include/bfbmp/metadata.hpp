#pragma once
#include "bfbmp/metadata.h"
#include "memstream.hpp"
#include "header.hpp"
#include <string>

namespace bfbmp
{
    template<class T>
    class metadata_base
    {
    public:
        metadata_base() : m_metadata(bfbmp_metadata_create())
        {
            
        }

        metadata_base(const std::string& song_name, const std::string& sub_name, const std::string& author_name, const std::string& mapper_name)
            : m_metadata(bfbmp_metadata_create_with_names(song_name.c_str(), sub_name.c_str(), author_name.c_str(), mapper_name.c_str()))
        {
            
        }

        ~metadata_base()
        {
            bfbmp_metadata_free(&m_metadata);
        }

        void set_song_name(const std::string& song_name)
        {
            bfbmp_metadata_set_song_name(&m_metadata, song_name.c_str());
        }

        void set_sub_name(const std::string& sub_name)
        {
            bfbmp_metadata_set_sub_name(&m_metadata, sub_name.c_str());
        }

        void set_author_name(const std::string& author_name)
        {
            bfbmp_metadata_set_author_name(&m_metadata, author_name.c_str());
        }

        void set_mapper_name(const std::string& mapper_name)
        {
            bfbmp_metadata_set_mapper_name(&m_metadata, mapper_name.c_str());
        }

        void set_bpm(float bpm)
        {
            m_metadata.beats_per_minute = bpm;
        }

        void set_beat_per_measure(uint8_t beats_per_measure)
        {
            m_metadata.beats_per_measure = beats_per_measure;
        }

        void set_start_offset(float start_offset)
        {
            m_metadata.start_offset = start_offset;
        }

        std::string get_song_name() const
        {
            return m_metadata.song_name;
        }

        std::string get_sub_name() const
        {
            return m_metadata.sub_name;
        }

        std::string get_author_name() const
        {
            return m_metadata.author_name;
        }

        std::string get_mapper_name() const
        {
            return m_metadata.mapper_name;
        }

        auto get_bpm() const
        {
            return m_metadata.beats_per_minute;
        }

        auto get_beats_per_measure() const
        {
            return m_metadata.beats_per_measure;
        }
        
        bool read(memstream_base<T>& memstream, const header_base<T>& header)
        {
            return bfbmp_metadata_read(memstream.native(), header.native(), &m_metadata);
        }

        
        operator bfbmp_metadata_t*()
        {
            return &m_metadata;
        }

        operator const bfbmp_metadata_t*() const
        {
            return &m_metadata;
        }

        bfbmp_metadata_t* native()
        {
            return &m_metadata;
        }

        const bfbmp_metadata_t* native() const
        {
            return &m_metadata;
        }
    private:
        bfbmp_metadata_t m_metadata;
    };

    using metadata = metadata_base<char>;
}