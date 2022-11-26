#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
#include <stdio.h>
#include <tfd.h>

int main()
{
    printf("Hello bfbmp reading example!\n");
    printf("Choosing a file...\n");

    const char* filterPattern[1] = { "*.bfbmp" };
    const char* filepath = tinyfd_openFileDialog(
        "Select a bfbmp file",
        NULL,
        1,
        filterPattern,
        "Binary Format Beatmap files (.bfbmp)",
        0);

    bfbmp* pbfbmp = bfbmp_init();
    bfbmp_result result = bfbmp_read_file(filepath, pbfbmp);
    if(result != bfbmp_success)
        printf("Error reading bfbmp file: Error code %d\n", result);
    else
        printf("Succesfully reading bfbmp file %s!\n", filepath);
    return 0;
}