#ifndef mywin
#define mywin

#include <string>
#include <vector>
#include "MyArhitecture.h"

const unsigned int TEXT_SEGMENT_SIZE = 1024;
const unsigned int PROCESS_SEGMENT_SIZE = 1024 * 8;
const unsigned int GLOBAL_DATA_SEGMENT_SIZE = 1024;
const unsigned int STACK_DATA_SEGMENT_SIZE = 6 * 1024;

void getProgramms(std::vector<std::string>&);
PCB initializePCB(int, std::string);
#endif
