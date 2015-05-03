#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

//iNES header constants
#define INES_HEADER_LEN         16			//min nes header valid byte len
#define SUB_CHAR                26			//used as escape sequence in MS-DOS

#define PRG_LEN 16384   //16kb
#define CHR_LEN 8192    //8kb

typedef char                    int8;
typedef short                   int16;
typedef int                     int32;
typedef long long               int64;
typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;
typedef unsigned long long      uint64;

#endif