# libbfbmp
libbfbmp is a C++ library that provides APIs for reading and writing Tiwann's bfbmp files (Binary Format Beatmap) which stores beatmap data for a *future* rhythm game.

# BFBMP File Format
You can find below the file format specifications, so it is possible to write its own program that read/write bfbmp files. <br>
Feel free to send a message at *erwann.messoah@gmail.com* for questions or suggestions. <br>
*NB: I'm currently making heavy changes on the file format structure. Make sure to update frequently...* <br>
 

### Header
The header contains basic information about the content of the file such as file size or chunk offsets <br>
*All offsets in the file are relative to the beginning of the file* <br>

| Type    | Size (bytes) | Description               |
|---------|--------------|---------------------------|
| char[4] | 4            | File ID "FBMP"            |
| uint32  | 4            | Chunk size (should be 32) |
| uint32  | 4            | Total file size           |
| uint32  | 4            | Metadata chunk offset     |
| uint32  | 4            | Image chunk offset        |
| uint32  | 4            | Sound chunk offset        |
| uint32  | 4            | Game chunk offset         |

### Metadata Chunk
The META chunk contains metadata information such as song name, audio bpm etc <br>

| Type    | Size (bytes) | Description       |
|---------|--------------|-------------------|
| char[4] | 4            | Chunk ID "META"   |
| uint32  | 4            | Chunk size        |
| string  | ?            | Song name         |
| string  | ?            | Sub name          |
| string  | ?            | Author name       |
| string  | ?            | Mapper name       |
| float   | 4            | Beats per minute  |
| uint32  | 4            | Beats per measure |
| float   | 4            | Start offset      |


### Image Chunk
The IMG chunk contains image data. It can be jpeg data or png data. <br>

| Type       | Size (Bytes)   | Description           |
|------------|----------------|-----------------------|
| char[4]    | 4              | IMG chunk ID *"!IMG"* |
| uint32     | 4              | Chunk size            |
| char[size] | size           | Data                  |

### Sound Chunk
The SND chunk contains sound data. It can be wav data, mp3 data, ogg data or flac data. <br>

| Type       | Size (Bytes)   | Description           |
|------------|----------------|-----------------------|
| char[4]    | 4              | SND chunk ID *"!SND"* |
| uint32     | 4              | Chunk size            |
| char[size] | size           | Data                  |

### Game Chunk
The GAM chunk contains level data. <br>

| Type       | Size (Bytes)       | Description                |
|------------|--------------------|----------------------------|
| char[4]    | 4                  | GAM chunk ID *"!GAM"*      |
| uint32     | 4                  | Chunk size                 |
| uint32     | 4                  | Number of level sub chunks |
| level[num] | num * sizeof level | Level sub chunks           |

#### Level Sub chunk

| Type    | Size (Bytes) | Description           |
|---------|--------------|-----------------------|
| char[4] | 4            | LVL chunk ID *"!LVL"* |
| string  | ?            | Level name            |
| float   | 4            | Scroll speed          |

#### Note sub chunk
| Type      | Size (Bytes)      | Description           |
|-----------|-------------------|-----------------------|
| char[4]   | 4                 | NOT chunk ID *"!NOT"* |
| uint32    | 4                 | Number of notes       |
| note[num] | num * sizeof note | Notes                 |



The type _**string**_ refers to the following structure

| Type       | Size (bytes) | Description |
|------------|--------------|-------------|
| uint32     | 4            | Size        |
| char[size] | size         | String data |

The type _**note**_ refers to the following structure

| Type   | Size (bytes) | Description         |
|--------|--------------|---------------------|
| uint32 | 4            | Note type           |
| float  | 4            | position (in beats) |
| float  | 4            | duration (in beats) |


----------------------------------------------------------
# How to build
First of all, make sure to have **[git][gitlink]** installed then clone this repo.
```shell
git clone https://github.com/Tiwann/libbfbmp.git [directory]
```
This project uses **[premake][premakelink]** as build tool.

## Building Visual Studio solution
To build visual studio solution files <br>
```shell
$ PATH_TO_PREMAKE/premake5 vs2022
```

## Building Makefiles
To build Makefile project
```shell
$ PATH_TO_PREMAKE/premake5 gmake2
```

-------------------------------------------------------
# How to use
To create a beatmap and write its data to a file:
```c++
#include <bfbmp/bfbmp.h>

int main(void)
{
    // Initialize a beatmap object and set its metadata
    bfbmp_beatmap_t beatmap = bfbmp_beatmap_create();
    bfbmp_beatmap_set_song_name(&beatmap, "The Song Name");
    bfbmp_beatmap_set_sub_name(&beatmap, "feat. some artist");
    bfbmp_beatmap_set_author_name(&beatmap, "The author");
    bfbmp_beatmap_set_mapper_name(&beatmap, "Map creator");
    beatmap.metadata.beats_per_minute = 160.0f;
    beatmap.metadata.beats_per_measure = 4;
    beatmap.metadata.start_offset = 0.0f;

    // Create a level object and add notes to it
    bfbmp_level_t level = bfbmp_level_create_with_name("Level Name");
    level.scroll_speed = 1.0f;

    bfbmp_note_t note;
    note.duration = 1.0f;
    note.position = 1.0f;
    note.type = bfbmp_note_normal;
    bfbmp_vector_note_push(&level.notes, note);

    // Adding level into beatmap
    bfbmp_vector_level_push(&beatmap.game_data, level);
    
    // Load image and sound into beatmpa object
    bfbmp_beatmap_load_image(&beatmap, "Path/To/Image/File");
    bfbmp_beatmap_load_sound(&beatmap, "Path/To/Audio/File");

    // Finally, write beatmap data to file
    const char* filepath = "Path/To/OutputFile.bfbmp";
    const uint8_t result = bfbmp_beatmap_encode_file(&beatmap, filepath, BFBMP_FALSE);
    if(result != BFBMP_SUCCESS) fprintf(stderr, "Failed to write beatmap to file!");

    // Always free the beatmap object after using it
    bfbmp_beatmap_free(&beatmap);
}
```

To read beatmap data:
```c++
#include <bfbmp/bfbmp.h>

int main(void)
{
    bfbmp_beatmap_t beatmap = bfbmp_beatmap_create();
    const char* filepath = "Path/To/File.bfbmp";
    bfbmp_beatmap_decode_file(&beatmap, filepath);
}
```

## Error handling
Each public api functions return a `uint8_t` that is used as a boolean to check if an error occured. It's value could be `BFBMP_SUCCESS` or `BFBMP_FAIL`

```c++
#include <stdio.h>
#include <bfbmp/bfbmp.h>

int main(void)
{
    bfbmp_beatmap_t beatmap = bfbmp_beatmap_create();
    const char* filepath = "Path/To/File.bfbmp";
    uint8_t result = bfbmp_beatmap_decode_file(&beatmap, filepath);
    if(result != BFBMP_SUCCESS) 
    { 
        fprintf(stderr, "Failed to decode beatmap!");
        return -1;
    }
    return 0;
}
```


## C++ Wrapper
A C++ header `bfbmp.hpp` is provided in the `include` directory that abstracts all the apis into C++ classes.

```c++
#include <bfbmp/bfbmp.hpp>
#include <iostream>

int main()
{
    const std::string filepath = "Path/To/File.bfbmp";
    bfbmp::beatmap beatmap;
    if(!beatmap.decode(filepath))
    {
        std::cerr << "Failed to decode beatmap!\n;
        return -1;
    }
    return 0;
}
```


[gitlink]:<https://git-scm.com/downloads>
[premakelink]:<https://premake.github.io/>