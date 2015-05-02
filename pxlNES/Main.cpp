#include <iostream>
#include <fstream>
#include <stdint.h>

//iNES header constants
#define MIN_NES_HEADER_LEN		8			//min nes header valid byte len
#define SUB_CHAR				26			//used as escape sequence in MS-DOS

typedef char					int8;
typedef short					int16;
typedef int						int32;
typedef long long				int64;
typedef unsigned char			uint8;
typedef unsigned short			uint16;
typedef unsigned int			uint32;
typedef unsigned long long		uint64;

int16 num_PRGs;
int16 num_CHRs;

int main(int argc, char* argv[]) {
	std::ifstream file;
	file.open("smb.nes", std::ios::ate | std::ios::binary);
	int32 size = file.tellg();
	file.seekg(0);

	if (size >= MIN_NES_HEADER_LEN) {
        std::string str(size, ' ');
        file.read(&str[0], size);

		if (str[0] == 'N' && str[1] == 'E' && str[2] == 'S' && str[3] == SUB_CHAR) {
            num_PRGs = str[4];
            num_CHRs = str[5];
            int8 flag6 = str[6];
            bool hmirror = ~flag6 & 1;
            bool SRAM_enabled = flag6 & 2;
            bool has_trainer = flag6 & 4;
            bool VRAM_4screen = flag6 & 8;

            int a = 5;
		}
	}

	return 0;
}
