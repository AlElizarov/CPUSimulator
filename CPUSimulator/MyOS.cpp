#include <iostream>
#include <string>
#include <vector>
#include <queue>  
#include "cpu.h"

using namespace std;

void getProgramms(vector<string>& programmsForExecute) {
	queue<PCB> processQueue;
	for (int i = 0; i < programmsForExecute.size(); i++) {
		processQueue.push(initializePCB(i+1, programmsForExecute[i]));
	}
	while (!processQueue.empty()) {
		executeProgramm(processQueue.front());
		processQueue.pop();
	}
}

PCB initializePCB(int processNumber, string path) {
	PCB pcb;
	pcb.path = path;
	pcb.startOfSegment = PROCESS_SEGMENT_SIZE * processNumber;
	pcb.endOfSegment= pcb.startOfSegment + PROCESS_SEGMENT_SIZE -1;

	pcb.startOfTextSegment = pcb.startOfSegment;
	pcb.endOfTextSegment = pcb.startOfTextSegment + TEXT_SEGMENT_SIZE-1;

	pcb.startOfGBSegment = pcb.endOfTextSegment +1;
	pcb.endOfGBSegment = pcb.startOfGBSegment + GLOBAL_DATA_SEGMENT_SIZE - 1;

	pcb.startOfStackSegment = pcb.endOfGBSegment + 1;
	pcb.endOfStackSegment = pcb.endOfSegment;

	for (int i = 0; i < REG_COUNT; i++) {
		pcb.registers[i] = 0;
	}
	pcb.registers[14] = pcb.startOfGBSegment;
	pcb.registers[15] = pcb.startOfStackSegment;
	
	
	pcb.pc = pcb.startOfTextSegment;
	return pcb;
}