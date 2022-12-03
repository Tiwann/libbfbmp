#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
#include <stdio.h>
#include <tfd.h>

void write_gui(void);
void write_no_gui(void);


void write_gui(void)
{
    
}

int main(const int argc, const char** argv)
{
    if(argc > 1)
    {
        if(!strcmp("--dialogs", argv[1]))
            if(argc > 2)
            {
                fprintf(stderr, "This argument doesn't require extra arguments\nExpected usage:\twriting --dialogs");
                _exit(EXIT_FAILURE);
            }
    } else
    {
        fprintf(stderr, "An error occurred.\nExpected usage:\twriting \"song name\" \"sub name\" \"author name\" \"mapper name\" \"beats per minutes\" \"start offset\" \"beats per measure\" \"image path\" \"audio path\"");
        _exit(EXIT_FAILURE);
    }

    
    return 0;
}