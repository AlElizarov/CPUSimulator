#ifndef mysim
#define mysim

#include <string>
#include "MyArhitecture.h"

bool readCommand();
void executeProgramm(PCB);
void loadProgramm(std::ifstream&);
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

std::string const messgesAfterRun[10] = {
	"programm finish successfully with code 0\n",
	"runtime error. Access violation. Error code: 1\n",
	"runtime error. Unknown command. Error code: 2\n",
	"runtime error. Access violation for communication. Error code: 3\n"
};

#endif
