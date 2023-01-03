#pragma once
#include "bfbmp/config.h"
#include <exception>
#include <string>

BFBMP_BEGIN
class exception : public std::exception
{
public:
    BFBMP_NODISCARD char const* what() const noexcept override
    {
        return "An exception has been thrown.";
    }
};

class metadata_incomplete_exception : public exception
{
public:
    BFBMP_NODISCARD char const* what() const noexcept override
    {
        return "An exception has been thrown: Metadata structure hasn't been filled entirely.";
    }
};

class wrong_image_format_exception : public exception
{
    public:
    BFBMP_NODISCARD char const* what() const noexcept override
    {
        return "An exception has been thrown: The image format is incompatible with bfbmp file format.";
    }
};

class wrong_audio_format_exception : public exception
{
    public:
    BFBMP_NODISCARD char const* what() const noexcept override
    {
        return "An exception has been thrown: The audio format is incompatible with bfbmp file format.";
    }
};

class image_buffer_empty : public exception
{
    public:
    BFBMP_NODISCARD char const* what() const noexcept override
    {
        return "An exception has been thrown: The image buffer is empty!";
    }
};

class audio_buffer_empty : public exception
{
    public:
    BFBMP_NODISCARD char const* what() const noexcept override
    {
        return "An exception has been thrown: The audio buffer is empty!";
    }
};



BFBMP_END