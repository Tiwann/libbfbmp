#include "bfbmp/beatmap.h"
#include "tinyfiledialogs.h"

int main()
{
    bfbmp::beatmap beatmap;

    beatmap.metadata.song_name = "Danganronpa";
    beatmap.metadata.sub_name = "The end of Kibogamine";
    beatmap.metadata.author_name = "binaria";
    beatmap.metadata.mapper_name = "Tiwann";
    beatmap.metadata.beats_per_minute = 120;
    beatmap.metadata.beats_per_measure = 4;
    beatmap.metadata.start_offset = 0.5f;
    
    bfbmp::level lvl_normal;
    lvl_normal.name = "Normal";
    lvl_normal.scroll_speed = 10.0f;
    for(int i = 0; i < 10; i++)
        lvl_normal.notes.push_back({ bfbmp::note_type::note_normal, (uint8_t)(i % 4), 1.0f * (float)i, 0.0f });

    bfbmp::level lvl_hard;
    lvl_hard.name = "Hard";
    lvl_hard.scroll_speed = 15.0f;
    for(int i = 0; i < 20; i++)
        lvl_hard.notes.push_back({ bfbmp::note_type::note_normal, (uint8_t)(i % 4), 1.0f * (float)i, 0.0f });
    
    beatmap.game_data.push_back(lvl_normal);
    beatmap.game_data.push_back(lvl_hard);
    
    const char* imgfilters[2] = { "*.png", "*.jpg" };
    if(const char* imgpath = tinyfd_openFileDialog("Open Image file", "", 2, imgfilters, "Image file", false)) beatmap.load_image(imgpath);
    const char* sndfilters[3] = { "*.wav", "*.ogg", "*.flac" };
    if(const char* sndpath = tinyfd_openFileDialog("Open Sound file", "", 3, sndfilters, "Sound file", false)) beatmap.load_sound(sndpath);

    const char* bfbmpfilter[1] = { "*.bfbmp" };
    if(const char* filepath = tinyfd_saveFileDialog("Save beatmap as...", "", 1, bfbmpfilter, "Binary Format Beatmap")) beatmap.encode(filepath);
    
    return 0;
}
