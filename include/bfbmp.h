#ifndef BFBMP_HEADER
#define BFBMP_HEADER

#define BFBMP_VERSION_MAJOR 0
#define BFBMP_VERSION_MINOR 4


#if defined(BFBMP_BUILD_DLL)
#if defined(_MSC_VER)
#define BFBMP_DLL_EXPORT _declspec(dllexport)
#define BFBMP_DLL_IMPORT _declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define BFBMP_DLL_EXPORT __attribute__ ((visibility ("default")))
#define BFBMP_DLL_IMPORT __attribute__ ((visibility ("default")))
#else
#define BFBMP_DLL_EXPORT
#define BFBMP_DLL_IMPORT
#endif

#if defined(BFBMP_EXPORT)
#define BFBMP_API BFBMP_DLL_EXPORT
#else
#define BFBMP_API BFBMP_DLL_IMPORT
#endif
#else
#define BFBMP_API extern
#endif





#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using bfbmp_char = char;
using bfbmp_int8 = int8_t;
using bfbmp_int16 = int16_t;
using bfbmp_int32 = int32_t;
using bfbmp_uint8 = uint8_t;
using bfbmp_uint16 = uint16_t;
using bfbmp_uint32 = uint32_t;
using bfbmp_size = uint32_t;
using bfbmp_position = uint32_t;
using bfbmp_offset = uint32_t;
using bfbmp_string = std::string;
using bfbmp_float32 = float;
using bfbmp_float64 = double;

static std::vector<bfbmp_string> sound_formats { ".wav", ".mp3", ".flac", ".ogg" };
static std::vector<bfbmp_string> image_formats { ".png", ".jpg", ".bmp" };

struct bfbmp_version
{
    bfbmp_uint16 major = BFBMP_VERSION_MAJOR;
    bfbmp_uint16 minor = BFBMP_VERSION_MINOR;
};

struct bfbmp_header
{
    bfbmp_char id[4] = { 'F', 'B', 'M', 'P' };
    bfbmp_version version;
    bfbmp_size chunk_size = 32;
    bfbmp_size total_size = 0;
    bfbmp_position metadata_position = 0;
    bfbmp_position image_position = 0;
    bfbmp_position sound_position = 0;
    bfbmp_position game_position = 0;
};

struct bfbmp_metadata
{
    bfbmp_char id[4] = { 'M', 'E', 'T', 'A' };
    bfbmp_uint32 chunk_size = 0;
    bfbmp_string song_name = "None";
    bfbmp_string sub_name = "None";
    bfbmp_string author_name = "None";
    bfbmp_string mapper_name = "None";
    bfbmp_float32 beats_per_minute = 0;
    bfbmp_uint8 beats_per_measure = 0;
    bfbmp_float32 start_offset = 0.0f;
};

enum bfbmp_note_type
{
    bfbmp_note_normal,
    bfbmp_note_long
};

struct bfbmp_note
{
    bfbmp_note_type type;
    bfbmp_float32 position;
    bfbmp_float32 duration_in_beats;
};

struct bfbmp_level
{
    bfbmp_float32 speed;
    bfbmp_string name;
    std::vector<bfbmp_note> notes;
};

struct bfbmp
{
    bfbmp_header header;
    bfbmp_metadata metadata;
    std::vector<bfbmp_uint8> image_data;
    std::vector<bfbmp_uint8> sound_data;
    std::vector<bfbmp_level> game_data;
};

static bool bfbmp_extcmp(const bfbmp_string& str, const bfbmp_string& cmp)
{
    const bfbmp_position extension_position = (bfbmp_size)str.find_last_of('.');
    const bfbmp_string extension(str.data() + extension_position, str.data() + str.size());
    return extension == cmp;
}

static bool bfbmp_id_check(const bfbmp_char id[4], const bfbmp_string& cmp)
{
    bfbmp_string str(id, id + 4);
    str[4] = 0;
    return str == cmp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// PUBLIC API
////////////////////////////////////////////////////////////////////////////////////////////////////
bool BFBMP_API bfbmp_read_header(const bfbmp_string& filepath, bfbmp_header& header);
bool BFBMP_API bfbmp_read_header(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp_header& header);
bool BFBMP_API bfbmp_read_metadata(const bfbmp_string& filepath, bfbmp_metadata& metadata);
bool BFBMP_API bfbmp_read_metadata(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp_metadata& metadata);
bool BFBMP_API bfbmp_read_image(const bfbmp_string& filepath, bfbmp& bfbmp);
bool BFBMP_API bfbmp_read_image(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp);
bool BFBMP_API bfbmp_read_sound(const bfbmp_string& filepath, bfbmp& bfbmp);
bool BFBMP_API bfbmp_read_sound(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp);
bool BFBMP_API bfbmp_load_sound(const bfbmp_string& filepath, bfbmp& bfbmp);
bool BFBMP_API bfbmp_load_sound(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp);
bool BFBMP_API bfbmp_load_image(const bfbmp_string& filepath, bfbmp& bfbmp);
bool BFBMP_API bfbmp_load_image(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp);
bool BFBMP_API bfbmp_encode(const bfbmp_string& filepath, const bfbmp& bfbmp);
bool BFBMP_API bfbmp_encode(const bfbmp_string& filepath, const bfbmp_string& image_path, const bfbmp_string& sound_path, bfbmp& bfbmp);
bool BFBMP_API bfbmp_decode(const bfbmp_string& filepath, bfbmp& bfbmp);

#if defined(BFBMP_IMPLEMENTATION)
bool bfbmp_read_header(const bfbmp_string& filepath, bfbmp_header& header)
{
    std::ifstream stream(filepath, std::ios::in | std::ios::binary);
    if(!stream.is_open()) return false;
    stream.read((bfbmp_char*)&header, sizeof(bfbmp_header));
    stream.close();
    if(!bfbmp_id_check(header.id, "FBMP")) return false;
    if(header.chunk_size == 0) return false;
    return true;
}
bool bfbmp_read_header(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp_header& header)
{
    if(!buffer) return false;
    if(!(buffer + buffer_size > buffer + sizeof(bfbmp_header))) return false;
    const bfbmp_string buff(buffer, buffer + sizeof(bfbmp_header));
    std::stringstream ss(buff, std::ios::in | std::ios::binary);
    ss.read((bfbmp_char*)&header, sizeof(bfbmp_header));
    ss.clear();
    if(!bfbmp_id_check(header.id, "FBMP")) return false;
    if(header.chunk_size == 0) return false;
    return true;
}
bool bfbmp_read_metadata(const bfbmp_string& filepath, bfbmp_metadata& metadata)
{
    std::ifstream stream(filepath, std::ios::in | std::ios::binary);
    if(!stream.is_open()) return false;
    bfbmp_header header;
    if(!bfbmp_read_header(filepath, header)) return false;
    if(header.metadata_position == 0) return false;
    
    stream.seekg(header.metadata_position, std::ios::beg);
    stream.read(metadata.id, 4);
    if(!bfbmp_id_check(metadata.id, "META")) return false;

    stream.read((bfbmp_char*)&metadata.chunk_size, sizeof(bfbmp_uint32));
    if(metadata.chunk_size == 0) return false;
    
    bfbmp_size length = 0;
    stream.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.song_name.resize(length);
    stream.read(&metadata.song_name[0], length);

    stream.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.sub_name.resize(length);
    stream.read(&metadata.sub_name[0], length);

    stream.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.author_name.resize(length);
    stream.read(&metadata.author_name[0], length);

    stream.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.mapper_name.resize(length);
    stream.read(&metadata.mapper_name[0], length);

    stream.read((bfbmp_char*)&metadata.beats_per_minute, sizeof(bfbmp_float32));
    stream.read((bfbmp_char*)&metadata.beats_per_measure, sizeof(bfbmp_uint8));
    stream.read((bfbmp_char*)&metadata.start_offset, sizeof(bfbmp_float32));
    return true;
}
bool bfbmp_read_metadata(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp_metadata& metadata)
{
    if(!buffer) return false;
    bfbmp_header header;
    if(!bfbmp_read_header(buffer, buffer_size, header)) return false;
    if(header.metadata_position == 0) return false;
    
    std::stringstream ss({ buffer, buffer + buffer_size }, std::ios::in | std::ios::binary);
    
    ss.seekg(header.metadata_position, std::ios::beg);
    ss.read(metadata.id, 4);
    if(!bfbmp_id_check(metadata.id, "META")) return false;

    ss.read((bfbmp_char*)&metadata.chunk_size, sizeof(bfbmp_uint32));
    if(metadata.chunk_size == 0) return false;
    
    bfbmp_size length = 0;
    ss.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.song_name.resize(length);
    ss.read(&metadata.song_name[0], length);

    ss.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.sub_name.resize(length);
    ss.read(&metadata.sub_name[0], length);

    ss.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.author_name.resize(length);
    ss.read(&metadata.author_name[0], length);

    ss.read((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    metadata.mapper_name.resize(length);
    ss.read(&metadata.mapper_name[0], length);

    ss.read((bfbmp_char*)&metadata.beats_per_minute, sizeof(bfbmp_float32));
    ss.read((bfbmp_char*)&metadata.beats_per_measure, sizeof(bfbmp_uint8));
    ss.read((bfbmp_char*)&metadata.start_offset, sizeof(bfbmp_float32));
    return true;
}
bool bfbmp_read_image(const bfbmp_string& filepath, bfbmp& bfbmp)
{
    if(bfbmp.header.image_position == 0) return false;
    std::ifstream stream(filepath, std::ios::in | std::ios::binary);
    if(!stream.is_open()) return false;
    stream.seekg(bfbmp.header.image_position, std::ios::beg);
    bfbmp_char img_id[4];
    stream.read(img_id, 4);
    if(!bfbmp_id_check(img_id, "!IMG")) return false;
    bfbmp_size chunk_size;
    stream.read((bfbmp_char*)&chunk_size, sizeof(bfbmp_size));
    if(chunk_size == 0) return false;
    bfbmp.image_data.resize(chunk_size);
    stream.read((bfbmp_char*)&bfbmp.image_data[0], chunk_size);
    if(bfbmp.image_data.empty()) return false;
    return true;
}
bool bfbmp_read_image(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp)
{
    if(!buffer) return false;
    if(buffer_size == 0) return false;
    if(bfbmp.header.image_position == 0) return false;
    std::stringstream ss({ buffer, buffer + buffer_size }, std::ios::in | std::ios::binary);
    
    ss.seekg(bfbmp.header.image_position, std::ios::beg);
    bfbmp_char img_id[4];
    ss.read(img_id, 4);
    if(!bfbmp_id_check(img_id, "!IMG")) return false;
    bfbmp_size chunk_size;
    ss.read((bfbmp_char*)&chunk_size, sizeof(bfbmp_char));
    if(chunk_size == 0) return false;
    bfbmp.image_data.resize(chunk_size);
    ss.read((bfbmp_char*)&bfbmp.image_data[0], chunk_size);
    if(bfbmp.image_data.empty()) return false;
    return true;
}
bool bfbmp_read_sound(const bfbmp_string& filepath, bfbmp& bfbmp)
{
    if(bfbmp.header.sound_position == 0) return false;
    std::ifstream stream(filepath, std::ios::in | std::ios::binary);
    if(!stream.is_open()) return false;
    stream.seekg(bfbmp.header.sound_position, std::ios::beg);
    bfbmp_char snd_id[4];
    stream.read(snd_id, 4);
    if(!bfbmp_id_check(snd_id, "!SND")) return false;
    bfbmp_size chunk_size;
    stream.read((bfbmp_char*)&chunk_size, sizeof(bfbmp_size));
    if(chunk_size == 0) return false;
    bfbmp.sound_data.resize(chunk_size);
    stream.read((bfbmp_char*)&bfbmp.sound_data[0], chunk_size);
    if(bfbmp.sound_data.empty()) return false;
    return true;
}
bool bfbmp_read_sound(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp)
{
    if(!buffer) return false;
    if(buffer_size == 0) return false;
    if(bfbmp.header.sound_position == 0) return false;
    std::stringstream ss({ buffer, buffer + buffer_size }, std::ios::in | std::ios::binary);
    
    ss.seekg(bfbmp.header.sound_position, std::ios::beg);
    bfbmp_char snd_id[4];
    ss.read(snd_id, 4);
    if(!bfbmp_id_check(snd_id, "!SND")) return false;
    bfbmp_size chunk_size;
    ss.read((bfbmp_char*)&chunk_size, sizeof(bfbmp_char));
    if(chunk_size == 0) return false;
    bfbmp.sound_data.resize(chunk_size);
    ss.read((bfbmp_char*)&bfbmp.sound_data[0], chunk_size);
    if(bfbmp.sound_data.empty()) return false;
    return true;
}
bool bfbmp_load_sound(const bfbmp_string& filepath, bfbmp& bfbmp)
{
    {
        const bfbmp_position extension_position = filepath.find_last_of('.');
        const bfbmp_string extension(filepath.data() + extension_position, filepath.data() + filepath.size());
        bfbmp_int32 index = -1;
        for(bfbmp_uint32 i = 0; i < sound_formats.size(); i++)
            if(sound_formats[i] == extension)
            {
                index = i;
                break;
            }
        if(index == -1) return false;
    }
    
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if(!file.is_open()) return false;
    bfbmp.sound_data = std::vector<bfbmp_uint8>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
    if(bfbmp.sound_data.empty()) return false;
    return true;
}
bool bfbmp_load_sound(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp)
{
    if(!(buffer || buffer_size > 0)) return false;
    bfbmp.sound_data = std::vector<bfbmp_uint8>(buffer, buffer + buffer_size);
    if(bfbmp.sound_data.empty()) return false;
    return true;
}
bool bfbmp_load_image(const bfbmp_string& filepath, bfbmp& bfbmp)
{
    {
        bfbmp_int32 index = -1;
        for(bfbmp_uint32 i = 0; i < image_formats.size(); i++)
            if(bfbmp_extcmp(filepath, image_formats[i]))
            {
                index = i;
                break;
            }
        if(index == -1) return false;
    }
    
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if(!file.is_open()) return false;
    bfbmp.image_data = std::vector<bfbmp_uint8>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
    if(bfbmp.sound_data.empty()) return false;
    return true;
}
bool bfbmp_load_image(const bfbmp_uint8* buffer, bfbmp_size buffer_size, bfbmp& bfbmp)
{
    if(!buffer) return false;
    bfbmp.image_data = std::vector<bfbmp_uint8>(buffer, buffer + buffer_size);
    if(bfbmp.image_data.empty()) return false;
    return true;
}
bool bfbmp_encode(const bfbmp_string& filepath, const bfbmp& bfbmp)
{
    bfbmp_string filename = filepath;
    if(bfbmp.image_data.empty()) return false;
    if(bfbmp.sound_data.empty()) return false;
    if(!bfbmp_extcmp(filename, ".bfbmp"))
        filename += ".bfbmp";
    
    std::ofstream stream(filename, std::ios::out | std::ios::binary);
    if(!stream.is_open()) return false;

    const bfbmp_position begin_hd = stream.tellp();
    stream.write((bfbmp_char*)&bfbmp.header, sizeof(bfbmp_header));
    const bfbmp_position end_hd = stream.tellp();
    bfbmp_size length = 0;

    const bfbmp_position begin_meta = stream.tellp();
    stream.write(bfbmp.metadata.id, 4);
    stream.write("____", 4);
    length = bfbmp.metadata.song_name.size() + 1;
    stream.write((bfbmp_char*)&length, 4);
    stream.write(bfbmp.metadata.song_name.data(), length);

    length = bfbmp.metadata.sub_name.size() + 1;
    stream.write((bfbmp_char*)&length, 4);
    stream.write(bfbmp.metadata.sub_name.data(), length);

    length = bfbmp.metadata.author_name.size() + 1;
    stream.write((bfbmp_char*)&length, 4);
    stream.write(bfbmp.metadata.author_name.data(), length);

    length = bfbmp.metadata.mapper_name.size() + 1;
    stream.write((bfbmp_char*)&length, 4);
    stream.write(bfbmp.metadata.mapper_name.data(), length);

    stream.write((const bfbmp_char*)&bfbmp.metadata.beats_per_minute, sizeof(bfbmp_float32));
    stream.write((const bfbmp_char*)&bfbmp.metadata.beats_per_measure, sizeof(bfbmp_uint32));
    stream.write((const bfbmp_char*)&bfbmp.metadata.start_offset, sizeof(bfbmp_float32));
    const bfbmp_position end_meta = stream.tellp();

    const bfbmp_position begin_img = stream.tellp();
    stream.write("!IMG", 4);
    length = bfbmp.image_data.size();
    stream.write((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    stream.write((const bfbmp_char*)&bfbmp.image_data[0], length);
    const bfbmp_position end_img = stream.tellp();

    const bfbmp_position begin_snd = stream.tellp();
    stream.write("!SND", 4);
    length = bfbmp.sound_data.size();
    stream.write((bfbmp_char*)&length, sizeof(bfbmp_uint32));
    stream.write((const bfbmp_char*)&bfbmp.sound_data[0], length);
    const bfbmp_position end_snd = stream.tellp();



    stream.seekp(offsetof(bfbmp_header, metadata_position), std::ios::beg);
    stream.write((const bfbmp_char*)&begin_meta, sizeof(bfbmp_position));

    stream.seekp(offsetof(bfbmp_header, image_position), std::ios::beg);
    stream.write((const bfbmp_char*)&begin_img, sizeof(bfbmp_position));

    stream.seekp(offsetof(bfbmp_header, sound_position), std::ios::beg);
    stream.write((const bfbmp_char*)&begin_snd, sizeof(bfbmp_position));
    
    
    stream.seekp(offsetof(bfbmp_header, total_size), std::ios::beg);
    const bfbmp_size total_size = end_snd;
    stream.write((const bfbmp_char*)&total_size, sizeof(bfbmp_size));

    stream.seekp(begin_meta + 4, std::ios::beg);
    const bfbmp_size meta_size = end_meta - begin_meta;
    stream.write((const bfbmp_char*)&meta_size, sizeof(bfbmp_size));
    
    return true;
}
bool bfbmp_encode(const bfbmp_string& filepath, const bfbmp_string& image_path, const bfbmp_string& sound_path, bfbmp& bfbmp)   
{
    if(!(filepath.empty() || image_path.empty() || sound_path.empty())) return false;
    
    bfbmp_load_image(image_path, bfbmp);
    bfbmp_load_sound(sound_path, bfbmp);
    bfbmp_encode(filepath, bfbmp);
    return true;
}
bool bfbmp_decode(const bfbmp_string& filepath, bfbmp& bfbmp)
{
    if(!bfbmp_read_header(filepath, bfbmp.header)) return false;
    if(!bfbmp_read_metadata(filepath, bfbmp.metadata)) return false;
    if(!bfbmp_read_image(filepath, bfbmp)) return false;
    if(!bfbmp_read_sound(filepath, bfbmp)) return false;
    return true;
}
#endif
#endif