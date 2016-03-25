#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <clocale>
#include "CPUSimulatorHeader.h"

using namespace std;

int pc;
bool run = true;
string codeSegment[CODE_SEGMENT_SIZE];
string instr;

string opcode;
string rr;
string rs;
string rt;
string imm;
string addr;

void readCommand() {
	instr = codeSegment[pc];
	if(instr == "00000000000000000000000000000000"){
		run = false;
	}
}

void executeProgramm(string& pathToMachineCode) {
	ifstream fileReader;
	fileReader.open(pathToMachineCode);
	loadProgramm(fileReader);

	while (run) {
		readCommand();
		if (run) {
			cout << instr<<endl;
		}
		pc = pc + TO_PC;
		deshifrCommand();
	}
}

void loadProgramm(ifstream& fileReader) {
	string commandToMemory;
	int startAdress = 0;
	while (getline(fileReader, commandToMemory)) {
		codeSegment[startAdress] = commandToMemory;
		startAdress = startAdress + TO_PC;
	}

	pc = 0;
}

void deshifrCommand() {
	getOpcode();
	if (isJOpcode()) {
		getJOperands();
	}
	else if (isIOpcode()) {
		getIOperands();
	}
	else {
		getROperands();
	}
}

void getOpcode() {
	opcode = "";
	for (int i = 0; i < OPCODE_LENGTH; i++) {
		opcode += instr[i];
	}
}