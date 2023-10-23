#pragma once
#include "bfbmp/header.h"
#include "memstream.hpp"

namespace bfbmp
{
    template<class T>
    class header_base
    {
    public:
        header_base() : m_header(bfbmp_header_create())
        {
            
        }
        
        bool read(memstream_base<T>& memstream)
        {
            return bfbmp_header_read(memstream.native(), &m_header);
        }


        operator bfbmp_header_t*()
        {
            return &m_header;
        }

        operator const bfbmp_header_t*() const
        {
            return &m_header;
        }

        bfbmp_header_t* native()
        {
            return &m_header;
        }

        const bfbmp_header_t* native() const
        {
            return &m_header;
        }
    private:
        bfbmp_header_t m_header;
    };

    using header = header_base<char>;
}
