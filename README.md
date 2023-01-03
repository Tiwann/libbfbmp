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
git clone https://github.com/Tiwann/libbfbmp.git libbfbmp
```

## Building Visual Studio solution
To build visual studio solution files <br>
```cmd
C:\libbfbmp>/premake/premake5.exe vs2022
```

## Building Makefile project
To build Makefile project
```bash
$ ./premake/premake5.exe gmake2
```

## Options
### Library Type
You can specify to build a static lib or a shared lib with the *--lib* argument:
```cmd
C:\libbfbmp>/premke/premake5.exe vs2022 --lib=static
or
C:\libbfbmp>/premke/premake5.exe vs2022 --lib=shared
```

### Include Premake Scripts
You can include premake scripts to the solution with *--addscripts*
```cmd
C:\libbfbmp>/premke/premake5.exe vs2022 --addscripts
```
-------------------------------------------------------
# How to use
To create a beatmap and write its data to a file:
```cpp
#include <bfbmp/beatmap.h>

int main()
{
    // create a beatmap object
    bfbmp::beatmap beatmap;
    
    // fill metadata
    beatmap.metadata.song_name = "The Song";
    beatmap.metadata.sub_name = "feat. some artists";
    beatmap.metadata.author_name = "The Author";
    beatmap.metadata.mapper_name = "Map Creator";
    beatmap.metadata.beats_per_minute = 160.0f;
    beatmap.metadata.beats_per_measure = 4;
    beatmap.metadata.start_offset = 0.0f;
    
    // create level data
    bfbmp::level lvl;
    lvl.name = "Easy"
    lvl.scroll_speed = 10.0f;
    lvl.notes.push_back({ bfbmp::note_type::note_normal, 1.0f, 0.0f});
    beatmap.game_data.push_back(lvl);
    
    // load audio and image
    beatmap.load_image("path to image file");
    beatmap.load_sound("path to audio file");
    
    // finally encode beatmap data to a file
    beatmap.encode("path to file");
    
    // you can also write data to a buffer
    std::vector<uint8_t> buffer;
    beatmap.encode(buffer);
}
```

To read beatmap data:
```cpp
#include <bfbmp/beatmap.h>

int main()
{
    bfbmp::beatmap beatmap;
    beatmap.decode("path to bfbmp file");
}
```

## Error handling
Each functions of the *`beatmap`* class returns a boolean for error checking and are coupled with exception throwing.

```cpp
#include <iostream>
#include <bfbmp/beatmap.h>
#include <bfbmp/exception.h>

int main()
{
    bfbmp::beatmap beatmap;

    if(!beatmap.decode("path to file"))
    {
        std::cerr << "Failed to decode bfbmp file!\n;
    }
}
```



[gitlink]:<https://git-scm.com/downloads>