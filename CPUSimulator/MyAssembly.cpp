#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>

using namespace std;

bool validateIOperands(string arg1, string arg2, string arg3);
bool validateROperands(string arg1, string arg2, string arg3);
string getMashineCode(vector<vector<string>> &vs);
bool findInRegisters(string arg);
bool isArgDigit(string arg);
int getNumberOfICommand(string command);
string toBinaryCode(int number);
void reverse(string &s);
string firstArgToBinaryCode(string arg);
int getNumberOfReg(string arg);
string constToBinaryCode(int constant);
int getNumberOfRFuncts(string funct);

string registers[4] = {"$s0","$s1","$s2","$s3"};
string iCommands[3] = { "addi", "andi", "ori" };
string rFuncts[3] = { "add", "sub", "mul" };

int main() {
	ifstream infile("D://myassm.txt");
	string line;
	vector<vector<string>> vs;
	while (getline(infile, line))
	{
		istringstream ss(line);
		string token;
		vector<string> tmp;
		while (std::getline(ss, token, ' ')) {
			istringstream ss2(token);
			string token2;
			while (getline(ss2, token2, ',')) {
				tmp.push_back(token2);
			}
		}
		vs.push_back(tmp);
	}
	
	string result = getMashineCode(vs);
	cout << result;

	_getch();
}

string getMashineCode(vector<vector<string>> &vs) {
	string machineCode;
	ofstream out("D://mashineCode.txt");
	for (int i = 0; i < vs.size(); i++) {
		if (vs[i][0] == "addi" || vs[i][0] == "andi" || vs[i][0] == "ori") {
			if (vs[i].size() != 4) {
				return "syntax error";
			}
			if (validateIOperands(vs[i][1], vs[i][2], vs[i][3]) == true) {
				int number = getNumberOfICommand(vs[i][0]);
				machineCode += toBinaryCode(number)+' ';
				machineCode += firstArgToBinaryCode(vs[i][1])+' ';
				machineCode += firstArgToBinaryCode(vs[i][2])+' ';
				machineCode += constToBinaryCode(stoi(vs[i][3]));
			}
			else return "syntax error";
		}
		else if (vs[i][0] == "add") {
			if (vs[i].size() != 4) {
				return "syntax error";
			}
			if (validateROperands(vs[i][1], vs[i][2], vs[i][3]) == true) {
				machineCode += "000000 ";
				machineCode += firstArgToBinaryCode(vs[i][1]) + ' ';
				machineCode += firstArgToBinaryCode(vs[i][2]) + ' ';
				machineCode += firstArgToBinaryCode(vs[i][3]) + ' ';
				machineCode += "00000 ";
				int number = getNumberOfRFuncts(vs[i][0]);
				machineCode += toBinaryCode(number) + ' ';
			}
		}
		else {
			return "syntax error";
		}
		out << machineCode;
		machineCode = "";
		out << '\n';
		//in file
	}
	out.close();
	return machineCode;
}

bool validateIOperands(string arg1, string arg2, string arg3) {
	if(findInRegisters(arg1) && findInRegisters(arg2) && isArgDigit(arg3))
		return true;
}

bool validateROperands(string arg1, string arg2, string arg3) {
	if (findInRegisters(arg1) && findInRegisters(arg2) && findInRegisters(arg3))
		return true;
}

bool findInRegisters(string arg) {
	for (int i = 0; i < 4; i++)
	{
		if (registers[i] == arg) {
			return true;
		}
	}
	return false;
}

bool isArgDigit(string arg) {
	for (int i = 0; i < arg.length(); i++) {
		if (!isdigit(arg[i])) {
			return false;
		}
	}
	return true;
}

int getNumberOfICommand(string command) {
	for (int i = 0; i < 3; i++) {
		if (command == iCommands[i]) {
			return i + 1;
		}
	}
}

string toBinaryCode(int number) {
	string binary;
	int idx = 0;
	while (idx < 6) {
		if (number % 2 == 0) {
			binary += '0';
		}
		else {
			binary += '1';
		}
		number /= 2;
		idx++;
	}
	reverse(binary);
	return binary;
}

void reverse(string &s)
{
	int c, i, j;

	for (i = 0, j = s.length() - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

string firstArgToBinaryCode(string arg) {
	int number = getNumberOfReg(arg);
	string binary;
	int idx = 0;
	while (idx < 5) {
		if (number % 2 == 0) {
			binary += '0';
		}
		else {
			binary += '1';
		}
		number /= 2;
		idx++;
	}
	reverse(binary);
	return binary;
}

int getNumberOfReg(string arg) {
	for (int i = 0; i < 4; i++) {
		if (arg == registers[i]) {
			return i + 1;
		}
	}
}

string constToBinaryCode(int constant) {
	string binary;
	int idx = 0;
	while (idx < 16) {
		if (constant % 2 == 0) {
			binary += '0';
		}
		else {
			binary += '1';
		}
		constant /= 2;
		idx++;
	}
	reverse(binary);
	return binary;
}

int getNumberOfRFuncts(string funct) {
	for (int i = 0; i < 3; i++) {
		if (funct == rFuncts[i]) {
			return i + 1;
		}
	}
}