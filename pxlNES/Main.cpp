#include <iostream>
#include <fstream>
#include <stdint.h>

//iNES header constants
#define MIN_NES_HEADER_LEN		8			//min nes header valid byte len
#define SUB_CHAR				26			//used as escape sequence

typedef char					int8;
typedef short					int16;
typedef int						int32;
typedef long long				int64;
typedef unsigned char			uint8;
typedef unsigned short			uint16;
typedef unsigned int			uint32;
typedef unsigned long long		uint64;

int main(int argc, char* argv[]) {
	std::ifstream file;
	file.open("smb.nes", std::ios::ate | std::ios::binary);
	int32 size = file.tellg();
	file.seekg(0);

	if (size >= MIN_NES_HEADER_LEN) {
		int8* buffer = new int8[size];
		file.read(buffer, size);
		std::string str = buffer;

		if (str[0] == 'N' && str[1] == 'E' && str[2] == 'S' && str[3] == SUB_CHAR) {
			int num_PRG = str[4];
			int num_CHR = str[5];
		}
	}

	return 0;
}
