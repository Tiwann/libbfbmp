#pragma once
#include <filesystem>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "config.h"
#include "header.h"
#include "metadata.h"
#include "level.h"
#include "memstream.h"

BFBMP_BEGIN
static std::vector<std::string> sound_extensions { ".wav", ".mp3", ".flac", ".ogg" };
static std::vector<std::string> sound_ids        { "RIFF", "ID3", "fLaC", "OggS" };
static std::vector<std::string> image_extensions { ".png", ".jpg" };
static std::vector<std::string> image_ids        { { (char)0x89, (char)0x50, (char)0x4e, (char)0x47 } , { (char)0xff, (char)0xd8, (char)0xff, (char)0xe0 } };
class BFBMP_API beatmap
{
public:
    header               header;
    metadata             metadata;
    std::vector<uint8_t> image_data;
    std::vector<uint8_t> sound_data;
    std::vector<level>   game_data;

public:
    bool load_image(const std::filesystem::path& filepath);
    bool load_image(uint8_t* buffer, size_t size);
    bool load_sound(const std::filesystem::path& filepath);
    bool load_sound(uint8_t* buffer, size_t size);
    bool encode(std::vector<uint8_t>& buffer);
    bool encode(const std::filesystem::path& filepath);
    bool decode(const std::filesystem::path& filepath);
    bool decode(uint8_t* buffer, size_t size);
    bool decode(const std::vector<uint8_t>& buffer);
private:
    static bool check_exts(const std::filesystem::path& filepath, const std::vector<std::string>& exts);
    static bool check_magics(void* data, const std::vector<std::string>& magics);
    bool read_metadata(memstream& stream);
    bool read_image(memstream& stream);
    bool read_sound(memstream& stream);
    bool read_game(memstream& stream);
};
BFBMP_END