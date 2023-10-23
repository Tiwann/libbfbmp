#pragma once
#include "bfbmp/memstream.h"
#include <vector>
#include <string>

namespace bfbmp
{
    template<class T>
    class memstream_base
    {
    public:
        memstream_base(std::vector<T>& data, int8_t mode)
        {
            bfbmp_memstream_open(&m_memstream, data.data(), data.size(), mode);
        }
        
        memstream_base(std::basic_string<T>& string, int8_t mode)
        {
            bfbmp_memstream_open(&m_memstream, string.data(), string.size(), mode);
        }

        memstream_base(T* buffer, size_t buffer_size, int8_t mode)
        {
            bfbmp_memstream_open(&m_memstream, buffer, buffer_size, mode);
        }

        bool read(T* buffer, size_t buffer_size)
        {
            return bfbmp_memstream_read(&m_memstream, buffer, 1, buffer_size);
        }

        bool read(T* buffer, size_t element_count, size_t element_size)
        {
            return bfbmp_memstream_read(&m_memstream, buffer, element_count, element_size);
        }

        std::string read_string(size_t string_size)
        {
            std::string result;
            result.resize(string_size);
            bfbmp_memstream_read(&m_memstream, &result[0], 1, string_size);
            return result;
        }

        int32_t read_int()
        {
            int32_t result;
            bfbmp_memstream_read(&m_memstream, &result, 1, sizeof(int32_t));
            return result;
        }

        uint32_t read_uint()
        {
            uint32_t result;
            bfbmp_memstream_read(&m_memstream, &result, 1, sizeof(uint32_t));
            return result;
        }

        int16_t read_short()
        {
            int16_t result;
            bfbmp_memstream_read(&m_memstream, &result, 1, sizeof(int16_t));
            return result;
        }

        uint16_t read_ushort()
        {
            uint16_t result;
            bfbmp_memstream_read(&m_memstream, &result, 1, sizeof(uint16_t));
            return result;
        }

        T read_byte()
        {
            T result;
            bfbmp_memstream_read(&m_memstream, &result, 1, sizeof(T));
            return result;
        }

        bool write(const T* buffer, size_t size)
        {
            return bfbmp_memstream_write(&m_memstream, buffer, size);
        }

        bool write(T character)
        {
            return bfbmp_memstream_write(&m_memstream, &character, 1);
        }

        bool seek(int64_t offset, bfbmp_seek seek)
        {
            return bfbmp_memstream_seek(&m_memstream, offset, seek);
        }

        uint64_t tell() const
        {
            return bfbmp_memstream_tell(&m_memstream);
        }

        bool is_opened() const
        {
            return bfbmp_memstream_is_opened(&m_memstream);
        }
        
        bool close()
        {
            if(!bfbmp_memstream_is_opened(&m_memstream))
                return BFBMP_FALSE;
            return bfbmp_memstream_close(&m_memstream);
        }

        operator bfbmp_memstream_t*()
        {
            return &m_memstream;
        }

        operator const bfbmp_memstream_t*() const
        {
            return &m_memstream;
        }

        bfbmp_memstream_t* native()
        {
            return &m_memstream;
        }

        const bfbmp_memstream_t* native() const
        {
            return &m_memstream;
        }

        ~memstream_base()
        {
            bfbmp_memstream_close(&m_memstream);
        }

        
    private:
        bfbmp_memstream_t m_memstream;
    };

    using memstream = memstream_base<char>;
}

