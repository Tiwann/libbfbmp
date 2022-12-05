# libbfbmp
libbfbmp is a C/C++ single header library that provides APIs for reading and writing Tiwann's bfbmp files (Binary Format Beatmap) that stores information about beatmaps and mods for a *future* rhythm game.

# BFBMP File Format
You can found below the file format specifications, so it is possible to write its own program that read/write bfbmp files.
Feel free to send a message at _erwann.messoah@gmail.com_ for questions or suggestions.
*NB: I'm currently making heavy changes on the file format structure. Make sure to update frequenlty...*

### Header
|   Type   |  Size (Bytes)  |                   Description                     |
| -------- | -------------- | ------------------------------------------------- |
|  char[4] |   4            | File ID *"FBMP"*                                  |
|  int32   |   4            | Packed version numbers (5.3.1.2 => 0x05030102)    |
|  int32   |   4            | Total file size                                   |

### Metadata Chunk
The metadata chunk contains information about
|   Type    |  Size (Bytes)  |                   Description                     |
| --------  | -------------- | ------------------------------------------------- |
| char[4]   | 4              | Meta chunk ID "META"                              |
| int32     | 4              | Meta chunk size                                   |
| string    | x              | Song name                                         |
| string    | x              | Sub name                                          |
| string    | x              | Author name                                       |
| string    | x              | Mapper name                                       |
| int32     | 4              | Song BPM                                          |
| float     | 4              | Start Offset                                      |
| int32     | 4              | Beats per measure                                 |

The type _**string**_ refers to the following structure
|   Type          |  Size (Bytes)  |                   Description                     |
| --------        | -------------- | ------------------------------------------------- |
| int32           | 4              | String length                                     |
| char[length]    | length         | String data                                       |

### Image Chunk
| Type        | Size (Bytes)    | Description            |
| --------    | --------------  | ---------------------- |
| char[4]     | 4               | IMG chunk ID *"!IMG"*  |
| int32       | 4               | Chunk data size        |
| char[size]  | size            | Data                   |

### Sound Chunk
| Type        | Size (Bytes)    | Description            |
| --------    | --------------  | ---------------------- |
| char[4]     | 4               | SND chunk ID *"!SND"*  |
| int32       | 4               | Chunk data size        |
| char[size]  | size            | Data                   |
----------------------------------------------------------
# How to use
As said above, libbfbmp is a single header library. It is written in C for maximum compatibility. To use it simply download **[bfbmp.h][bfbmphlnk]** and place it into your C/C++ project.
Then in a c/cpp file:
```c
#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
// Use APIs here
```

## Reading
If you want to read bfbmp data you have to use  ```bfbmp_read_xxx()``` apis:
```c
bfbmp* m_bfbmp = bfbmp_init();
bfbmp_read_file("Filepath", m_bfbmp);
or
bfbmp_read_memory(buffer, buffer_size, m_bfbmp);
```
Note that you can use ```bfbmp_read_metadata_xxx()``` to read metadata chunk only.

## Writing
If you want to write bfbmp data to file you have to use  ```bfbmp_write_file()``` api:
```c
bfbmp* m_bfbmp = bfbmp_init_data(params);
bfbmp_write_file("filepath", m_bfbmp);
```

# Options
You can define ```BFBMP_MALLOC```, ```BFBMP_REALLOC```, ```BFBMP_FREE```, ```BFBMP_MEMSET```, ```BFBMP_MEMCPY``` if you want to use custom memory allocation functions.
Example using Unreal Engine:
```cpp
#define BFBMP_MALLOC(size)          FMemory::Malloc(size)
#define BFBMP_REALLOCC(ptr, size)   FMemory::Malloc(ptr, size)
#define BFBMP_FREE(ptr)             FMemory::Free(ptr)
#define BFBMP_IMPLEMENTATION
#include "bfbmp.h"
// libbfbmp now uses Unreal Engine's memory allocation functions
```

Defining ```BFBMP_NO_STDIO``` disables ```bfbmp_xxx_file()``` apis.
Defining ```BFBMP_USE_ASSERT``` enbales assertions.

# Upcoming apis
Currently working on some apis for reading audio data or/and image data only.

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

Binaries are located into ```examples/xxx/binaries```



[bfbmphlnk]:<https://github.com/Tiwann/libbfbmp/blob/master/include/bfbmp.h>
