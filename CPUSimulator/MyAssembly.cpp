#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <clocale>
#include "CPUSimulatorHeader.h"

using namespace std;

int const REG_SIZE = 4;
int const ICOMMANDS_SIZE = 7;
int const RFUNCTS_SIZE = 13;

string registers[REG_SIZE] = {"$s0","$s1","$s2","$s3"};
string iCommands[ICOMMANDS_SIZE] = { "addi", "andi", "ori", "lw", "sw", "beq", "bne" };
string rFuncts[RFUNCTS_SIZE] = { "add", "sub", "mul", "div", "and", "or", "xor", "nor", "slt", "sll", "srl", "sra", "jr" };

string pathToAsmProgramm;

int main() {
	setlocale(LC_ALL, "Russian");

	ifstream assemblyFile = createFileReader();
	createPause();

	string assemblyInstruction;
	vector<vector<string>> assemblyProgramm;

	while (getline(assemblyFile, assemblyInstruction))
	{
		assemblyProgramm.push_back(parseOneAssemblyCommand(assemblyInstruction));
	}

	assemblyFile.close();
	cout<<createMachineCode(assemblyProgramm);
	_getch();
}

void createPause() {
	cout << "Нажмите любую клавишу что бы начать компиляцию...";
	cin.get();
	cin.get();
}

ifstream createFileReader() {
	ifstream fileReader;
	cout << "Введите полное имя файла с программой на ассемблере: ";
	cin >> pathToAsmProgramm;
	fileReader.open(pathToAsmProgramm);
	while (fileReader.is_open() == 0) {
		cout << "Файла с укзанным именем не существует. Пожалуйста, введите другое имя: ";
		cin >> pathToAsmProgramm;
		fileReader.open(pathToAsmProgramm);
	}
	return fileReader;
}

vector<string> parseOneAssemblyCommand(string& assemblyInstruction) {
	vector<string> tmp;
	istringstream iss(assemblyInstruction);
	string subStringAfterSpaces;

	while (getline(iss, subStringAfterSpaces, ' ')) {
		istringstream issForSubstring(subStringAfterSpaces);
		string subStringAfterCommas;
		while (getline(issForSubstring, subStringAfterCommas, ',')) {
			tmp.push_back(subStringAfterCommas);
		}
	}
	return tmp;
}

string createMachineCode(vector<vector<string>>& assemblyInstractions) {
	string machineCode;
	ofstream out("D://machineCode.txt");

	for (int i = 0; i < assemblyInstractions.size(); i++) {
		if (isIType(assemblyInstractions[i][0])) {
			if (isICorrect(assemblyInstractions[i])) {
				machineCode += convertIToMachineCode(assemblyInstractions[i]);
			}
			else {
				return  "syntax error on line: " + to_string(i+1);
			}
		}
		else if (isRType(assemblyInstractions[i][0])) {
			if (isRCorrect(assemblyInstractions[i])){
				machineCode += convertRToMachineCode(assemblyInstractions[i]);
			}
			else {
				return  "syntax error on line: " + to_string(i + 1);
			}
		}
		else {
			return  "syntax error on line: " + to_string(i + 1);
		}

		out << machineCode;
		machineCode = "";
		out << '\n';
	}

	out.close();
	return "program compiled successfully";
}

bool isIType(string& command) {
	return command == "addi" || command == "andi" || command == "ori";
}

bool isRType(string& command) {
	return command == "add" || command == "mul" || command == "sub";
}

bool isICorrect(vector<string>& assmInstr) {
	return assmInstr.size() == 4 && handleIOperands(assmInstr[1], assmInstr[2], assmInstr[3]);
}

bool isRCorrect(vector<string>& assmInstr) {
	return isRSizeCorrect(assmInstr) && handleROperands(assmInstr[1], assmInstr[2], assmInstr[3]);
}

bool isRSizeCorrect(vector<string>& assmInstr) {
	return assmInstr[0] == "jr" && assmInstr.size() == 2 || assmInstr[0] != "jr" && assmInstr.size() == 4;
}

bool handleIOperands(string& arg1, string& arg2, string& arg3) {
	if(isRegister(arg1) && isRegister(arg2) && isArgDigit(arg3))
		return true;
}

bool handleROperands(string& arg1, string& arg2, string& arg3) {
	if (isRegister(arg1) && isRegister(arg2) && isRegister(arg3))
		return true;
}

bool isRegister(string& arg) {
	for (int i = 0; i < REG_SIZE; i++)
	{
		if (registers[i] == arg) {
			return true;
		}
	}
	return false;
}

bool isArgDigit(string& arg) {
	for (int i = 0; i < arg.length(); i++) {
		if (!isdigit(arg[i])) {
			return false;
		}
	}
	return true;
}

string convertIToMachineCode(vector<string>& assmInstr) {
	string opcode = toBinaryCode(getNumberOfICommand(assmInstr[0]), 6) + ' ';
	string rr = toBinaryCode(getNumberOfReg(assmInstr[1]), 5) + ' ';                   // регистр-назначение
	string rs = toBinaryCode(getNumberOfReg(assmInstr[2]), 5) + ' ';                   // регистр-источник
	string constant = toBinaryCode(stoi(assmInstr[3]), 16);                      // константный аргумент
	return opcode + rs + rr + constant;
}

string convertRToMachineCode(vector<string>& assmInstr) {
	string result;
	string opcode = "000000 ";                                         // 6 bits
	string rr = toBinaryCode(getNumberOfReg(assmInstr[1]), 5) + ' ';   // регистр назначение 5 bits
	if (assmInstr[0] == "jr") {
		return opcode + rr
			+ "000000000000000 "                                      // 15 нулей - 15 bits
			+ toBinaryCode(getNumberOfRFuncts(assmInstr[0]), 6);      // funct - 6 bits
	}
	string rs = toBinaryCode(getNumberOfReg(assmInstr[2]), 5) + ' ';   // первый регистр источник 5 bits
	string rt = toBinaryCode(getNumberOfReg(assmInstr[3]), 5) + ' ';   // второй регистр источник 5 bits
	string shamt = "00000 ";
	string funct = toBinaryCode(getNumberOfRFuncts(assmInstr[0]), 6) + ' ';
	return opcode + rs + rt + rr +shamt + funct;
}

int getNumberOfICommand(string& command) {
	for (int i = 0; i < ICOMMANDS_SIZE; i++) {
		if (command == iCommands[i]) {
			return i + 1;
		}
	}
}

int getNumberOfRFuncts(string& funct) {
	for (int i = 0; i < RFUNCTS_SIZE; i++) {
		if (funct == rFuncts[i]) {
			return i + 1;
		}
	}
}

int getNumberOfReg(string& arg) {
	for (int i = 0; i < REG_SIZE; i++) {
		if (arg == registers[i]) {
			return i + 1;
		}
	}
}

string toBinaryCode(int number, int sizeOfBinaryField) {
	string binary;
	for (int i = 0; i < sizeOfBinaryField; i++ ){
		if (number % 2 == 0) {
			binary += '0';
		}
		else {
			binary += '1';
		}
		number /= 2;
	}
	reverse(binary);
	return binary;
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



