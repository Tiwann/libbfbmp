#include "bfbmp/beatmap.h"
#include "tinyfiledialogs.h"

#include <iostream>

int main()
{
    std::cout << "Press enter and select a bfbmp file.\n";
    bfbmp::beatmap beatmap;
    const char* filter[] = { "*.bfbmp" };
    if(const char* filepath = tinyfd_openFileDialog("Open bfbmp file", "", 1, filter, "Binary Format Beatmap (.bfbmp)", false))
        beatmap.decode(filepath);
    return 0;
}
