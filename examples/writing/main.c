#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
#include <stdio.h>

int main()
{
    printf("Hello bfbmp wrinting example!\n");
    bfbmp* pbfbmp = bfbmp_init_data(
        "The song",
        "Feat. the feat",
        "Author",
        "Mapper",
        140, 0.0f, 4,
        "C:/Users/Tiwann/Downloads/aisaka.png",
        "C:/Users/Tiwann/Downloads/Reveil.mp3");

    bfbmp_result result = bfbmp_write_file("C:/Users/Tiwann/Desktop/aisaka.bfbmp", pbfbmp);
    if(result == bfbmp_success)
        printf("Successfully written bfbmp file!\n");
    
    return 0;
}