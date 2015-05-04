#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

//iNES header constants
#define INES_HEADER_LEN             16			//min nes header valid byte len
#define SUB_CHAR                    26			//used as escape sequence in MS-DOS

#define PRG_LEN                     16384   //16kb
#define CHR_LEN                     8192    //8kb

#define MAX_OPCODES                 256

#define RAM_SIZE               2048
#define VRAM_ACTUAL_SIZE            16384
#define VRAM_EMULATED_SIZE          12320

#define VRAM_PATTERN_TABLE_0        0       //size: 4096, mapped to emulated PPU VRAM address
#define VRAM_PATTERN_TABLE_1        4096    //size: 4096, mapped to emulated PPU VRAM address
#define VRAM_NAMETABLE_0            8192    //size: 1024, mapped to emulated PPU VRAM address
#define VRAM_NAMETABLE_1            9216    //size: 1024, mapped to emulated PPU VRAM address
#define VRAM_NAMETABLE_2            10240   //size: 1024, mapped to emulated PPU VRAM address
#define VRAM_NAMETABLE_3            11264   //size: 1024, mapped to emulated PPU VRAM address
#define VRAM_NAMETABLE_MIRRORS      12288   //size: 3840, mapped to actual PPU VRAM address
#define VRAM_PALETTE_INDEX          12288   //size: 32, mapped to emulated PPU VRAM address
#define VRAM_PALETTE_MIRROR         16160   //size: 32, mapped to actual PPU VRAM address

typedef char                        int8;
typedef short                       int16;
typedef int                         int32;
typedef long long                   int64;
typedef unsigned char               uint8;
typedef unsigned short              uint16;
typedef unsigned int                uint32;
typedef unsigned long long          uint64;

#endif