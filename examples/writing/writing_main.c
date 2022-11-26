#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
#include <stdio.h>
#include <tfd.h>

int main()
{
    printf("Hello bfbmp wrinting example!\n");
    char soname[128];
    char suname[128];
    char auth[128];
    char mapp[128];
    int bpm;
    int bpmeasure;
    float startoff;

    
    printf("Enter a song name: ");
    scanf("%s", soname);

    printf("Enter a sub name: ");
    scanf("%s", suname);
    
    printf("Enter an Author name: ");
    scanf("%s", auth);
    
    printf("Enter a mapper name: ");
    scanf("%s", mapp);

    printf("Enter a BPM: ");
    scanf("%d", &bpm);

    printf("Enter a Start Offset: ");
    scanf("%f", &startoff);

    printf("Enter a Beats Per Measure value: ");
    scanf("%d", &bpmeasure);


    printf("Select an image file...\n");
    const char* coverFilters[3] = { "*.jpg", "*.jpeg", "*.png" };
    const char* coverPath = tinyfd_openFileDialog(
        "Select an image file",
        NULL,
        3,
        coverFilters,
        "Image files",
        0);

    printf("Select an audio file...\n");
    const char* audioFilters[4] = { "*.mp3", "*.wav", "*.flac", "*.ogg" };
    const char* audioPath = tinyfd_openFileDialog(
        "Select an audio file",
        NULL,
        4,
        audioFilters,
        "Audio files",
        0);

    
    
    bfbmp* pbfbmp = bfbmp_init_data(
        soname,
        suname,
        auth,
        mapp,
        bpm, startoff, bpmeasure,
        coverPath,
        audioPath);

    const char* bfbmpFilter[1] = { "*.bfbmp" };
    const char* dest = tinyfd_saveFileDialog("Save file as...", NULL, 1, bfbmpFilter, "Binary Format Beatmap");
    bfbmp_result result = bfbmp_write_file(dest, pbfbmp);
    if(result == bfbmp_success)
        printf("Successfully written bfbmp file!\n");
    
    return 0;
}