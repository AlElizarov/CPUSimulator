#ifndef cpuheader
#define cpuheader

#include <string>
#include <vector>

using namespace std;


//myassembly functions
void createPause(string);
ifstream createFileReader(string&);
string createMachineCodePath(string&);

vector<string> parseOneAssemblyCommand(string&);

string createMachineCode(vector<vector<string>>&, string&);

bool isJType(string&);
bool isIType(string&);
bool isRType(string&);

bool isJCorrect(vector<string>&);
bool isICorrect(vector<string>&);
bool isRCorrect(vector<string>&);

bool handleJOperands(string&);
bool handleIOperands(vector<string>&);
bool handleROperands(vector<string>&);
bool isRSizeCorrect(vector<string>&);

bool isRegister(string&);
bool isArgDigit(string&);
bool isMark(string& arg);

string convertJToMachineCode(vector<string>&);
string convertIToMachineCode(vector<string>&);
string convertRToMachineCode(vector<string>&);

int getNumberOfCommand(string&);
int getNumberOfReg(string&);

string toBinaryCode(int, int);
void reverse(string&);


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
string myTrim(string&);


//constants assembly
int const REG_SIZE = 16;

int const ICOMMANDS_SIZE = 7;
int const RCOMMANDS_SIZE = 14;
int const JCOMMANDS_SIZE = 2;
int const COMMANDS_SIZE = 23;

int const OPCODE_START = 27;
int const OPCODE_FINISH = 31;
int const OPCODE_LENGTH = OPCODE_FINISH - OPCODE_START + 1;

int const REGCODE_LENGTH = 4;

int const RR_START = 23;
int const RR_FINISH = 26;

int const RS_START = 19;
int const RS_FINISH = 22;

int const RT_START = 15;
int const RT_FINISH = 18;

int const IMM_START = 3;
int const IMM_FINISH = 18;
int const IMM = IMM_FINISH - IMM_START + 1;

string const registersCodes[REG_SIZE] = { "$0","$s0","$s1","$s2", "$t0", "$t1", "$t2", "$t3", "$t4", "$a0",
                                      "$a1" "$a2", "$a3", "$v0", "$sp", "$gp" };
string const iCommands[ICOMMANDS_SIZE] = { "addi", "lw", "sw", "andi", "ori", "beq", "bne" };
string const rCommands[RCOMMANDS_SIZE] = { "add", "sub", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra", "jr", "print" };
string const jCommands[JCOMMANDS_SIZE] = { "j", "jal" };
string const commands[COMMANDS_SIZE] = { "add", "sub", "mul", "div", "and", "or", "xor", "nor", "slt",
"sll", "srl", "sra", "jr", "print", "addi", "lw", "sw", "andi", "ori", "beq", "bne", "j", "jal" };


//consts simulator
int const TO_PC = 4;
unsigned int const CODE_SEGMENT_SIZE = 16 * 1024;
string const jOpcodes[JCOMMANDS_SIZE] = { "10101", "10110" };
string const iOpcodes[ICOMMANDS_SIZE] = { "01110", "01111", "10000", "10001", "10010", "10011", "10100" };
string const rOpcodes[RCOMMANDS_SIZE] = { "00000", "00001", "00010", "00011", "00100","00101", "00110", "00111", "01000", "01001", "01010", "01011", "01100", "01101" };


#endif
