# libbfbmp
libbfbmp is a C/C++ single header library that provides APIs for reading and writing Tiwann's bfbmp files (Binary Format Beatmap) that stores information about beatmaps and mods for a *future* rhythm game.

# BFBMP File Format
You can found below the file format specifications, so it is possible to write its own program that read/write bfbmp files.
Feel free to send a message at _erwann.messoah@gmail.com_ for questions or suggestions.

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
