#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
#include <iostream>
#include <tinyfiledialogs.h>
#include <filesystem>

int main(int argc, const char** argv, const char** envp)
{
    const std::filesystem::path filepath = tinyfd_openFileDialog(
    "Please select a bfbmp file",
    nullptr,
    0,
    nullptr,
    "Binary Format Beatmap",
    false);

    bfbmp bfbmp;
    if(!bfbmp_decode(filepath.string(), bfbmp))
    {
        std::cerr << "Failed to read bfbmp file!\n";
        return -1;
    }

    std::cout << "*** BINARY FORMAT BEATMAP ***\n";
    std::cout << "File size: " << bfbmp.header.total_size << '\n';
    std::cout << "** METADATA **\n";
    std::cout << "Song Name: "  << bfbmp.metadata.song_name << '\n';
    std::cout << "Sub Name: "  << bfbmp.metadata.sub_name << '\n';
    std::cout << "Author Name: "  << bfbmp.metadata.author_name << '\n';
    std::cout << "Mapper Name: "  << bfbmp.metadata.mapper_name << '\n';
    std::cout << "BPM: " << bfbmp.metadata.beats_per_minute << '\n';
    std::cout << "Beats Per Measure: " << (bfbmp_uint32)bfbmp.metadata.beats_per_measure << '\n';
    std::cout << "Start offset: " << bfbmp.metadata.start_offset << '\n';
    std::cout << "** IMAGE **\n";
    std::cout << "Size: " << bfbmp.image_data.size() << '\n';
    std::cout << "** SOUND **\n";
    std::cout << "Size: " << bfbmp.sound_data.size() << '\n';
    
    
    return 0;
}