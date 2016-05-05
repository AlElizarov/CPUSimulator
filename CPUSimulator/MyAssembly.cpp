#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "cpu.h"
using namespace std;

struct AsmProgramm
{
	map<string, int> marksTable;
	vector<vector<string>> instractions;
	string path;
};
AsmProgramm programm;

string compiledStatus;
int compilingErrorsStatus;

void compile(string path) {
	programm.path = path;
	firstPassage();

	createPause("Press enter to compile...");

	string pathToMachineCode = createMachineCodePath();
	createMachineCode(pathToMachineCode);                    // second passage

	if (compiledStatus.compare("program compiled successfully\n") == 0) {
		cout << compiledStatus;
	}
	else {
		cout << compiledStatus << messgesAfterCompile[compilingErrorsStatus] << endl;
	}
}

void firstPassage() {
	string assemblyInstr;
	int currentCommand = 0;
	ifstream fileReader;
	fileReader.open(programm.path);
	while (getline(fileReader, assemblyInstr))
	{
		if (findCharInString(assemblyInstr, '#')) {
			assemblyInstr = cutOffSubstrFromStr(assemblyInstr, '#');
		}
		assemblyInstr = myTrim(assemblyInstr);
		if (assemblyInstr.compare("") == 0) {
			continue;
		}
		if (isMark(assemblyInstr)) {
			programm.marksTable[cutOffSubstrFromStr(assemblyInstr, ':')] = currentCommand;
		}
		else {
			programm.instractions.push_back(parseOneAssemblyCommand(assemblyInstr));
			currentCommand += 4;
		}
	}
	fileReader.close();
}

bool isMark(string& assemblyInstr) {
	return !findCharInString(assemblyInstr, ' ')
		&& !findCharInString(assemblyInstr, ',')
		&& findCharInString(assemblyInstr, ':')
		&& assemblyInstr.length() > 1;
}

void createPause(string message) {
	cout << message;
	cin.get();
}

string createMachineCodePath() {
	return cutOffSubstrFromStr(programm.path, '.') + ".dat";
}

vector<string> parseOneAssemblyCommand(string& assemblyInstruction) {
	vector<string> tmp;
	istringstream iss(assemblyInstruction);
	string subStringAfterSpaces;

	while (getline(iss, subStringAfterSpaces, ' ')) {
		if (findCharInString(subStringAfterSpaces, ',')) {
			istringstream issForSubstring(subStringAfterSpaces);
			string subStringAfterCommas;
			while (getline(issForSubstring, subStringAfterCommas, ',')) {
				tmp.push_back(subStringAfterCommas);
			}
		}
		else {
			tmp.push_back(subStringAfterSpaces);
		}
	}
	return tmp;
}

void createMachineCode(string& path) {
	ofstream out(path, ios_base::out | ios_base::binary);
	int machineCode;

	for (size_t instr = 0; instr < programm.instractions.size(); instr++) {

		if (isIType(programm.instractions[instr][OPCODE_POS])) {
			if (isICorrect(programm.instractions[instr])) {
				machineCode = convertIToMachineCode(programm.instractions[instr]);
			}
			else {
				compiledStatus =  "syntax error on line: " + to_string(instr + 1);
				return;
			}
		}
		else if (isRType(programm.instractions[instr][OPCODE_POS])) {
			if (isRCorrect(programm.instractions[instr])) {
				machineCode = convertRToMachineCode(programm.instractions[instr]);
			}
			else {
				compiledStatus = "syntax error on line: " + to_string(instr + 1);
				return;
			}
		}
		else if (isJType(programm.instractions[instr][OPCODE_POS])){
			if (isJCorrect(programm.instractions[instr])) {
				machineCode = convertJToMachineCode(programm.instractions[instr]);
			}
			else {
				compiledStatus =  "syntax error on line: " + to_string(instr + 1);
				return;
			}
		}
		else if (isRTType(programm.instractions[instr][OPCODE_POS])) {
			if (isRTCorrect(programm.instractions[instr])) {
				machineCode = convertRTToMachineCode(programm.instractions[instr]);
			}
			else {
				compiledStatus = "syntax error on line: " + to_string(instr + 1);
				return;
			}
		}
		else if (isUType(programm.instractions[instr][OPCODE_POS])) {
			if (isUCorrect(programm.instractions[instr])) {
				machineCode = convertUToMachineCode(programm.instractions[instr]);
			}
			else {
				compiledStatus = "syntax error on line: " + to_string(instr + 1);
				return;
			}
		}
		else {
			compilingErrorsStatus = 0;
			compiledStatus = "syntax error on line: " + to_string(instr + 1);
			return;
		}

		out.write((char*) &machineCode, sizeof machineCode);
		machineCode = 0;

		if (instr == programm.instractions.size() - 1) {
			int temp = 0;                                            // last command - halt.
		    out.write((char*)&temp, sizeof(int));
		}
	}

	out.close();

	compiledStatus = "program compiled successfully\n";
}

bool isIType(string& commandName) {
	return isThisType(iCommands, ICOMMANDS_COUNT, commandName);
}

bool isRType(string& commandName) {
	return isThisType(rCommands, RCOMMANDS_COUNT, commandName);
}

bool isRTType(string& commandName) {
	return isThisType(rtCommands, RTCOMMANDS_COUNT, commandName);
}

bool isJType(string& commandName) {
	return isThisType(jCommands, JCOMMANDS_COUNT, commandName);
}

bool isUType(string& commandName) {
	return isThisType(uCommands, UCOMMANDS_COUNT, commandName);
}

bool isThisType(const string* thisTypeCommands, int thisTypeCommandsCount, string& commandName) {
	for (int command = 0; command < thisTypeCommandsCount; command++) {
		if (commandName.compare(thisTypeCommands[command]) == 0) {
			return true;
		}
	}
	return false;
}

bool isJCorrect(vector<string>& assmInstr) {
	if (assmInstr.size() != JCOMMANDS_FIELDS_COUNT) {
		compilingErrorsStatus = 1;
		return false;
	}
	return handleJOperands(assmInstr);
}

bool isICorrect(vector<string>& assmInstr) {
	if (assmInstr.size() != ICOMMANDS_FIELDS_COUNT) {
		compilingErrorsStatus = 1;
		return false;
	}
	return handleIOperands(assmInstr);
}

bool isRCorrect(vector<string>& assmInstr) {
	if (assmInstr.size() != RCOMMANDS_FIELDS_COUNT) {
		compilingErrorsStatus = 1;
		return false;
	}
	return handleROperands(assmInstr);
}

bool isRTCorrect(vector<string>& assmInstr) {
	if (assmInstr.size() != RTCOMMANDS_FIELDS_COUNT) {
		compilingErrorsStatus = 1;
		return false;
	}
	return handleRTOperands(assmInstr);
}

bool isUCorrect(vector<string>& assmInstr) {
	if (assmInstr.size() != UCOMMANDS_FIELDS_COUNT) {
		compilingErrorsStatus = 1;
		return false;
	}
	return handleUOperands(assmInstr);
}

bool handleJOperands(vector<string>& assmInstr) {
	return isInMarksTable(assmInstr[RR_POS]);
}

bool handleIOperands(vector<string>& assmInstr) {
	return isRegister(assmInstr[RR_POS]) && isRegisterForWrite(assmInstr[RR_POS]) && isRegister(assmInstr[RS_POS]) && isArgDigit(assmInstr[IMM_POS]);
}

bool handleROperands(vector<string>& assmInstr) {
	return isRegister(assmInstr[RR_POS]) && isRegisterForWrite(assmInstr[RR_POS]) && isRegister(assmInstr[RS_POS]) && isRegister(assmInstr[RT_POS]);
}

bool handleRTOperands(vector<string>& assmInstr) {
	return isRegister(assmInstr[RR_POS]);
}

bool handleUOperands(vector<string>& assmInstr) {
	return isRegister(assmInstr[RR_POS]) && isRegister(assmInstr[RS_POS]) && isInMarksTable(assmInstr[MARK_POS]);
}

bool isRegister(string& registerName) {
	for (string reg : registersCodes)
	{
		if (reg.compare(registerName) == 0) {
			return true;
		}
	}
	return false;
}

bool isRegisterForWrite(string& registerName) {
	if (registerName.compare("$0") == 0 || registerName.compare("$ra") == 0 || registerName.compare("$k0") == 0) {
		compilingErrorsStatus = 2;
		return false;
	}
	return true;
}

bool isInMarksTable(string& arg) {
	if (programm.marksTable.count(arg)) {
		return true;
		
	}
	return false;
}

bool isArgDigit(string& arg) {
	for (size_t character = 0; character < arg.length(); character++) {
		if (character == 0 && arg[character] == '-') continue;
		if (!isdigit(arg[character])) {
			return false;
		}
	}
	return true;
}

int convertJToMachineCode(vector<string>& assmInstr) {
	int opcode = getOpcode(assmInstr[OPCODE_POS]);
	int addr = getAddrJ(assmInstr[RR_POS]);
	return opcode | addr;
}

int convertIToMachineCode(vector<string>& assmInstr) {
	int opcode = getOpcode(assmInstr[OPCODE_POS]);
	int rr = getOperand(assmInstr[RR_POS], RR_POS);
	int rs = getOperand(assmInstr[RS_POS], RS_POS);
	int imm = getConstant(assmInstr[IMM_POS]);
	return opcode | rr | rs | imm;
}

int convertRToMachineCode(vector<string>& assmInstr) {                            
	int opcode = getOpcode(assmInstr[OPCODE_POS]);
	int rr = getOperand(assmInstr[RR_POS], RR_POS);
	int rs = getOperand(assmInstr[RS_POS], RS_POS);
	int rt = getOperand(assmInstr[RT_POS], RT_POS);
	return opcode | rr | rs | rt;                     
}

int convertRTToMachineCode(vector<string>& assmInstr) {
	int opcode = getOpcode(assmInstr[OPCODE_POS]);
	int rr = getOperand(assmInstr[RR_POS], RR_POS);
	return opcode | rr;
}

int convertUToMachineCode(vector<string>& assmInstr) {
	int opcode = getOpcode(assmInstr[OPCODE_POS]);
	int rr = getOperand(assmInstr[RR_POS], RR_POS);
	int rs = getOperand(assmInstr[RS_POS], RS_POS);
	int addr = getAddrU(assmInstr[MARK_POS]);
	return opcode | rr | rs | addr;
}

int getOpcode(string& commandName) {
	int numberOfComand = getNumberFromArray(commandName, commands, COMMANDS_COUNT);
	return leftShift(numberOfComand, CAPACITY - OPCODE_LENGTH);
}

int getOperand(string& regName, int regPos) {
	int numberOfReg = getNumberFromArray(regName, registersCodes, REG_COUNT);
	return leftShift(numberOfReg, CAPACITY - (OPCODE_LENGTH + regPos * REGCODE_LENGTH));
}

int getAddrJ(string& mark) {
	int numberOfMark = programm.marksTable[mark];
	return leftShift(numberOfMark, CAPACITY - (OPCODE_LENGTH + IMM));
}

int getAddrU(string& mark) {
	int numberOfMark = programm.marksTable[mark];
	return leftShift(numberOfMark, CAPACITY - (OPCODE_LENGTH + 2 * REGCODE_LENGTH + IMM));
}

int getConstant(string& imm) {
	int constant = stoi(imm);
	int mask = 0x0000FFFF;
	int constant16Bits = constant & mask;
	return leftShift(constant16Bits, CAPACITY - (OPCODE_LENGTH + 2 * REGCODE_LENGTH + IMM));
}

//call with only correct value after check finished successfully
int getNumberFromArray(string& value, const string* arr, int arrLength) {
	for (int i = 0; i < arrLength; i++) {
		if (value.compare(arr[i]) == 0) {
			return i;
		}
	}
}





