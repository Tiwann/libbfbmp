#include "bfbmp/beatmap.h"
#include "tinyfiledialogs.h"

int main()
{
    bfbmp::beatmap beatmap;
#if 0
    beatmap.metadata.song_name = "Danganronpa";
    beatmap.metadata.sub_name = "The end of Kibogamine";
    beatmap.metadata.author_name = "binaria";
    beatmap.metadata.mapper_name = "Tiwann";
    beatmap.metadata.beats_per_minute = 120;
    beatmap.metadata.beats_per_measure = 4;
    beatmap.metadata.start_offset = 0.5f;
    
    bfbmp::level lvl;
    lvl.name = "Normal";
    lvl.scroll_speed = 10.0f;
    lvl.notes.push_back({ bfbmp::note_type::note_normal, 1.0f, 0.0f });
    
    beatmap.game_data.push_back(lvl);
    
    beatmap.load_image(R"(C:\Users\Tiwann\Downloads\rope.png)");
    beatmap.load_sound(R"(C:\Users\Tiwann\Downloads\summernights 139bpm @jonnywoodbeats (tropical,catchy,pop).mp3)");
    beatmap.encode("C:/Users/Tiwann/Desktop/Danganronpa.bfbmp");
#endif

    const std::string filepath(tinyfd_openFileDialog("Open bfbmp file", "", 0, nullptr, "Binary Format Beatmap", false));
    beatmap.decode(filepath);

    
    return 0;
}
