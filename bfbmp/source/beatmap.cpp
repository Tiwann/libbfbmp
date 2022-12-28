#include "bfbmp/beatmap.h"
#include "bfbmp/memstream.h"
#include "bfbmp/ofstream.h"
#include "bfbmp/ifstream.h"
#include <cstring>


BFBMP_BEGIN
bool beatmap::encode(std::vector<uint8_t>& buffer)
{
    memstream memstream(std::ios::out | std::ios::binary);
    
    const std::streamoff header_beg = memstream.tellp();
    const version version = { BFBMP_VERSION };
    memstream.write((const uint8_t*)"FBMP", 4);
    memstream.write((const uint8_t*)&version, 4);
    memstream.write((const uint8_t*)"____", 4);
    memstream.write((const uint8_t*)"____", 4);
    static constexpr uint32_t zero = 0;
    memstream.write((const uint8_t*)&zero, 4);
    memstream.write((const uint8_t*)&zero, 4);
    memstream.write((const uint8_t*)&zero, 4);
    memstream.write((const uint8_t*)&zero, 4);
    const std::streamoff header_end = memstream.tellp();

    const std::streamoff meta_beg = memstream.tellp();
    uint32_t length;
    memstream.write((const uint8_t*)"META", 4);
    memstream.write((const uint8_t*)"____", 4);
    
    length = (uint32_t)metadata.song_name.size() + 1;
    memstream.write((uint8_t*)&length, 4);
    memstream.write((const uint8_t*)metadata.song_name.data(), length);

    length = (uint32_t)metadata.sub_name.size() + 1;
    memstream.write((uint8_t*)&length, 4);
    memstream.write((const uint8_t*)metadata.sub_name.data(), length);

    length = (uint32_t)metadata.author_name.size() + 1;
    memstream.write((uint8_t*)&length, 4);
    memstream.write((const uint8_t*)metadata.author_name.data(), length);

    length = (uint32_t)metadata.mapper_name.size() + 1;
    memstream.write((uint8_t*)&length, 4);
    memstream.write((const uint8_t*)metadata.mapper_name.data(), length);

    memstream.write((uint8_t*)&metadata.beats_per_minute, 4);
    memstream.write(&metadata.beats_per_measure, 1);
    memstream.write((uint8_t*)&metadata.start_offset, 4);
    const std::streamoff meta_end = memstream.tellp();


    std::streamoff img_beg = 0;
    if(!image_data.empty())
    {
        img_beg = memstream.tellp();
        memstream.write((const uint8_t*)"!IMG", 4);
        length = (uint32_t)image_data.size();
        memstream.write((uint8_t*)&length, 4);
        memstream.write(image_data.data(), length);
    }
    const std::streamoff img_end = memstream.tellp();
    
    std::streamoff snd_beg = 0;
    if(!sound_data.empty())
    {
        snd_beg = memstream.tellp();
        memstream.write((const uint8_t*)"!SND", 4);
        length = (uint32_t)sound_data.size();
        memstream.write((uint8_t*)&length, 4);
        memstream.write(sound_data.data(), length);
    }
    const std::streamoff snd_end = memstream.tellp();

    std::streamoff data_beg = 0;
    if(!game_data.empty())
    {
        data_beg = memstream.tellp();
        memstream.write((const uint8_t*)"!GAM", 4);
        memstream.write((const uint8_t*)"____", 4);
        length = (uint32_t)game_data.size();
        memstream.write((const uint8_t*)&length, 4);
        for(const auto& level : game_data)
        {
            memstream.write((const uint8_t*)"!LVL", 4);
            length = (uint32_t)level.name.size() + 1;
            memstream.write((const uint8_t*)&length, 4);
            memstream.write((const uint8_t*)level.name.data(), length);
            memstream.write((const uint8_t*)&level.scroll_speed, 4);
            memstream.write((const uint8_t*)"!NOT", 4);
            length = (uint32_t)level.notes.size();
            memstream.write((uint8_t*)&length, 4);
            for(const auto& note : level.notes)
            {
                memstream.write((const uint8_t*)&note.type, 4);
                memstream.write((const uint8_t*)&note.position, 4);
                if(note.type == note_type::note_long)
                    memstream.write((const uint8_t*)&note.duration, 4);
            }
        }
    }
    const std::streamoff data_end = memstream.tellp();

    // chunk size
    memstream.seekp(header_beg + 8, std::ios::beg);
    const std::streamoff header_size = header_end - header_beg;
    memstream.write((const uint8_t*)&header_size, 4);

    // total size
    memstream.seekp(header_beg + 12, std::ios::beg);
    memstream.write((const uint8_t*)&data_end, 4);

    // meta chunk size
    memstream.seekp(meta_beg + 4, std::ios::beg);
    const std::streamoff meta_size = meta_end - meta_beg;
    memstream.write((const uint8_t*)&meta_size, 4);

    // data chunk size
    memstream.seekp(data_beg + 4, std::ios::beg);
    const std::streamoff data_size = data_end - data_beg;
    memstream.write((const uint8_t*)&data_size, 4);
    
    // meta chunk position
    memstream.seekp(header_beg + 16, std::ios::beg);
    memstream.write((const uint8_t*)&header_end, 4);
    
    // img chunk position
    memstream.seekp(header_beg + 20, std::ios::beg);
    memstream.write((const uint8_t*)&img_beg, 4);

    // snd chunk position
    memstream.seekp(header_beg + 24, std::ios::beg);
    memstream.write((const uint8_t*)&snd_beg, 4);

    // data chunk position
    memstream.seekp(header_beg + 28, std::ios::beg);
    memstream.write((const uint8_t*)&data_beg, 4);
    
    const auto& string = memstream.str();
    buffer = std::vector(string.begin(), string.end());
    memstream.clear();
    return !buffer.empty();
}

bool beatmap::encode(const std::filesystem::path& filepath)
{
    ofstream filestream(filepath, std::ios::binary);
    if(!filestream.is_open()) return false;
    std::vector<uint8_t> buffer;
    if(!encode(buffer)) return false;
    filestream.write(buffer.data(), buffer.size());
    filestream.close();
    return true;
}

bool beatmap::decode(const std::filesystem::path& filepath)
{
    ifstream filestream(filepath, std::ios::binary);
    if(!filestream.is_open()) return false;
    *this = beatmap();

    memstream memstream;
    memstream << filestream.rdbuf();
    filestream.close();
    
    memstream.read((uint8_t*)&header, sizeof(struct header));
    if(header.chunk_size == 0) return false;
    if(header.total_size == 0) return false;

    if(!read_metadata(memstream)) return false;
    if(!read_image(memstream)) return false;
    if(!read_sound(memstream)) return false;
    if(!read_game(memstream)) return false;
    
    return true;
}

bool beatmap::decode(uint8_t* buffer, size_t size)
{
    return false;
}

bool beatmap::decode(const std::vector<uint8_t>& buffer)
{
    return false;
}

bool beatmap::check_exts(const std::filesystem::path& filepath, const std::vector<std::string>& exts)
{
    const std::string filename_ext = filepath.extension().string();
    for(const auto& ext : exts)
    {
        if(filename_ext == ext) return true;
    }
    return false;
}

bool beatmap::check_magics(void* data, const std::vector<std::string>& magics)
{
    const std::string input((char*)data);
    for(const auto& magic : magics)
    {
        if(input == magic)
            return true;
    }
    return false;
}

bool beatmap::load_image(const std::filesystem::path& filepath)
{
    if(!check_exts(filepath, image_extensions)) return false;
    std::ifstream stream(filepath, std::ios::binary);
    if(!stream.is_open()) return false;
    stream.seekg(0, std::ios::end);
    const size_t size = stream.tellg();
    if(size == 0) return false;
    stream.seekg(0, std::ios::beg);

    image_data.resize(size);
    stream.read((char*)image_data.data(), size);
    return !image_data.empty();
}

bool beatmap::load_image(uint8_t* buffer, size_t size)
{
    image_data = { buffer, buffer + size };
    return !image_data.empty();
}

bool beatmap::load_sound(const std::filesystem::path& filepath)
{
    if(!check_exts(filepath, sound_extensions)) return false;
    std::ifstream stream(filepath, std::ios::binary);
    if(!stream.is_open()) return false;
    stream.seekg(0, std::ios::end);
    const size_t size = stream.tellg();
    if(size == 0) return false;
    stream.seekg(0, std::ios::beg);

    sound_data.resize(size);
    stream.read((char*)sound_data.data(), size);
    return !sound_data.empty();
}

bool beatmap::load_sound(uint8_t* buffer, size_t size)
{
    sound_data = { buffer, buffer + size };
    return !sound_data.empty();
}

bool beatmap::read_metadata(memstream& stream)
{
    uint32_t length;
    uint8_t magic[4];
    if(header.metadata_position == 0) return false;
    
    stream.seekg(header.metadata_position, std::ios::beg);
    stream.read(magic, 4);
    if(strncmp((char*)magic, "META", 4) != 0) return false;
    stream.read((uint8_t*)&metadata.chunk_size, 4);
        
    stream.read((uint8_t*)&length, 4);
    metadata.song_name.resize(length);
    stream.read((uint8_t*)metadata.song_name.data(), length);

    stream.read((uint8_t*)&length, 4);
    metadata.sub_name.resize(length);
    stream.read((uint8_t*)metadata.sub_name.data(), length);

    stream.read((uint8_t*)&length, 4);
    metadata.author_name.resize(length);
    stream.read((uint8_t*)metadata.author_name.data(), length);

    stream.read((uint8_t*)&length, 4);
    metadata.mapper_name.resize(length);
    stream.read((uint8_t*)metadata.mapper_name.data(), length);

    stream.read((uint8_t*)&metadata.beats_per_minute, 4);
    stream.read(&metadata.beats_per_measure, 1);
    stream.read((uint8_t*)&metadata.start_offset, 4);
    return true;
}

bool beatmap::read_image(memstream& stream)
{
    if(header.image_position == 0) return false;
    uint8_t magic[4];
    uint32_t length;
    
    stream.seekg(header.image_position, std::ios::beg);
    stream.read(magic, 4);
    if(strncmp((char*)magic, "!IMG", 4) != 0) return false;
    stream.read((uint8_t*)&length, 4);
    if(length == 0) return false;
    image_data.resize(length);
    stream.read(image_data.data(), length);
    return true;
}

bool beatmap::read_sound(memstream& stream)
{
    if(header.sound_position == 0) return false;

    uint8_t magic[4];
    uint32_t length;
    stream.seekg(header.sound_position, std::ios::beg);
    stream.read(magic, 4);
    if(strncmp((char*)magic, "!SND", 4) != 0) return false;
    stream.read((uint8_t*)&length, 4);
    if(length == 0) return false;
    sound_data.resize(length);
    stream.read(sound_data.data(), length);
    return true;
}

bool beatmap::read_game(memstream& memstream)
{
    if(header.game_position == 0) return false;

    uint8_t magic[4];
    uint32_t str_length;
    uint32_t lvl_size;
    uint32_t note_size;
    uint32_t chunk_size;

    memstream.seekg(header.game_position, std::ios::beg);
    memstream.read(magic, 4);
    if(strncmp((const char*)magic, "!GAM", 4) != 0) return false;
    memstream.read((uint8_t*)&chunk_size, 4);
    memstream.read((uint8_t*)&lvl_size, 4);

    for(size_t i = 0; i < lvl_size; i++)
    {
        level lvl;
        memstream.read(magic, 4);
        if(strncmp((const char*)magic, "!LVL", 4) != 0) break;
        memstream.read((uint8_t*)&str_length, 4);
        lvl.name.resize(str_length);
        memstream.read((uint8_t*)lvl.name.data(), str_length);
        memstream.read((uint8_t*)&lvl.scroll_speed, 4);
        memstream.read(magic, 4);
        if(strncmp((const char*)magic, "!NOT", 4) != 0) break;
        memstream.read((uint8_t*)&note_size, 4);
        for(size_t j = 0; j < note_size; j++)
        {
            note note;
            memstream.read((uint8_t*)&note.type, 4);
            memstream.read((uint8_t*)&note.position, 4);
            if(note.type == note_type::note_long)
                memstream.read((uint8_t*)&note.duration, 4);
            lvl.notes.push_back(note);
        }
        game_data.push_back(lvl);
    }
    
    return true;
}

BFBMP_END
