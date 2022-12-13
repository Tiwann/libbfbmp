# libbfbmp
libbfbmp is a C++ single header library that provides APIs for reading and writing Tiwann's bfbmp files (Binary Format Beatmap) which stores beatmap data for a *future* rhythm game.

# BFBMP File Format
You can find below the file format specifications, so it is possible to write its own program that read/write bfbmp files. <br>
Feel free to send a message at *erwann.messoah@gmail.com* for questions or suggestions. <br>
*NB: I'm currently making heavy changes on the file format structure. Make sure to update frequently...* <br>

### Header
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
| char[4] | 4            | Chunk ID "META    |
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
| Type       | Size (Bytes)   | Description           |
|------------|----------------|-----------------------|
| char[4]    | 4              | IMG chunk ID *"!IMG"* |
| uint32     | 4              | Chunk size            |
| char[size] | size           | Data                  |

### Sound Chunk
| Type       | Size (Bytes)   | Description           |
|------------|----------------|-----------------------|
| char[4]    | 4              | SND chunk ID *"!SND"* |
| uint32     | 4              | Chunk size            |
| char[size] | size           | Data                  |
----------------------------------------------------------
# How to use
As said above, libbfbmp is a single header library. <br>
To use it simply download **[bfbmp.h][bfbmphlnk]** and place it into your C++ project.
Then in a cpp file:
```cpp
#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
// Use APIs here
```

## Reading
If you want to read all bfbmp data you have to use ```bfbmp_decode()``` apis:
```c
bfbmp bfbmp;
bfbmp_decode("filepath", bfbmp);
or
bfbmp_decode(buffer, buffer_size, bfbmp);
```
Note that you can use ```bfbmp_read_metadata()``` to read metadata chunk only. <br>
```
bfbmp_metadata metadata;
bfbmp_read_metadata("filepath", metadata);
```

## Writing
If you want to write bfbmp data to file you have to use  ```bfbmp_encode()``` apis:
```c
bfbmp bfbmp;
// filling data...
bfbmp_encode("filepath", bfbmp);
```

# Demo Examples
This repo includes some example projects that you can build and run.
First, makes sure you have git installed then clone this repositoty
```sh
git clone https://github.com/Tiwann/libbfbmp.git
or if you have ssh setup
git clone git@github.com:Tiwann/libbfbmp.git
```

If using Visual Studio you can build a solution by running ```build.bat``` or
```cmd
C:\libbfbmp> .\premake\premake5 v2022
```

If not using Visual Studio you can generate a makefile with
```sh
$ ./premake/premake5 gmake2
```
 Then build using
 ```sh
$ make config=debug
```

Binaries are located into ```examples/{project name}/binaries/{configuration}```



[bfbmphlnk]:<https://github.com/Tiwann/libbfbmp/blob/master/include/bfbmp.h>
