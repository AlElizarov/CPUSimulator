#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include "MyArhitecture.h"
#include "MySimulator.h"

using namespace std;

unsigned short pc;
int ram[RAM_SIZE];
short registers[REG_COUNT];

struct Instraction {
	int command;
	int opcode;
	unsigned short rr;
	unsigned short rs;
	unsigned short rt;
	short imm;
};

Instraction instaction;
PCB pcb;

void executeProgramm(PCB pcbInput) {
	pcb = pcbInput;
	pc = pcb.pc;

	for (int i = 0; i < REG_COUNT; i++) {
		registers[i] = pcb.registers[i];
	}

	ifstream fileReader;
	fileReader.open(pcb.path);
	loadProgramm(fileReader);
	
	while (readCommand()) {
		decriptCommand();
		executeCommand();
		pc = pc + TO_PC;
		if (registers[17] != 0) {
			break;
		}
	}
	fileReader.close();
	cout <<pcb.path<<" run status: "<< messgesAfterRun[registers[17]];

}

void loadProgramm(ifstream& fileReader) {
	int commandToMemory;
	int startAdress = pc;
	while (fileReader.read((char*)& commandToMemory, sizeof(int))) {
		ram[startAdress] = commandToMemory;
		startAdress = startAdress + TO_PC;
	}
}

bool readCommand() {
	instaction.command = ram[pc];
	return instaction.command != 0;
}

void decriptCommand() {
	separateOpcode();
	if (isJOpcode()) {
		getJOperands();
	}
	else if (isIOpcode()) {
		getIOperands();
	}
	else if(isROpcode()) {
		getROperands();
	}
	else if (isRTOpcode()) {
		getRTOperands();
	}
	else {
		getUOperands();
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
	int mask = 0x0001FFFE;
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
	int mask = 0x07C00000;
	int shift = CAPACITY - (OPCODE_LENGTH + REGCODE_LENGTH);
	instaction.rr = separate(mask, shift);
}

void getRS() {
	int mask = 0x003E0000;
	int shift = CAPACITY - (OPCODE_LENGTH + 2 * REGCODE_LENGTH);
	instaction.rs = separate(mask, shift);
}

void getRT() {
	int mask = 0x0001F000;
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
	short mask;
	int shift;
	int addr;
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
		mask = 0xFFFF;
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
		mask = 0x7FFF;
		registers[rr] = registers[rr] & mask;
		registers[rr] = registers[rs] >> shift;
		break;
	case 12:
		pc = registers[rr];
		break;
	case 13:
		cout << registers[rr]<<endl;
		break;
	case 14:
		registers[rr] = registers[rs] + imm;
		break;
	case 15:
		addr = registers[rs] + imm;
		if (addr < pcb.startOfStackSegment || addr >= RAM_SIZE) {
			registers[17] = 1;
		}
		else {
			registers[rr] = ram[addr];
		}
		break;
	case 16:
		addr = registers[rs] + imm;
		if (addr < pcb.startOfStackSegment || addr >= RAM_SIZE) {
			registers[17] = 1;
		}
		else {
			ram[addr] = registers[rr];
		}
		break;
	case 17:
		registers[rr] = registers[rs] & imm;
		break;
	case 18:
		registers[rr] = registers[rs] | imm;
		break;
	case 19:
		if (registers[rr] == registers[rs]) {
			pc = imm + pcb.startOfSegment - TO_PC;     //because there will be + TO_PC after execute and before read, so we will read newxt instraction
		}
		break;
	case 20:
		if (registers[rr] != registers[rs]) {
			pc = imm + pcb.startOfSegment - TO_PC;
		}
		break;
	case 21:
		pc = imm + pcb.startOfSegment-TO_PC;
		break;
	case 22:
		registers[18] = pc;
		pc = imm;
		break;
	default:
		registers[17] = 2;
		break;
	}
}


