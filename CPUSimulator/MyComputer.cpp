#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include "MyAssembly.h"
#include "MyOS.h"
#include "MyComputer.h"
#include "utils.h"

using namespace std;

int main() {
	while (true) {
		cout << "\n\n\nDo you want to compile or execute programm? (c/e). q to quite: ";
		string mode = choiceMode();
		if (mode == "c" || mode == "C") {
			complileMode();
		}
		if (mode == "e" || mode == "E") {
			executeMode();
		}
		if (mode == "q" || mode == "Q") {
			cout << "You don't choice any mode. Programm finishs. Good buy!" << endl;
			break;
		}
	}
	_getch();
}

string inputProgramForCompile() {
	cout << "Enter the full name of the file with a program in assembly language: ";
	string pathToAsmProgramm;
	getline(cin, pathToAsmProgramm);
	ifstream fileReader;
	fileReader.open(pathToAsmProgramm);
	string validateMsgs[3] = {"File with the specified name does not exist. ", 
		"Wrong extension of file. Extensiom have to be .txt. ", 
		"Please enter a different file name or q to quite: "};
	validatePath(validateMsgs, fileReader, pathToAsmProgramm, "txt");
	return pathToAsmProgramm;
}

vector<string> inputProgrammsForExecute() {
	string path;
	ifstream fileReader;
	vector<string> programmNames;
	int processCount = 0;
	while (processCount < 7) {
		if (processCount == 6) {
			cout << "There are could be 7 processes or less. You already input 6 processes" << endl;
		}
		cout << "Enter the full name of programm or q to quite: ";
		getline(std::cin, path);
		if (path == "q" || path == "Q") {
			break;
		}
		fileReader.open(path);
		string validateMsgs[3] = { "Programm with the specified name does not exist. ",
			"Wrong extension of file.Extensiom have to be.dat. ",
			"Please enter a different programm or q to quite: " };
		validatePath(validateMsgs, fileReader, path, "tad");
		if (path != "") {
			programmNames.push_back(path);
		}
		processCount++;
	}
	return programmNames;
}

void validatePath(string* validateMsgs, ifstream& fileReader, string& path, string extension) {
	while (!fileReader.is_open() || cutOffSubstrFromStr(reverse(path), '.') != extension) {
		if (!fileReader.is_open()) {
			cout << validateMsgs[0];
		}
		else if (cutOffSubstrFromStr(reverse(path), '.') != extension) {
			cout << validateMsgs[1];
		}
		cout << validateMsgs[2];
		cin >> path;
		if (path == "q" || path == "Q") {
			path = "";
			std::cin.get();
			break;
		}
		fileReader.open(path);
		cin.get();
	}
}

void complileMode() {
	string programmForCompile = inputProgramForCompile();
	if (programmForCompile == "") {
		cout << "You don't choice any files." << endl;
	}
	else {
		compile(programmForCompile);
	}
}

void executeMode() {
	vector<string> programmsForExecute = inputProgrammsForExecute();
	if (programmsForExecute.size() > 0) {
		getProgramms(programmsForExecute);
	}
	else {
		cout << "You don't choice any programms for execute." << endl;
	}
}

string choiceMode() {
	string mode;
	getline(cin, mode);
	while (mode != "c" && mode != "e" && mode != "C" && mode != "E" && mode != "q" && mode != "Q") {
		cout << "Please enter c or e (compile or execute) or q to quite: ";
		getline(cin, mode);
	}
	return mode;
}