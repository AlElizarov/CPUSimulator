#ifndef myarch
#define myarch
#include <string>

int const OPCODE_LENGTH = 5;
int const REGCODE_LENGTH = 5;
int const IMM = 16;
int const CAPACITY = 32;

int const REG_COUNT = 19;
struct PCB
{
	std::string path;
	unsigned short pc;

	unsigned short startOfTextSegment;
	unsigned short endOfTextSegment;

	unsigned short startOfGBSegment;
	unsigned short endOfGBSegment;

	unsigned short startOfStackSegment;
	unsigned short endOfStackSegment;

	unsigned short startOfSegment;
	unsigned short endOfSegment;

	short registers[REG_COUNT];
};
#endif
