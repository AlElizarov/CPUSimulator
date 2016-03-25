#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <clocale>
#include "CPUSimulatorHeader.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	string pathToAsmProgram;
	ifstream assemblyFile = createFileReader(pathToAsmProgram);
	createPause("Нажмите enter что бы начать компиляцию...");

	string assemblyInstruction;
	vector<vector<string>> assemblyProgramm;

	while (getline(assemblyFile, assemblyInstruction))
	{
		assemblyProgramm.push_back(parseOneAssemblyCommand(assemblyInstruction));
	}

	assemblyFile.close();

	string pathToMachineCode = createMachineCodePath(pathToAsmProgram);
	cout << createMachineCode(assemblyProgramm, pathToMachineCode);
	createPause("Нажмите enter что бы начать выполнение...");
	executeProgramm(pathToMachineCode);

	_getch();
}

void createPause(string message) {
	cout << message;
	cin.get();
	cin.get();
}

ifstream createFileReader(string& pathToAsmProgramm) {
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

string createMachineCodePath(string& pathToAsmProgram) {
	istringstream iss(pathToAsmProgram);
	string sub;
	int i = 0;
	string path;
	string extension;
	while (getline(iss, sub, '.')) {
		if (i == 0) {
			path = sub;
		}
		if (i == 1) {
			extension = sub;
		}
		i++;
	}
	return path + "MachineCode." + extension;
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

string createMachineCode(vector<vector<string>>& assemblyInstractions, string& path) {
	string machineCode;
	ofstream out(path);

	for (int i = 0; i < assemblyInstractions.size(); i++) {
		if (isIType(assemblyInstractions[i][0])) {
			if (isICorrect(assemblyInstractions[i])) {
				machineCode += convertIToMachineCode(assemblyInstractions[i]);
			}
			else {
				return  "syntax error on line: " + to_string(i + 1)+'\n';
			}
		}
		else if (isRType(assemblyInstractions[i][0])) {
			if (isRCorrect(assemblyInstractions[i])) {
				machineCode += convertRToMachineCode(assemblyInstractions[i]);
			}
			else {
				return  "syntax error on line: " + to_string(i + 1) + '\n';
			}
		}
		else if (isJType(assemblyInstractions[i][0])){
			if (isJCorrect(assemblyInstractions[i])) {
				machineCode += convertJToMachineCode(assemblyInstractions[i]);
			}
			else {
				return  "syntax error on line: " + to_string(i + 1) + '\n';
			}
		}
		else {
			return  "syntax error on line: " + to_string(i + 1) + '\n';
		}

		out << machineCode;
		machineCode = "";
		out << '\n';
		if (i == assemblyInstractions.size() - 1) {
			out << "00000000000000000000000000000000";                     //32 нуля - halt.
		}
	}

	out.close();
	return "program compiled successfully\n";
}

bool isIType(string& command) {
	for (int i = 0; i < ICOMMANDS_SIZE; i++) {
		if (command == iCommands[i]) {
			return true;
		}
	}
	return false;
}

bool isRType(string& command) {
	for (int i = 0; i < RCOMMANDS_SIZE; i++) {
		if (command == rCommands[i]) {
			return true;
		}
	}
	return false;
}

bool isJType(string& command) {
	for (int i = 0; i < JCOMMANDS_SIZE; i++) {
		if (command == jCommands[i]) {
			return true;
		}
	}
	return false;
}

bool isJCorrect(vector<string>& assmInstr) {
	return assmInstr.size() == 2 && handleJOperands(assmInstr[1]);
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

bool handleJOperands(string& arg) {
	if (isRegister(arg))
		return true;
	return false;
}

bool handleIOperands(string& arg1, string& arg2, string& arg3) {
	if(isRegister(arg1) && isRegister(arg2) && isArgDigit(arg3))
		return true;
	return false;
}

bool handleROperands(string& arg1, string& arg2, string& arg3) {
	if (isRegister(arg1) && isRegister(arg2) && isRegister(arg3))
		return true;
	return false;
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

string convertJToMachineCode(vector<string>& assmInstr) {
	string opcode = toBinaryCode(getNumberOfCommand(assmInstr[0]), OPCODE_LENGTH);
	string addr = toBinaryCode(getNumberOfReg(assmInstr[1]), IMM);                   // регистр с адресом
	return opcode + addr +"00000000000";
}

string convertIToMachineCode(vector<string>& assmInstr) {
	string opcode = toBinaryCode(getNumberOfCommand(assmInstr[0]), OPCODE_LENGTH);
	string rr = toBinaryCode(getNumberOfReg(assmInstr[1]), REGCODE_LENGTH);                   // регистр-назначение
	string rs = toBinaryCode(getNumberOfReg(assmInstr[2]), REGCODE_LENGTH);                   // регистр-источник
	string constant = toBinaryCode(stoi(assmInstr[3]), IMM);                      // константный аргумент
	return opcode + rr + rs + constant+"000";
}

string convertRToMachineCode(vector<string>& assmInstr) {
	string result;
	string opcode = toBinaryCode(getNumberOfCommand(assmInstr[0]), OPCODE_LENGTH);                                         // 5 bits
	string rr = toBinaryCode(getNumberOfReg(assmInstr[1]), REGCODE_LENGTH);   // регистр назначение 4 bits
	if (assmInstr[0] == "jr") {
		return opcode + rr
			+ "00000000000000000000000";                                      // 23 нуля - 23 bits

	}
	string rs = toBinaryCode(getNumberOfReg(assmInstr[2]), REGCODE_LENGTH);   // первый регистр источник 5 bits
	string rt = toBinaryCode(getNumberOfReg(assmInstr[3]), REGCODE_LENGTH);   // второй регистр источник 5 bits
	return opcode + rr + rs + rt + "000000000000000";                     //15 нулей
}

int getNumberOfCommand(string& command) {
	for (int i = 0; i < COMMANDS_SIZE; i++) {
		if (command == commands[i]) {
			return i;
		}
	}
}

int getNumberOfReg(string& arg) {
	for (int i = 0; i < REG_SIZE; i++) {
		if (arg == registers[i]) {
			return i;
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



