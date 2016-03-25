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
short registers[REG_SIZE];

string opcode;
unsigned short rr;
unsigned short rs;
unsigned short rt;
unsigned short imm;

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
		pc = pc + TO_PC;
		deshifrCommand();
	}
	cout << "result: $s0 = " << registers[1] << " $s1 = " << registers[2] << " $s2 = " << registers[3] << " $t0 = " << registers[4] << endl;
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
		if (opcode == "01110") {
			registers[rr] = registers[rs] + imm;
		}
	}
	else {
		getROperands();
		if (opcode == "00000") {
			registers[rr] = registers[rs] + registers[rt];
		}
	}
}

bool isJOpcode() {
	for (int i = 0; i < JCOMMANDS_SIZE; i++) {
		if (opcode == jOpcodes[i]){
			return true;
		}
	}
	return false;
}

bool isIOpcode() {
	for (int i = 0; i < ICOMMANDS_SIZE; i++) {
		if (opcode == iOpcodes[i]) {
			return true;
		}
	}
	return false;
}

void getJOperands() {
	getImm();
}

void getIOperands() {
	getImm();
	getRR();
	getRS();
}

void getROperands() {
	getRR();
	getRS();
	getRT();
}

void getOpcode() {
	opcode = "";
	for (int i = OPCODE_FINISH; i >= OPCODE_START; i--) {
		opcode += instr[i];
	}
}

int fromBinaryToInt(string& command) {
	int pow = 1;
	int result = 0;
	for (int i = command.length()-1; i >= 0; i--) {
		if (command[i] == '1') {
			result = result + pow;
		}
		pow = pow << 1;
	}
	return result;
}

void getImm() {
	string tmp = "";
	for (int i = IMM_FINISH; i >= IMM_START; i--) {
		tmp += instr[i];
	}
	imm = fromBinaryToInt(tmp);
}

void getRR() {
	string tmp = "";
	for (int i = RR_FINISH; i >= RR_START; i--) {
		tmp += instr[i];
	}
	rr = fromBinaryToInt(tmp);
}

void getRS() {
	string tmp = "";
	for (int i = RS_FINISH; i >= RS_START; i--) {
		tmp += instr[i];
	}
	rs = fromBinaryToInt(tmp);
}

void getRT() {
	string tmp = "";
	for (int i = RT_FINISH; i >= RT_START; i--) {
		tmp += instr[i];
	}
	rt = fromBinaryToInt(tmp);
}