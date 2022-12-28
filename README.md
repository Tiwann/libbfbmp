# libbfbmp
libbfbmp is a C++ library that provides APIs for reading and writing Tiwann's bfbmp files (Binary Format Beatmap) which stores beatmap data for a *future* rhythm game.

# BFBMP File Format
You can find below the file format specifications, so it is possible to write its own program that read/write bfbmp files. <br>
Feel free to send a message at *erwann.messoah@gmail.com* for questions or suggestions. <br>
*NB: I'm currently making heavy changes on the file format structure. Make sure to update frequently...* <br>

### Header
The header contains basic information about the content of the file such as file size or chunk offsets <br>
*All offsets in the file are relative to the beginning of the file* <br>

| Type    | Size (bytes) | Description                                                 |
|---------|--------------|-------------------------------------------------------------|
| char[4] | 4            | File ID "FBMP"                                              |
| uint32  | 4            | Chunk size (should be 32)                                   |
| uint32  | 4            | Total file size (bfbmp files cannot exceed 0xFFFFFFFF bytes |
| uint32  | 4            | Metadata chunk offset                                       |
| uint32  | 4            | Image chunk offset                                          |
| uint32  | 4            | Sound chunk offset                                          |
| uint32  | 4            | Game chunk offset                                           |

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


The type _**string**_ refers to the following structure

| Type       | Size (bytes) | Description |
|------------|--------------|-------------|
| uint32     | 4            | Size        |
| char[size] | size         | String data |

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

----------------------------------------------------------
# How to use
First of all to use the library, make sure to have **git** installed then clone this repo.
```shell
git clone https://github.com/Tiwann/libbfbmp.git libbfbmp
```

Then build the solution files with
```shell
C:\libbfbmp> .\premake\premake5.exe vs2022
```

You can chose whether to build a static lib or a shared lib using *--lib* argument
```shell
C:\libbfbmp> .\premake\premake5.exe vs2022 --lib=shared
C:\libbfbmp> .\premake\premake5.exe vs2022 --lib=static
```

If you are compiling with gcc you can build a Makefile project
```shell
$ ./premake/premake5 gmake2 --lib=shared
$ ./premake/premake5 gmake2 --lib=static
```
