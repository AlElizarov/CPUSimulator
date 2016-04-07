#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <clocale>
#include <map>
#include "CPUSimulatorHeader.h"

using namespace std;

map<string, int> marksTable;
vector<vector<string>> assemblyProgramm;
string pathToAsmProgramm;
ifstream fileReader;

int main() {
	setlocale(LC_ALL, "Russian");

	firstPassage();                                               
	createPause("Нажмите enter что бы начать компиляцию...");

	string pathToMachineCode = createMachineCodePath();                
	createMachineCode(pathToMachineCode);                    // second passage

	createPause("Нажмите enter что бы начать выполнение...");
	executeProgramm(pathToMachineCode);

	_getch();
}

void firstPassage() {
	createFileReader();
	string assemblyInstr;
	int currentCommand = 0;
	while (getline(fileReader, assemblyInstr))
	{
		if (isMark(assemblyInstr)) {
			marksTable[cutOffCharacterFromStr(assemblyInstr, ':')] = currentCommand;
		}
		else {
			assemblyProgramm.push_back(parseOneAssemblyCommand(myTrim(assemblyInstr)));
		}
		currentCommand += 4;
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
	cin.get();
}


void createFileReader() {
	cout << "Введите полное имя файла с программой на ассемблере: ";
	cin >> pathToAsmProgramm;
	fileReader.open(pathToAsmProgramm);
	while (!fileReader.is_open()) {
		cout << "Файла с укзанным именем не существует. Пожалуйста, введите другое имя: ";
		cin >> pathToAsmProgramm;
		fileReader.open(pathToAsmProgramm);
	}
}

string createMachineCodePath() {
	return cutOffCharacterFromStr(pathToAsmProgramm, '.') + ".dat";
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

bool findCharInString(string& str, char c) {
	for (char character : str) {
		if (character == c)
			return true;
	}
	return false;
}

void createMachineCode(string& path) {
	ofstream out(path, ios_base::out | ios_base::binary);

	string compiledStatus;
	int machineCode;

	for (int instr = 0; instr < assemblyProgramm.size(); instr++) {

		if (isIType(assemblyProgramm[instr][OPCODE_POS])) {
			if (isICorrect(assemblyProgramm[instr])) {
				machineCode = convertIToMachineCode(assemblyProgramm[instr]);
			}
			else {
				compiledStatus =  "syntax error i on line: " + to_string(instr + 1)+'\n';
			}
		}
		else if (isRType(assemblyProgramm[instr][OPCODE_POS])) {
			if (isRCorrect(assemblyProgramm[instr])) {
				machineCode = convertRToMachineCode(assemblyProgramm[instr]);
			}
			else {
				compiledStatus = "syntax error r on line: " + to_string(instr + 1) + '\n';
			}
		}
		else if (isJType(assemblyProgramm[instr][OPCODE_POS])){
			if (isJCorrect(assemblyProgramm[instr])) {
				machineCode = convertJToMachineCode(assemblyProgramm[instr]);
			}
			else {
				compiledStatus =  "syntax error j on line: " + to_string(instr + 1) + '\n';
			}
		}
		else if (isRTType(assemblyProgramm[instr][OPCODE_POS])) {
			if (isRTCorrect(assemblyProgramm[instr])) {
				machineCode = convertRTToMachineCode(assemblyProgramm[instr]);
			}
			else {
				compiledStatus = "syntax error j on line: " + to_string(instr + 1) + '\n';
			}
		}
		else if (isUType(assemblyProgramm[instr][OPCODE_POS])) {
			if (isUCorrect(assemblyProgramm[instr])) {
				machineCode = convertUToMachineCode(assemblyProgramm[instr]);
			}
			else {
				compiledStatus = "syntax error j on line: " + to_string(instr + 1) + '\n';
			}
		}
		else {
			compiledStatus = "syntax error on line: " + to_string(instr + 1) + '\n';
		}

		out.write((char*) &machineCode, sizeof machineCode);
		machineCode = 0;

		if (instr == assemblyProgramm.size() - 1) {
			int temp = 0;                                            // last command - halt.
		    out.write((char*)&temp, sizeof(int));
		}
	}

	out.close();

	compiledStatus = "program compiled successfully\n";
	cout << compiledStatus;
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
		if (commandName == thisTypeCommands[command]) {
			return true;
		}
	}
	return false;
}

bool isJCorrect(vector<string>& assmInstr) {
	return assmInstr.size() == JCOMMANDS_FIELDS_COUNT && handleJOperands(assmInstr);
}

bool isICorrect(vector<string>& assmInstr) {
	return assmInstr.size() == ICOMMANDS_FIELDS_COUNT && handleIOperands(assmInstr);
}

bool isRCorrect(vector<string>& assmInstr) {
	return assmInstr.size() == RCOMMANDS_FIELDS_COUNT && handleROperands(assmInstr);
}

bool isRTCorrect(vector<string>& assmInstr) {
	return assmInstr.size() == RTCOMMANDS_FIELDS_COUNT && handleRTOperands(assmInstr);
}

bool isUCorrect(vector<string>& assmInstr) {
	return assmInstr.size() == UCOMMANDS_FIELDS_COUNT && handleUOperands(assmInstr);
}

bool handleJOperands(vector<string>& assmInstr) {
	return isInMarksTable(assmInstr[RR_POS]);
}

bool handleIOperands(vector<string>& assmInstr) {
	return isRegister(assmInstr[RR_POS]) && isRegister(assmInstr[RS_POS]) && isArgDigit(assmInstr[IMM_POS]);
}

bool handleROperands(vector<string>& assmInstr) {
	return isRegister(assmInstr[RR_POS]) && isRegister(assmInstr[RS_POS]) && isRegister(assmInstr[RT_POS]);
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
		if (reg == registerName) {
			return true;
		}
	}
	return false;
}

bool isInMarksTable(string& arg) {
	if (marksTable.count(arg)) {
		return true;
		
	}
	return false;
}

bool isArgDigit(string& arg) {
	for (int character = 0; character < arg.length(); character++) {
		if (character == 0 && arg[character] == '-') continue;
		if (!isdigit(arg[character])) {
			return false;
		}
	}
	return true;
}

int convertJToMachineCode(vector<string>& assmInstr) {
	int opcode = getOpcode(assmInstr[OPCODE_POS]);
	int addr = getAddr(assmInstr[RR_POS]);
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
	int addr = getAddrOfMark(assmInstr[MARK_POS]);
	return opcode | rr | rs | addr;
}

int getOpcode(string& commandName) {
	int numberOfComand = getNumberFromArray(commandName, commands, COMMANDS_COUNT);
	return shift(numberOfComand, 32 - OPCODE_LENGTH);
}

int getOperand(string& regName, int regPos) {
	int numberOfReg = getNumberFromArray(regName, registersCodes, REG_COUNT);
	return shift(numberOfReg, 32 - (OPCODE_LENGTH + regPos * REGCODE_LENGTH));
}

int getAddr(string& mark) {
	int numberOfMark = marksTable[mark];
	return shift(numberOfMark, 32 - (OPCODE_LENGTH + IMM));
}

int getAddrOfMark(string& mark) {
	int numberOfMark = marksTable[mark];
	return shift(numberOfMark, 32 - (OPCODE_LENGTH + 2 * REGCODE_LENGTH + IMM));
}

int getConstant(string& imm) {
	int constant = stoi(imm);
	int mask = 0x0000FFFF;
	int constant16Bits = constant & mask;
	return shift(constant16Bits, 32 - (OPCODE_LENGTH + 2 * REGCODE_LENGTH + IMM));
}

int getNumberFromArray(string& value, const string* arr, int arrLength) {
	for (int i = 0; i < arrLength; i++) {
		if (value == arr[i]) {
			return i;
		}
	}
}

int shift(int shifted, int shiftValue) {
	int result = shifted;
	for (int i = 0; i < shiftValue; i++) {
		result = result << 1;
	}
	return result;
}

void reverse(string &s)
{
	int tmp;

	for (int i = 0, j = s.length() - 1; i < j; i++, j--)
	{
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}
}

string myTrim(string& str)
{
	string trimmed = ltrim(str);
	reverse(trimmed);
	trimmed = ltrim(trimmed);
	reverse(trimmed);
	return trimmed;
}

string ltrim(string& str) {
	int afterSpaces = 0;
	for (char c : str)
	{
		if (!isspace(c))
			break;
		afterSpaces++;
	}
	return str.substr(afterSpaces, (str.length() - afterSpaces));
}

string cutOffCharacterFromStr(string& str, char character) {
	int beforeDot = 0;
	for (char c : str)
	{
		if (c == character)
			break;
		beforeDot++;
	}
	return str.substr(0, beforeDot);
}



