#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include "Assembly.h"
#include "comp.h"
//#include "CPUSimulatorHeader.h"

using namespace std;

int main() {
	cout << "Do you want to compile or execute programm? (c/e)";
	char mode;
	cin >> mode;
	while (mode != 'c' && mode != 'e' && mode != 'C' && mode != 'E') {
		cout << "Please enter c or e (compile or execute)";
		cin >> mode;
	}
	cin.get();
	if (mode == 'c' || mode == 'C') {
		compile(inputProgramForCompile());
	}
	if (mode == 'e' || mode == 'E') {
		inputProgrammsForExecute();
	}
	_getch();
}

string inputProgramForCompile() {
	cout << "Enter the full name of the file with a program in assembly language: ";
	string pathToAsmProgramm;
	cin >> pathToAsmProgramm;
	ifstream fileReader;
	fileReader.open(pathToAsmProgramm);
	while (!fileReader.is_open()) {
		cout << "File with the specified name does not exist. Please enter a different name: ";
		cin >> pathToAsmProgramm;
		fileReader.open(pathToAsmProgramm);
	}
	return pathToAsmProgramm;
}

vector<string> inputProgrammsForExecute() {
	string path;
	ifstream fileReader;
	vector<string> programmNames;
	while (true) {
		cout << "Enter the full name of programm or q to quite: ";
		getline(cin, path);
		if (path == "q") {
			break;
		}
		cout << path << endl;
		fileReader.open(path);
		while (!fileReader.is_open()) {
			cout << "File with the specified name does not exist. Please enter a different name: ";
			getline(cin, path);
			fileReader.open(path);
		}
		programmNames.push_back(path);
	}
	return programmNames;
}