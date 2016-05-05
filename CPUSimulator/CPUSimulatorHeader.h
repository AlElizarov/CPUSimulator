#ifndef cpuheader
#define cpuheader

#include <string>

using namespace std;

//simulator functions
bool readCommand();
void executeProgramm(string&);
void loadProgramm(ifstream&);
void callOperatingSystem();

void decriptCommand();
void executeCommand();

int separate(int, int);
int rightShift(int, int);

void separateOpcode();
void separateJAddress();
void separateImm();

void getJOperands();
void getIOperands();
void getROperands();
void getRTOperands();
void getUOperands();

void getRR();
void getRS();
void getRT();

bool isJOpcode();
bool isIOpcode();
bool isROpcode();
bool isRTOpcode();

//simulator constants
int const TO_PC = 4;
unsigned int const RAM_SIZE = 8 * 1024;
unsigned int const TEXT_SEGMENT_SIZE_START = 0;
unsigned int const TEXT_SEGMENT_SIZE_FINISH = 1024-1;
unsigned int const GLOBAL_DATA_SEGMENT_SIZE_START = TEXT_SEGMENT_SIZE_FINISH+1;
unsigned int const GLOBAL_DATA_SEGMENT_SIZE_FINISH = GLOBAL_DATA_SEGMENT_SIZE_START + 1024 - 1;
unsigned int const DYNAMIC_DATA_SEGMENT_SIZE_START = GLOBAL_DATA_SEGMENT_SIZE_FINISH + 1;


int const FIRST_R_COMMAND_IDX = 0;
int const LAST_R_COMMAND_IDX = 11;
int const FIRST_RT_COMMAND_IDX = 12;
int const LAST_RT_COMMAND_IDX = 13;
int const FIRST_I_COMMAND_IDX = 14;
int const LAST_I_COMMAND_IDX = 18;
int const FIRST_U_COMMAND_IDX = 19;
int const LAST_U_COMMAND_IDX = 20;
int const FIRST_J_COMMAND_IDX = 21;
int const LAST_J_COMMAND_IDX = 22;

string const messgesAfterRun[10] = {
	"programm finish successfully with code 0\n",
	"runtime error. Access violation. Error code: 1\n",
	"runtime error. Unknown command. Error code: 2\n",
};

#endif
