#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <clocale>
#include "CPUSimulatorHeader.h"

using namespace std;

int pc = 0;
int ram[RAM_SIZE];
short registers[REG_COUNT];

struct Instraction {
	int command;
	int opcode;
	unsigned short rr;
	unsigned short rs;
	unsigned short rt;
	unsigned short imm;
};

Instraction instaction;


void executeProgramm(string& pathToMachineCode) {
	ifstream fileReader;
	fileReader.open(pathToMachineCode);
	loadProgramm(fileReader);
	
	while (readCommand()) {
		pc = pc + TO_PC;
		decriptCommand();
		executeCommand();
	}
	pc = 0;
	fileReader.close();

	cout << "result:" << endl;
	cout << "$s0 = " << registers[1] << " $s1 = " << registers[2];
}

void loadProgramm(ifstream& fileReader) {
	int commandToMemory;
	int startAdress = 0;
	while (fileReader.read((char*)& commandToMemory, sizeof(int))) {
		ram[startAdress] = commandToMemory;
		startAdress = startAdress + TO_PC;
	}

	for (int i = 0; i < startAdress; i = i + TO_PC) {
		cout << "bin: " << ram[i] << endl;
	}
}

bool readCommand() {
	instaction.command = ram[pc];
	return instaction.command != 0;
}

void decriptCommand() {
	separateOpcode();
	if (isJOpcode()) {
		cout << "yes j"<<endl;
		getJOperands();
	}
	else if (isIOpcode()) {
		getIOperands();
		cout << "yes i" << endl;
	}
	else if(isROpcode()) {
		getROperands();
		cout << "yes r" << endl;
	}
	else if (isRTOpcode()) {
		getRTOperands();
		cout << "yes rt" << endl;
	}
	else {
		getUOperands();
		cout << "yes u" << endl;
	}
}

void separateOpcode() {
	int mask = 0xF8000000;
	int shift = CAPACITY - OPCODE_LENGTH;
	int maskForOpcodeAfterShift = 0x0000001F;
	int opcode = separate(mask, shift);
	opcode = opcode & maskForOpcodeAfterShift;
	instaction.opcode = opcode;
}

bool isJOpcode() {
	return instaction.opcode >= FIRST_J_COMMAND_IDX && instaction.opcode <= LAST_J_COMMAND_IDX;
}

void getJOperands() {
	separateJAddress();
}

void separateJAddress() {
	int mask = 0x07FFF800;
	int shift = CAPACITY - (OPCODE_LENGTH + IMM);
	instaction.imm = separate(mask, shift);
}

bool isIOpcode() {
	return instaction.opcode >= FIRST_I_COMMAND_IDX && instaction.opcode <= LAST_I_COMMAND_IDX;
}

void getIOperands() {
	separateImm();
	getRR();
	getRS();
}

void separateImm() {
	int mask = 0x0007FFF8;
	int shift = CAPACITY - (OPCODE_LENGTH + 2 * REGCODE_LENGTH + IMM);
	instaction.imm = separate(mask, shift);
}

bool isROpcode() {
	return instaction.opcode >= FIRST_R_COMMAND_IDX && instaction.opcode <= LAST_R_COMMAND_IDX;
}

void getROperands() {
	getRR();
	getRS();
	getRT();
}

bool isRTOpcode() {
	return instaction.opcode >= FIRST_RT_COMMAND_IDX && instaction.opcode <= LAST_RT_COMMAND_IDX;
}

void getRTOperands() {
	getRR();
}

void getUOperands() {
	getIOperands();
}

void getRR() {
	int mask = 0x07800000;
	int shift = CAPACITY - (OPCODE_LENGTH + REGCODE_LENGTH);
	instaction.rr = separate(mask, shift);
}

void getRS() {
	int mask = 0x00780000;
	int shift = CAPACITY - (OPCODE_LENGTH + 2 * REGCODE_LENGTH);
	instaction.rs = separate(mask, shift);
}

void getRT() {
	int mask = 0x00078000;
	int shift = CAPACITY - (OPCODE_LENGTH + 3 * REGCODE_LENGTH);
	instaction.rt = separate(mask, shift);
}

int separate(int mask, int shift) {
	int separateValue;
	separateValue = instaction.command & mask;
	separateValue = rightShift(separateValue, shift);
	return separateValue;
}

int rightShift(int shifted, int shiftValue) {
	int result = shifted;
	for (int i = 0; i < shiftValue; i++) {
		result >>= 1;
	}
	return result;
}

void executeCommand() {
	int rr = instaction.rr;
	int rs = instaction.rs;
	int rt = instaction.rt;
	int imm = instaction.imm;

	int mask;
	int shift;
	cout <<"opcode = "<<instaction.opcode<< " rr = " << rr << " rs = " << rs << " rt = " << rt << " imm = " << imm<<endl;
	switch (instaction.opcode)
	{
	case 0:
		registers[rr] = registers[rs] + registers[rt];
		break;
	case 1:
		registers[rr] = registers[rs] - registers[rt];
		break;
	case 2:
		registers[rr] = registers[rs] * registers[rt];
		break;
	case 3:
		registers[rr] = registers[rs] / registers[rt];
		break;
	case 4:
		registers[rr] = registers[rs] & registers[rt];
		break;
	case 5:
		registers[rr] = registers[rs] | registers[rt];
		break;
	case 6:
		registers[rr] = registers[rs] ^ registers[rt];
		break;
	case 7:
		mask = 0xFFFFFFFF;
		registers[rr] = registers[rs] | registers[rt];
		registers[rr] = registers[rr] ^ mask;
		break;
	case 8:
		registers[rr] = registers[rs] < registers[rt] ? 1 : 0;
		break;
	case 9:
		registers[rr] = registers[rs] << registers[rt];
		break;
	case 10:
		registers[rr] = registers[rs] >> registers[rt];
		break;
	case 11:
		shift = registers[rt];
		registers[rr] = registers[rs] >> 1;
		shift--;
		mask = 0x7FFFFFFF;
		registers[rr] = registers[rr] & mask;
		registers[rr] = registers[rs] >> shift;
		break;
	case 12:
		pc = registers[rr];
		break;
	case 13:
		cout << registers[rr];
		break;
	case 14:
		registers[rr] = registers[rs] + imm;
		break;
	case 15:

		break;
	case 16:
		break;
	default:
		break;
	}
}


