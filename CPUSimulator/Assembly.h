#ifndef assemblyheader
#define assemblyheader

#include <string>
#include <vector>
#include "myarchitecture.h"

using namespace std;

//methods
void compile(string);
void createPause(string);
void createFileReader();
string createMachineCodePath();
void firstPassage();

vector<string> parseOneAssemblyCommand(string&);

void createMachineCode(string&);

bool isJType(string&);
bool isIType(string&);
bool isRType(string&);
bool isRTType(string&);
bool isUType(string&);
bool isThisType(const string*, int, string&);

bool isJCorrect(vector<string>&);
bool isICorrect(vector<string>&);
bool isRCorrect(vector<string>&);
bool isRTCorrect(vector<string>&);
bool isUCorrect(vector<string>&);
bool isUCorrect(vector<string>&);

bool handleJOperands(vector<string>&);
bool handleIOperands(vector<string>&);
bool handleROperands(vector<string>&);
bool handleRTOperands(vector<string>&);
bool handleUOperands(vector<string>&);

bool isRegister(string&);
bool isRegisterForWrite(string&);
bool isArgDigit(string&);
bool isInMarksTable(string&);
bool isMark(string&);

int convertJToMachineCode(vector<string>&);
int convertIToMachineCode(vector<string>&);
int convertRToMachineCode(vector<string>&);
int convertRTToMachineCode(vector<string>&);
int convertUToMachineCode(vector<string>&);

int leftShift(int, int);
void reverse(string&);
string myTrim(string&);
string ltrim(string&);
string cutOffSubstrFromStr(string&, char);
bool findCharInString(string&, char);

int getOpcode(string&);
int getOperand(string&, int);
int getAddrJ(string&);
int getAddrU(string&);
int getConstant(string&);
int getNumberFromArray(string&, const string*, int);


//constants
int const OPCODE_POS = 0;
int const RR_POS = 1;
int const RS_POS = 2;
int const RT_POS = 3;
int const IMM_POS = 3;
int const MARK_POS = 3;

unsigned int const RCOMMANDS_FIELDS_COUNT = 4;
unsigned int const ICOMMANDS_FIELDS_COUNT = 4;
unsigned int const JCOMMANDS_FIELDS_COUNT = 2;
unsigned int const RTCOMMANDS_FIELDS_COUNT = 2;
unsigned int const UCOMMANDS_FIELDS_COUNT = 4;

int const COMMANDS_COUNT = 23;
int const ICOMMANDS_COUNT = 5;
int const RCOMMANDS_COUNT = 12;
int const RTCOMMANDS_COUNT = 2;
int const JCOMMANDS_COUNT = 2;
int const UCOMMANDS_COUNT = 2;

string const messgesAfterCompile[10] = {
	". Unknown symbols\n",
	". Wrong size of command\n",
	". Wrong operands\n",
};
string const registersCodes[REG_COUNT] = { "$0","$s0","$s1","$s2", "$t0", "$t1", "$t2", "$t3", "$t4", "$a0",
"$a1", "$a2", "$a3", "$v0", "$sp", "$gp", "$hp", "$k0", "$ra" };
string const iCommands[ICOMMANDS_COUNT] = { "addi", "lw", "sw", "andi", "ori" };
string const uCommands[UCOMMANDS_COUNT] = { "beq", "bne" };
string const rCommands[RCOMMANDS_COUNT] = { "add", "sub","mul", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra" };
string const rtCommands[RTCOMMANDS_COUNT] = { "jr", "print" };
string const jCommands[JCOMMANDS_COUNT] = { "j", "jal" };
string const commands[COMMANDS_COUNT] = { "add", "sub", "mul", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra", "jr", "print", "addi", "lw", "sw", "andi", "ori", "beq", "bne", "j", "jal" };

#endif
