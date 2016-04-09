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

//assembly constants
int const CAPACITY = 32;
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

int const REG_COUNT = 19;

int const COMMANDS_COUNT = 23;
int const ICOMMANDS_COUNT = 5;
int const RCOMMANDS_COUNT = 12;
int const RTCOMMANDS_COUNT = 2;
int const JCOMMANDS_COUNT = 2;
int const UCOMMANDS_COUNT = 2;

int const OPCODE_LENGTH = 5;
int const REGCODE_LENGTH = 5;
int const IMM = 16;

string const messgesAfterCompile[10] = {
	". Unknown symbols\n",
	". Wrong size of command\n",
	". Wrong operands\n",
};
string const registersCodes[REG_COUNT] = { "$0","$s0","$s1","$s2", "$t0", "$t1", "$t2", "$t3", "$t4", "$a0",
                                      "$a1", "$a2", "$a3", "$v0", "$sp", "$gp", "$hp", "$k0", "$ra"};
string const iCommands[ICOMMANDS_COUNT] = { "addi", "lw", "sw", "andi", "ori"};
string const uCommands[UCOMMANDS_COUNT] = { "beq", "bne" };
string const rCommands[RCOMMANDS_COUNT] = { "add", "sub","mul", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra"};
string const rtCommands[RTCOMMANDS_COUNT] = { "jr", "print"};
string const jCommands[JCOMMANDS_COUNT] = { "j", "jal" };
string const commands[COMMANDS_COUNT] = { "add", "sub", "mul", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra", "jr", "print", "addi", "lw", "sw", "andi", "ori", "beq", "bne", "j", "jal"};

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
