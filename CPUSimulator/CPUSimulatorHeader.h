#ifndef cpuheader
#define cpuheader

#include <string>
#include <vector>

using namespace std;


//myassembly functions
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
bool isArgDigit(string&);
bool isInMarksTable(string&);
bool isMark(string&);

int convertJToMachineCode(vector<string>&);
int convertIToMachineCode(vector<string>&);
int convertRToMachineCode(vector<string>&);
int convertRTToMachineCode(vector<string>&);
int convertUToMachineCode(vector<string>&);

int shift(int shifted, int shiftValue);

void reverse(string&);
string myTrim(string&);
string ltrim(string&);
string cutOffCharacterFromStr(string&, char);

//simulator functions
void readCommand();
void executeProgramm(string&);
void loadProgramm(ifstream&);

void deshifrCommand();

void getOpcode();
void getJOperands();
void getIOperands();
void getROperands();

int fromBinaryToInt(string&);
void getImm();
void getRR();
void getRS();
void getRT();

bool isJOpcode();
bool isIOpcode();

bool findCharInString(string&, char);

//assembly constants
int const OPCODE_POS = 0;
int const RR_POS = 1;
int const RS_POS = 2;
int const RT_POS = 3;
int const IMM_POS = 3;
int const MARK_POS = 1;

int const RCOMMANDS_FIELDS_COUNT = 4;
int const ICOMMANDS_FIELDS_COUNT = 4;
int const JCOMMANDS_FIELDS_COUNT = 2;
int const RTCOMMANDS_FIELDS_COUNT = 2;
int const UCOMMANDS_FIELDS_COUNT = 4;

int const REG_COUNT = 16;

int const ICOMMANDS_COUNT = 5;
int const RCOMMANDS_COUNT = 12;
int const RTCOMMANDS_COUNT = 12;
int const JCOMMANDS_COUNT = 2;
int const UCOMMANDS_COUNT = 2;
int const COMMANDS_COUNT = 23;

int const OPCODE_LENGTH = 5;
int const REGCODE_LENGTH = 4;
int const IMM = 16;

//int const OPCODE_START = 27;
//int const OPCODE_FINISH = 31;
//int const OPCODE_LENGTH = OPCODE_FINISH - OPCODE_START + 1;
//
//int const REGCODE_LENGTH = 4;
//
//int const RR_START = 23;
//int const RR_FINISH = 26;
//
//int const RS_START = 19;
//int const RS_FINISH = 22;
//
//int const RT_START = 15;
//int const RT_FINISH = 18;
//
//int const IMM_START = 3;
//int const IMM_FINISH = 18;
//int const IMM = IMM_FINISH - IMM_START + 1;

string const registersCodes[REG_COUNT] = { "$0","$s0","$s1","$s2", "$t0", "$t1", "$t2", "$t3", "$t4", "$a0",
                                      "$a1" "$a2", "$a3", "$v0", "$sp", "$gp" };
string const iCommands[ICOMMANDS_COUNT] = { "addi", "lw", "sw", "andi", "ori"};
string const uCommands[UCOMMANDS_COUNT] = { "beq", "bne" };
string const rCommands[RCOMMANDS_COUNT] = { "add", "sub", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra"};
string const rtCommands[RTCOMMANDS_COUNT] = { "jr", "print"};
string const jCommands[JCOMMANDS_COUNT] = { "j", "jal" };
string const commands[COMMANDS_COUNT] = { "add", "sub", "mul", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra", "jr", "print", "addi", "lw", "sw", "andi", "ori", "beq", "bne", "j", "jal" };


//simulator constants
int const TO_PC = 4;
unsigned int const CODE_SEGMENT_SIZE = 16 * 1024;
string const jOpcodes[JCOMMANDS_COUNT] = { "10101", "10110" };
string const iOpcodes[ICOMMANDS_COUNT] = { "01110", "01111", "10000", "10001", "10010"};
string const uOpcodes[UCOMMANDS_COUNT] = { "10011", "10100" };
string const rOpcodes[RCOMMANDS_COUNT] = { "00000", "00001", "00010", "00011", "00100","00101", "00110", "00111", "01000", "01001", "01010", "01011"};
string const rtOpcodes[RTCOMMANDS_COUNT] = { "01100", "01101" };

#endif
